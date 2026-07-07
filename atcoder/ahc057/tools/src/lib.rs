#![allow(non_snake_case, unused_macros)]

use proconio::input;
use rand::prelude::*;
use std::ops::RangeBounds;
use svg::node::{
    element::{Circle, Definitions, Group, Line, Rectangle, Style, Title},
    Text,
};

pub trait SetMinMax {
    fn setmin(&mut self, v: Self) -> bool;
    fn setmax(&mut self, v: Self) -> bool;
}
impl<T> SetMinMax for T
where
    T: PartialOrd,
{
    fn setmin(&mut self, v: T) -> bool {
        *self > v && {
            *self = v;
            true
        }
    }
    fn setmax(&mut self, v: T) -> bool {
        *self < v && {
            *self = v;
            true
        }
    }
}

#[macro_export]
macro_rules! mat {
	($($e:expr),*) => { Vec::from(vec![$($e),*]) };
	($($e:expr,)*) => { Vec::from(vec![$($e),*]) };
	($e:expr; $d:expr) => { Vec::from(vec![$e; $d]) };
	($e:expr; $d:expr $(; $ds:expr)+) => { Vec::from(vec![mat![$e $(; $ds)*]; $d]) };
}

#[derive(Clone, Debug)]
pub struct Input {
    pub N: usize,
    pub T: usize,
    pub M: usize,
    pub K: usize,
    pub L: i64,
    pub x: Vec<f64>,
    pub y: Vec<f64>,
    pub vx: Vec<f64>,
    pub vy: Vec<f64>,
}

impl std::fmt::Display for Input {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        writeln!(f, "{} {} {} {} {}", self.N, self.T, self.M, self.K, self.L)?;
        for i in 0..self.N {
            writeln!(
                f,
                "{} {} {} {}",
                self.x[i] as i64, self.y[i] as i64, self.vx[i] as i64, self.vy[i] as i64
            )?;
        }
        Ok(())
    }
}

pub fn parse_input(s: &str) -> Input {
    let f = proconio::source::once::OnceSource::from(s);
    input! {
        from f,
        N: usize, T: usize, M: usize, K: usize, L: i64,
        pv: [(i64, i64, i64, i64); N],
    }
    let x = pv.iter().map(|&(x, _, _, _)| x as f64).collect();
    let y = pv.iter().map(|&(_, y, _, _)| y as f64).collect();
    let vx = pv.iter().map(|&(_, _, vx, _)| vx as f64).collect();
    let vy = pv.iter().map(|&(_, _, _, vy)| vy as f64).collect();
    Input {
        N,
        T,
        M,
        K,
        L,
        x,
        y,
        vx,
        vy,
    }
}

pub fn read<T: Copy + PartialOrd + std::fmt::Display + std::str::FromStr, R: RangeBounds<T>>(
    token: Option<&str>,
    range: R,
) -> Result<T, String> {
    if let Some(v) = token {
        if let Ok(v) = v.parse::<T>() {
            if !range.contains(&v) {
                Err(format!("Out of range: {}", v))
            } else {
                Ok(v)
            }
        } else {
            Err(format!("Parse error: {}", v))
        }
    } else {
        Err("Unexpected EOF".to_owned())
    }
}

pub struct Output {
    pub out: Vec<(usize, usize, usize)>, // (t, i, j)
}

pub fn parse_output(input: &Input, f: &str) -> Result<Output, String> {
    let mut out: Vec<(usize, usize, usize)> = Vec::new();
    for (ln, line) in f.lines().filter(|l| !l.trim().is_empty()).enumerate() {
        let mut it = line.split_whitespace();
        let t = read::<usize, _>(it.next(), 0..input.T)?;
        let i = read::<usize, _>(it.next(), 0..input.N)?;
        let j = read::<usize, _>(it.next(), 0..input.N)?;
        if it.next().is_some() {
            return Err(format!("Too many tokens at line {}", ln + 1));
        }
        out.push((t, i, j));
    }
    Ok(Output { out })
}

pub fn gen(seed: u64) -> Input {
    let mut rng = rand_chacha::ChaCha20Rng::seed_from_u64(seed);
    let N = 300usize;
    let T = 1000usize;
    let M = 10usize;
    let K = 30usize;
    let L = 100_000i64; // torus size
    let V = 100i64; // velocity range [-100,100]
    let mut x = Vec::with_capacity(N);
    let mut y = Vec::with_capacity(N);
    let mut vx = Vec::with_capacity(N);
    let mut vy = Vec::with_capacity(N);
    for _ in 0..N {
        x.push(rng.gen_range(0..L) as f64);
        y.push(rng.gen_range(0..L) as f64);
        vx.push(rng.gen_range(-V..=V) as f64);
        vy.push(rng.gen_range(-V..=V) as f64);
    }
    Input {
        N,
        T,
        M,
        K,
        L,
        x,
        y,
        vx,
        vy,
    }
}

pub fn compute_score(input: &Input, out: &Output) -> (i64, String) {
    let (mut score, err, _) = compute_score_details(input, out, input.T - 1);
    if err.len() > 0 {
        score = 0;
    }
    (score, err)
}

pub struct State {
    N: usize,
    M: usize,
    K: usize,
    L: f64,
    t: usize,
    ps: Vec<(f64, f64)>,
    vs: Vec<(f64, f64)>,
    uf: UnionFind,
    failed: Vec<bool>,
    cost: i64,
    err: String,
}

impl State {
    fn new(input: &Input) -> Self {
        Self {
            N: input.N,
            M: input.M,
            K: input.K,
            L: input.L as f64,
            t: 0,
            ps: input
                .x
                .iter()
                .zip(input.y.iter())
                .map(|(&x, &y)| (x, y))
                .collect(),
            vs: input
                .vx
                .iter()
                .zip(input.vy.iter())
                .map(|(&vx, &vy)| (vx, vy))
                .collect(),
            uf: UnionFind::new(input.N),
            failed: vec![false; input.N],
            cost: 0,
            err: String::new(),
        }
    }
    fn unite(&mut self, i: usize, j: usize) {
        if self.uf.same(i, j) {
            self.failed[self.uf.find(i)] = true;
            if self.err.is_empty() {
                self.err = format!("WA: same_component at t={} for pair ({}, {})", self.t, i, j);
            }
            return;
        }
        let mut dx = self.ps[i].0 - self.ps[j].0;
        let mut dy = self.ps[i].1 - self.ps[j].1;
        dx = (dx + self.L * 0.5).rem_euclid(self.L) - self.L * 0.5;
        dy = (dy + self.L * 0.5).rem_euclid(self.L) - self.L * 0.5;
        let d = ((dx * dx + dy * dy).sqrt().round()) as i64;
        self.cost += d;
        let si = self.uf.size(i);
        let sj = self.uf.size(j);
        let ri = self.uf.find(i);
        let rj = self.uf.find(j);
        let svi = self.vs[ri];
        let svj = self.vs[rj];
        let new_vx = (svi.0 * si as f64 + svj.0 * sj as f64) / (si + sj) as f64;
        let new_vy = (svi.1 * si as f64 + svj.1 * sj as f64) / (si + sj) as f64;
        self.uf.unite(i, j);
        let r = self.uf.find(i);
        self.vs[r] = (new_vx, new_vy);
        self.failed[r] |= self.failed[ri] | self.failed[rj];
        if self.uf.size(r) > self.K && self.err.is_empty() {
            self.err = format!("WA: oversize at t={} for pair ({}, {})", self.t, i, j);
        }
    }
    fn step(&mut self) {
        self.t += 1;
        for i in 0..self.N {
            let r = self.uf.find(i);
            self.ps[i].0 = (self.ps[i].0 + self.vs[r].0).rem_euclid(self.L);
            self.ps[i].1 = (self.ps[i].1 + self.vs[r].1).rem_euclid(self.L);
        }
    }
    fn score(&self) -> i64 {
        (1e6 * ((self.L as i64 * (self.N - self.M) as i64) as f64 / (self.cost + 1) as f64).log2())
            .round() as i64
    }
}

pub fn compute_score_details(input: &Input, out: &Output, t: usize) -> (i64, String, State) {
    let mut state = State::new(input);
    let mut ops = vec![vec![]; input.T];
    for &(t, i, j) in &out.out {
        ops[t].push((i, j));
    }
    for t in 0..=t {
        if t > 0 {
            state.step();
        }
        ops[t].sort();
        for &(i, j) in &ops[t] {
            state.unite(i, j);
        }
    }
    if state.err.is_empty() && (0..input.N).filter(|&i| state.uf.find(i) == i).count() != input.M {
        state.err = "Not finished".to_owned();
    }
    (state.score(), state.err.clone(), state)
}

/// 0 <= val <= 1
fn color(val: f64) -> String {
    let s = 0.65;
    let l = 0.55;

    let (r, g, b) = hsl_to_rgb(val, s, l);
    format!("#{:02X}{:02X}{:02X}", r, g, b)
}

fn hsl_to_rgb(h: f64, s: f64, l: f64) -> (u8, u8, u8) {
    let c = (1.0 - (2.0 * l - 1.0).abs()) * s;
    let hp = h * 6.0;
    let x = c * (1.0 - ((hp % 2.0) - 1.0).abs());

    let (r1, g1, b1) = match hp {
        hp if hp < 1.0 => (c, x, 0.0),
        hp if hp < 2.0 => (x, c, 0.0),
        hp if hp < 3.0 => (0.0, c, x),
        hp if hp < 4.0 => (0.0, x, c),
        hp if hp < 5.0 => (x, 0.0, c),
        _ => (c, 0.0, x),
    };

    let m = l - c / 2.0;

    let r = ((r1 + m) * 255.0).round() as u8;
    let g = ((g1 + m) * 255.0).round() as u8;
    let b = ((b1 + m) * 255.0).round() as u8;

    (r, g, b)
}

pub fn rect(x: usize, y: usize, w: usize, h: usize, fill: &str) -> Rectangle {
    Rectangle::new()
        .set("x", x)
        .set("y", y)
        .set("width", w)
        .set("height", h)
        .set("fill", fill)
}

pub fn group(title: String) -> Group {
    Group::new().add(Title::new().add(Text::new(title)))
}

pub fn vis_default(input: &Input, out: &Output) -> (i64, String, String) {
    let (mut score, err, svg, _) = vis(input, out, input.T - 1);
    if err.len() > 0 {
        score = 0;
    }
    (score, err, svg)
}

pub fn vis(input: &Input, out: &Output, t: usize) -> (i64, String, String, i64) {
    let L = input.L as f64;
    let W = 700;
    let H = 700;
    let (score, err, state) = compute_score_details(input, &out, t);
    let (_, _, final_state) = compute_score_details(input, &out, input.T - 1);
    let mut doc = svg::Document::new()
        .set("id", "vis")
        .set("viewBox", (-5, -5, W + 10, H + 10))
        .set("width", W + 10)
        .set("height", H + 10)
        .set("style", "background-color:white");
    doc = doc.add(Style::new(format!(
        "text {{text-anchor: middle;dominant-baseline: central;}}\n
        .vis_v:hover {{r:6}}
        .vis_es:hover .vis_e {{stroke-width:3}}"
    )));
    doc = doc.add(
        rect(0, 0, W, H, "white")
            .set("stroke", "lightgray")
            .set("stroke-width", 1),
    );
    doc = doc.add(
        Definitions::new().add(
            svg::node::element::ClipPath::new()
                .set("id", "clip-area")
                .add(
                    Rectangle::new()
                        .set("x", 0)
                        .set("y", 0)
                        .set("width", W)
                        .set("height", H),
                ),
        ),
    );
    let mut all = Group::new().set("clip-path", "url(#clip-area)");
    let mut ids = vec![!0; input.N];
    let mut ID = 0;
    for i in 0..input.N {
        if final_state.uf.find(i) == i {
            if final_state.uf.size(i) == input.K {
                ids[i] = ID;
                ID += 1;
            }
        }
    }
    for &(t, i, j) in &out.out {
        if t > state.t {
            continue;
        }
        let c = ids[final_state.uf.find(i)];
        let c = if c == !0 || final_state.failed[final_state.uf.find(i)] {
            "gray".to_owned()
        } else {
            color(c as f64 / ID as f64)
        };
        let pi = state.ps[i];
        let pj = state.ps[j];
        let x = if pj.0 - pi.0 > L / 2.0 {
            pj.0 - L
        } else if pj.0 - pi.0 < -L / 2.0 {
            pj.0 + L
        } else {
            pj.0
        };
        let y = if pj.1 - pi.1 > L / 2.0 {
            pj.1 - L
        } else if pj.1 - pi.1 < -L / 2.0 {
            pj.1 + L
        } else {
            pj.1
        };
        let d = ((pi.0 - x).powi(2) + (pi.1 - y).powi(2)).sqrt();
        let mut g =
            group(format!("edge {}-{}\nd = {:.0}\nt = {}", i, j, d, t)).set("class", "vis_es");
        for dx in [-L, 0.0, L] {
            for dy in [-L, 0.0, L] {
                if 0.0 <= pi.0.max(x) + dx
                    && pi.0.min(x) + dx <= L
                    && 0.0 <= pi.1.max(y) + dy
                    && pi.1.min(y) + dy <= L
                {
                    g = g.add(
                        Line::new()
                            .set("x1", (pi.0 + dx) * W as f64 / L)
                            .set("y1", H as f64 - (pi.1 + dy) * H as f64 / L)
                            .set("x2", (x + dx) * W as f64 / L)
                            .set("y2", H as f64 - (y + dy) * H as f64 / L)
                            .set("stroke", c.clone())
                            .set("stroke-width", 1)
                            .set("class", "vis_e"),
                    );
                }
            }
        }
        all = all.add(g);
    }
    for i in 0..input.N {
        let c = ids[final_state.uf.find(i)];
        let c = if c == !0 || final_state.failed[final_state.uf.find(i)] {
            "gray".to_owned()
        } else {
            color(c as f64 / ID as f64)
        };
        let r = state.uf.find(i);
        all = all.add(
            group(format!(
                "point {}\n(x, y) = ({:.0}, {:.0})\n(vx, vy) = ({:.2}, {:.2})\nsize = {}",
                i,
                state.ps[i].0,
                state.ps[i].1,
                state.vs[r].0,
                state.vs[r].1,
                state.uf.size(i)
            ))
            .add(
                Circle::new()
                    .set("cx", state.ps[i].0 * W as f64 / L)
                    .set("cy", H as f64 - state.ps[i].1 * H as f64 / L)
                    .set("r", 3)
                    .set("fill", c)
                    .set("class", "vis_v"),
            ),
        );
    }
    doc = doc.add(all);
    (score, err, doc.to_string(), state.cost)
}

use std::cell::Cell;

#[derive(Clone, Debug)]
pub struct UnionFind {
    /// size / parent
    ps: Vec<Cell<usize>>,
    pub is_root: Vec<bool>,
}

impl UnionFind {
    pub fn new(n: usize) -> UnionFind {
        UnionFind {
            ps: vec![Cell::new(1); n],
            is_root: vec![true; n],
        }
    }
    pub fn find(&self, x: usize) -> usize {
        if self.is_root[x] {
            x
        } else {
            let p = self.find(self.ps[x].get());
            self.ps[x].set(p);
            p
        }
    }
    pub fn unite(&mut self, x: usize, y: usize) {
        let mut x = self.find(x);
        let mut y = self.find(y);
        if x == y {
            return;
        }
        if self.ps[x].get() < self.ps[y].get() {
            ::std::mem::swap(&mut x, &mut y);
        }
        *self.ps[x].get_mut() += self.ps[y].get();
        self.ps[y].set(x);
        self.is_root[y] = false;
    }
    pub fn same(&self, x: usize, y: usize) -> bool {
        self.find(x) == self.find(y)
    }
    pub fn size(&self, x: usize) -> usize {
        self.ps[self.find(x)].get()
    }
}
