#![allow(non_snake_case, unused_macros)]

use itertools::Itertools;
use proconio::input;
use rand::prelude::*;
use std::ops::RangeBounds;
use svg::node::element::{Group, Rectangle, Style, Title};

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
    N: usize,
    w: Vec<Vec<i32>>,
    d: Vec<Vec<i32>>,
}

impl std::fmt::Display for Input {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        writeln!(f, "{}", self.N)?;
        for i in 0..self.N {
            writeln!(f, "{}", self.w[i].iter().join(" "))?;
        }
        for i in 0..self.N {
            writeln!(f, "{}", self.d[i].iter().join(" "))?;
        }
        Ok(())
    }
}

pub fn parse_input(f: &str) -> Input {
    let f = proconio::source::once::OnceSource::from(f);
    input! {
        from f,
        N: usize,
        w: [[i32; N]; N],
        d: [[i32; N]; N],
    }
    Input { N, w, d }
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
    pub out: Vec<char>,
}

pub fn parse_output(input: &Input, f: &str) -> Result<Output, String> {
    let mut ss = f.split_whitespace();
    let mut out = vec![];
    while let Some(c) = ss.next() {
        out.push(read(Some(c), '1'..='Z')?);
        if out.len() > 2 * input.N.pow(3) {
            return Err("Too many operations".to_owned());
        }
    }
    Ok(Output { out })
}

pub fn gen(seed: u64) -> Input {
    let mut rng = rand_chacha::ChaCha20Rng::seed_from_u64(seed);
    let N = 20;
    let mut w = mat![0; N; N];
    let mut d = mat![0; N; N];
    for i in 0..N {
        for j in 0..N {
            if i > 0 || j > 0 {
                w[i][j] = f64::powi(rng.gen_range(1.0..=f64::sqrt(1000.0)), 2).round() as i32;
                d[i][j] = (w[i][j] as f64 * rng.gen_range(10.0..=30.0)).round() as i32;
            }
        }
    }
    Input { N, w, d }
}

pub fn compute_score(input: &Input, out: &Output) -> (i64, String) {
    let (mut score, err, _) = compute_score_details(input, &out.out);
    if err.len() > 0 {
        score = 0;
    }
    (score, err)
}

const DIJ: [(usize, usize); 4] = [(!0, 0), (1, 0), (0, !0), (0, 1)];
const DIR: [char; 4] = ['U', 'D', 'L', 'R'];

pub struct State {
    pi: usize,
    pj: usize,
    w: Vec<Vec<i32>>,
    d: Vec<Vec<i32>>,
    stack: Vec<(i32, i32)>,
    T: usize,
}

impl State {
    fn new(input: &Input) -> Self {
        State {
            pi: 0,
            pj: 0,
            w: input.w.clone(),
            d: input.d.clone(),
            stack: vec![],
            T: 0,
        }
    }
    fn apply(&mut self, c: char) -> Result<(), String> {
        let N = self.w.len();
        match c {
            '1' => {
                if self.w[self.pi][self.pj] == 0 {
                    return Err(format!(
                        "Operation 1 cannot be applied at ({}, {})",
                        self.pi, self.pj
                    ));
                }
                self.stack
                    .push((self.w[self.pi][self.pj], self.d[self.pi][self.pj]));
                self.w[self.pi][self.pj] = 0;
                self.d[self.pi][self.pj] = 0;
            }
            '2' => {
                if self.w[self.pi][self.pj] > 0 {
                    return Err(format!(
                        "Operation 2 cannot be applied at ({}, {})",
                        self.pi, self.pj
                    ));
                }
                let (w, d) = self.stack.pop().ok_or("Stack is empty")?;
                self.w[self.pi][self.pj] = w;
                self.d[self.pi][self.pj] = d;
            }
            c => {
                if let Some(dir) = DIR.iter().position(|&d| d == c) {
                    let (di, dj) = DIJ[dir];
                    let ni = self.pi + di;
                    let nj = self.pj + dj;
                    if ni >= N || nj >= N {
                        return Err(format!(
                            "Operation {} cannot be applied at ({}, {})",
                            c, self.pi, self.pj
                        ));
                    }
                    self.pi = ni;
                    self.pj = nj;
                    self.T += 1;
                    let mut sum_w = vec![0; self.stack.len() + 1];
                    for i in (0..self.stack.len()).rev() {
                        sum_w[i] = sum_w[i + 1] + self.stack[i].0;
                        self.stack[i].1 -= sum_w[i + 1];
                    }
                    for i in 0..self.stack.len() {
                        if self.stack[i].1 <= 0 {
                            return Err(format!("The {i}-th box is crushed"));
                        }
                    }
                    if self.pi == 0 && self.pj == 0 {
                        self.stack.clear();
                    }
                } else {
                    return Err(format!("Invalid operation: {}", c));
                }
            }
        }
        Ok(())
    }
}

pub fn compute_score_details(input: &Input, out: &[char]) -> (i64, String, State) {
    let mut state = State::new(input);
    for &c in out {
        if let Err(err) = state.apply(c) {
            return (0, err, state);
        }
    }
    let mut R = state.stack.len();
    for i in 0..input.N {
        for j in 0..input.N {
            if state.w[i][j] > 0 {
                R += 1;
            }
        }
    }
    let score = if R > 0 {
        (input.N * input.N) as i64 - R as i64
    } else {
        (input.N * input.N) as i64 + 2 * input.N.pow(3) as i64 - state.T as i64
    };
    (score, String::new(), state)
}

/// 0 <= val <= 1
pub fn color(mut val: f64) -> String {
    val.setmin(1.0);
    val.setmax(0.0);
    let (r, g, b) = if val < 0.5 {
        let x = val * 2.0;
        (
            30. * (1.0 - x) + 144. * x,
            144. * (1.0 - x) + 255. * x,
            255. * (1.0 - x) + 30. * x,
        )
    } else {
        let x = val * 2.0 - 1.0;
        (
            144. * (1.0 - x) + 255. * x,
            255. * (1.0 - x) + 30. * x,
            30. * (1.0 - x) + 70. * x,
        )
    };
    format!(
        "#{:02x}{:02x}{:02x}",
        r.round() as i32,
        g.round() as i32,
        b.round() as i32
    )
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
    Group::new().add(Title::new(title))
}

pub fn vis_default(input: &Input, out: &Output) -> (i64, String, String) {
    let (mut score, err, svg) = vis(input, &out.out);
    if err.len() > 0 {
        score = 0;
    }
    (score, err, svg)
}

pub fn vis(input: &Input, out: &[char]) -> (i64, String, String) {
    let D = 600 / input.N;
    let (score, err, state) = compute_score_details(input, &out);
    let W = D * input.N + 2 * D;
    let H = D * input.N;
    let mut doc = svg::Document::new()
        .set("id", "vis")
        .set("viewBox", (-5, -5, W + 10, H + 10))
        .set("width", W + 10)
        .set("height", H + 10)
        .set("style", "background-color:white");
    doc = doc.add(Style::new(format!(
        "text {{text-anchor: middle;dominant-baseline: central;}}"
    )));
    for i in 0..input.N {
        for j in 0..input.N {
            if state.w[i][j] == 0 {
                doc = doc.add(
                    group(format!("({},{})", i, j)).add(
                        rect(D * j, D * i, D, D, "white")
                            .set("stroke", "black")
                            .set("stroke-width", 1),
                    ),
                );
            } else {
                let w = D / 4
                    + ((D - D / 4) as f64 * (state.w[i][j] as f64 / 1000.0).sqrt()).round()
                        as usize;
                let h = D / 4
                    + ((D - D / 4) as f64 * (state.d[i][j].max(0) as f64 / 30000.0).sqrt()).round()
                        as usize;
                let color = color(1.0 - state.d[i][j] as f64 / state.w[i][j] as f64 / 30.0);
                doc = doc.add(
                    group(format!(
                        "({},{})\nw = {}\nd = {}",
                        i, j, state.w[i][j], state.d[i][j]
                    ))
                    .add(
                        rect(D * j, D * i, D, D, "white")
                            .set("stroke", "black")
                            .set("stroke-width", 1),
                    )
                    .add(
                        rect(D * j + (D - w) / 2, D * i + D - h, w, h, &color)
                            .set("stroke", "black")
                            .set("stroke-width", 1),
                    ),
                );
            }
        }
    }
    doc = doc.add(
        rect(state.pj * D, state.pi * D, D, D, "none")
            .set("stroke", "red")
            .set("stroke-width", 2),
    );
    let mut y = 0;
    for &(wi, di) in &state.stack {
        let w = D / 4 + ((D - D / 4) as f64 * (wi as f64 / 1000.0).sqrt()).round() as usize;
        let h = D / 4 + ((D - D / 4) as f64 * (di.max(0) as f64 / 30000.0).sqrt()).round() as usize;
        let color = color(1.0 - di as f64 / wi as f64 / 30.0);
        doc = doc.add(
            group(format!("w = {}\nd = {}", wi, di)).add(
                rect(
                    D * input.N + D + (D - w) / 2,
                    D * input.N - y - h,
                    w,
                    h,
                    &color,
                )
                .set("stroke", "black")
                .set("stroke-width", 1),
            ),
        );
        y += h;
    }
    (score, err, doc.to_string())
}
