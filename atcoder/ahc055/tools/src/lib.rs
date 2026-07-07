use anyhow::{bail, Context, Result};
use rand::Rng;
use rand_chacha::{rand_core::SeedableRng, ChaCha20Rng};
use std::{fmt, str};
use svg::node::element::SVG;

mod lib_vis;
use lib_vis::*;

#[cfg(target_arch = "wasm32")]
use wasm_bindgen::prelude::*;

#[cfg(target_arch = "wasm32")]
use once_cell::sync::Lazy;

#[cfg(target_arch = "wasm32")]
use std::sync::Mutex;

const N_DEFAULT: usize = 200;
const H_LB: i32 = 100;
const H_UB: i32 = 500;
const C_LB: i32 = 1;
const C_UB: i32 = 6;

const ATTACK_NUMERATOR: f64 = 500.0;
const ATTACK_DENOMINATOR_LB: f64 = 1.0;
const ATTACK_DENOMINATOR_UB: f64 = 500.0;

fn read<T: Copy + PartialOrd + std::fmt::Display + std::str::FromStr>(
    token: Option<&str>,
    lb: T,
    ub: T,
) -> Result<T> {
    if let Some(v) = token {
        if let Ok(v) = v.parse::<T>() {
            if v < lb || ub < v {
                bail!("Out of range: {}", v);
            } else {
                Ok(v)
            }
        } else {
            bail!("Parse error: {}", v);
        }
    } else {
        bail!("Unexpected EOF");
    }
}

#[derive(Clone)]
pub struct Input {
    pub n: usize,
    pub treasure_hitpoints: Vec<i32>,
    pub weapon_max_counts: Vec<i32>,
    pub attack_powers: Vec<Vec<i32>>,
}

impl Input {
    pub fn new() -> Self {
        Input {
            n: 0,
            treasure_hitpoints: vec![],
            weapon_max_counts: vec![],
            attack_powers: vec![],
        }
    }
}

impl fmt::Display for Input {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        writeln!(f, "{}", self.n)?;
        for i in 0..self.n {
            write!(f, "{}", self.treasure_hitpoints[i])?;
            if i != self.n - 1 {
                write!(f, " ")?;
            }
        }
        writeln!(f)?;
        for i in 0..self.n {
            write!(f, "{}", self.weapon_max_counts[i])?;
            if i != self.n - 1 {
                write!(f, " ")?;
            }
        }
        writeln!(f)?;
        for i in 0..self.n {
            for j in 0..self.n {
                write!(f, "{}", self.attack_powers[i][j])?;
                if j != self.n - 1 {
                    write!(f, " ")?;
                }
            }
            writeln!(f)?;
        }
        Ok(())
    }
}

fn parse_input(input: &str) -> Result<Input> {
    let mut tokens = input.trim().split_whitespace();

    // Parse N
    let n = read(tokens.next(), 1, N_DEFAULT).context("N is out of range")?;

    let mut treasure_hitpoints = vec![];
    let mut weapon_max_counts = vec![];
    let mut attack_powers = vec![vec![]; n];

    for c in 0..n {
        treasure_hitpoints.push(
            read(tokens.next(), H_LB, H_UB)
                .context(format!("treasure hitpoints H[{}] is out of range", c))?,
        );
    }
    for c in 0..n {
        weapon_max_counts.push(
            read(tokens.next(), C_LB, C_UB)
                .context(format!("weapon max counts C[{}] is our of range", c))?,
        );
    }
    for i in 0..n {
        for j in 0..n {
            attack_powers[i].push(
                read(
                    tokens.next(),
                    (ATTACK_NUMERATOR / ATTACK_DENOMINATOR_UB).round() as i32,
                    (ATTACK_NUMERATOR / ATTACK_DENOMINATOR_LB).round() as i32,
                )
                .context(format!("attack powers A[{}][{}] is out of range", i, j))?,
            );
        }
    }

    Ok(Input {
        n,
        treasure_hitpoints,
        weapon_max_counts,
        attack_powers,
    })
}

#[derive(Clone)]
pub struct Attack {
    pub is_hand: bool,
    pub weapon_index: usize,
    pub target_treasure_index: usize,
}

impl fmt::Display for Attack {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        if self.is_hand {
            write!(f, "-1 {}", self.target_treasure_index)?;
        } else {
            write!(f, "{} {}", self.weapon_index, self.target_treasure_index)?;
        }
        Ok(())
    }
}

impl Attack {
    pub fn new(is_hand: bool, weapon_index: usize, target_treasure_index: usize) -> Self {
        Attack {
            is_hand,
            weapon_index,
            target_treasure_index,
        }
    }
}

#[derive(Clone)]
enum Op {
    Attack(Attack),
}

impl fmt::Display for Op {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Op::Attack(attack) => write!(f, "{}", attack)?,
        }
        Ok(())
    }
}

#[derive(Clone)]
struct Output {
    ops: Vec<Op>,
}

impl fmt::Display for Output {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        for op in &self.ops {
            write!(f, "{}", op)?;
            writeln!(f)?;
        }
        Ok(())
    }
}

fn parse_output(output: &str) -> Result<Output> {
    let mut ops = vec![];

    for (i, line) in output.trim().lines().enumerate() {
        let line = line.trim();
        if line.is_empty() {
            continue;
        }

        let op = parse_op(line).context(format!("{}-th line", i + 1))?;
        ops.push(op);
    }

    Ok(Output { ops })
}

fn parse_op(line: &str) -> Result<Op> {
    let mut tokens = line.split_whitespace();

    let weapon_index_or_hand =
        read(tokens.next(), -1, N_DEFAULT as i32 - 1).context("weapon index is out of range")?;
    let target_treasure_index =
        read(tokens.next(), 0, N_DEFAULT - 1).context("target treasure index is out of range")?;

    let (is_hand, weapon_index) = if weapon_index_or_hand == -1 {
        (true, 0)
    } else {
        (false, weapon_index_or_hand as usize)
    };

    if tokens.next().is_some() {
        bail!("Too many inputs");
    }

    Ok(Op::Attack(Attack::new(
        is_hand,
        weapon_index,
        target_treasure_index,
    )))
}

#[derive(Clone)]
struct State {
    turn: i32,
    treasure_opened: Vec<bool>,
    treasure_hitpoints: Vec<i32>,
    weapon_counts: Vec<i32>,
    action: String,
}

impl State {
    fn new(input: &Input) -> Self {
        State {
            turn: 0,
            treasure_opened: vec![false; input.n],
            treasure_hitpoints: input.treasure_hitpoints.clone(),
            weapon_counts: input.weapon_max_counts.clone(),
            action: String::new(),
        }
    }

    fn attack(&mut self, input: &Input, attack: &Attack) -> Result<()> {
        self.turn += 1;
        if self.treasure_opened[attack.target_treasure_index] {
            bail!("Treasure is already opened");
        }

        if attack.is_hand {
            self.treasure_hitpoints[attack.target_treasure_index] -= 1;
            self.action = format!(
                "Attack with bare hands (damage: 1) on treasure {}",
                attack.target_treasure_index
            );
        } else {
            if !self.treasure_opened[attack.weapon_index] {
                bail!("Weapon is not available because the treasure is not opened");
            }
            if self.weapon_counts[attack.weapon_index] == 0 {
                bail!("Weapon cannot be used because the count is 0");
            }
            self.treasure_hitpoints[attack.target_treasure_index] -=
                input.attack_powers[attack.weapon_index][attack.target_treasure_index];
            self.weapon_counts[attack.weapon_index] -= 1;
            self.action = format!(
                "Attack with weapon {} (damage: {}) on treasure {}",
                attack.weapon_index,
                input.attack_powers[attack.weapon_index][attack.target_treasure_index],
                attack.target_treasure_index
            );
        }

        if self.treasure_hitpoints[attack.target_treasure_index] <= 0 {
            self.treasure_opened[attack.target_treasure_index] = true;
        }

        Ok(())
    }

    fn is_finished(&self) -> bool {
        self.treasure_opened.iter().all(|&opened| opened)
    }

    fn get_score(&self, input: &Input) -> i32 {
        input.treasure_hitpoints.iter().sum::<i32>() - self.turn + 1
    }
}

#[derive(Clone)]
pub struct VisData {
    state: State,
    #[allow(dead_code)]
    op: Option<Op>,
}

#[derive(Debug)]
pub struct JudgeResult {
    pub score: i32,
}

pub fn judge(
    input_s: &str,
    output_s: &str,
    vis_data_vec: &mut Vec<VisData>,
    input_res: &mut Input,
) -> Result<JudgeResult> {
    let input = parse_input(input_s).context("parse input")?;
    *input_res = input.clone();
    let output = parse_output(output_s).context("parse output")?;

    let mut state = State::new(&input);
    vis_data_vec.push(VisData {
        state: state.clone(),
        op: None,
    });

    for (i, op) in output.ops.into_iter().enumerate() {
        match &op {
            Op::Attack(attack) => {
                state
                    .attack(&input, attack)
                    .context(format!("{}-th attack", i + 1))?;
            }
        }
        vis_data_vec.push(VisData {
            state: state.clone(),
            op: Some(op),
        });
    }

    if !state.is_finished() {
        bail!("You have to open all the treasures");
    }

    Ok(JudgeResult {
        score: state.get_score(&input),
    })
}

pub fn gen(seed: u64, n: Option<usize>) -> Result<Input> {
    let mut rng = ChaCha20Rng::seed_from_u64(seed ^ 94);

    // Don't pass usize or isize ranges to gen_range
    // This leads to non-reproducible results between 64-bit targets and 32-bit targets (e.g. Wasm)
    let n = n.unwrap_or(N_DEFAULT);

    let mut treasure_hitpoints = vec![0; n];
    let mut weapon_max_counts = vec![0; n];
    let mut attack_powers = vec![vec![0; n]; n];

    for i in 0..n {
        treasure_hitpoints[i] = rng.gen_range(H_LB..=H_UB);
        weapon_max_counts[i] = rng.gen_range(C_LB..=C_UB);
        for j in 0..n {
            attack_powers[i][j] = (ATTACK_NUMERATOR
                / rng.gen_range(ATTACK_DENOMINATOR_LB..=ATTACK_DENOMINATOR_UB))
            .round() as i32;
        }
    }

    Ok(Input {
        n,
        treasure_hitpoints,
        weapon_max_counts,
        attack_powers,
    })
}

pub fn draw_svg(
    vis_data: &VisData,
    input: &Input,
    attacker_weapon_index: Option<usize>,
) -> SVG {
    let svg = new_svg();
    draw_treasure_grid(vis_data, input, attacker_weapon_index, svg)
}

#[cfg(target_arch = "wasm32")]
#[wasm_bindgen]
pub fn generate(seed: u64, n: Option<usize>) -> Result<String, JsError> {
    let n = n.filter(|&n| n != 0);
    Ok(gen(seed, n).unwrap().to_string())
}

#[cfg(target_arch = "wasm32")]
#[wasm_bindgen(getter_with_clone)]
pub struct SolInfo {
    pub error: Option<String>,
    pub score: i32,
    pub max_turn: usize,
}

#[cfg(target_arch = "wasm32")]
#[derive(Clone)]
struct VisCache {
    error: Option<String>,
    vis_data_vec: Vec<VisData>,
    input: Input,
}

#[cfg(target_arch = "wasm32")]
static VIS_CACHE: Lazy<Mutex<Option<VisCache>>> = Lazy::new(|| Mutex::new(None));

#[cfg(target_arch = "wasm32")]
#[wasm_bindgen]
pub fn get_sol_info(input_s: &str, output_s: &str) -> Result<SolInfo, JsError> {
    let mut vis_data_vec = vec![];
    let mut input_res = Input::new();
    let res = judge(input_s, output_s, &mut vis_data_vec, &mut input_res);

    match res {
        Ok(res) => {
            *VIS_CACHE.lock().unwrap() = Some(VisCache {
                error: None,
                vis_data_vec: vis_data_vec.clone(),
                input: input_res,
            });

            let sol_info = SolInfo {
                error: None,
                score: res.score,
                max_turn: vis_data_vec.len(),
            };
            Ok(sol_info)
        }
        Err(err) => {
            *VIS_CACHE.lock().unwrap() = Some(VisCache {
                error: Some(format!("{:#}", err)),
                vis_data_vec: vis_data_vec.clone(),
                input: input_res,
            });

            // don't show an error if output is empty
            let sol_info = SolInfo {
                error: Some(format!("{:#}", err)),
                score: 0,
                max_turn: vis_data_vec.len() + if output_s == "" { 0 } else { 1 },
            };
            Ok(sol_info)
        }
    }
}

#[cfg(target_arch = "wasm32")]
#[wasm_bindgen(getter_with_clone)]
pub struct VisResult {
    pub svg: String,
    pub action: String,
}

#[cfg(target_arch = "wasm32")]
#[wasm_bindgen]
pub fn visualize(
    _input: &str,
    _output: &str,
    t: usize,
    attacker_weapon_index: Option<usize>,
) -> Result<VisResult, JsError> {
    console_error_panic_hook::set_once();

    let VisCache {
        error,
        vis_data_vec,
        input,
    } = VIS_CACHE.lock().unwrap().clone().unwrap();

    if let Some(attacker_weapon_index) = attacker_weapon_index {
        if attacker_weapon_index >= input.n {
            return Err(JsError::new(&format!("{:#}", "\"weapon to show A_ij\" is out of range")));
        }
    }

    if t < vis_data_vec.len() {
        let vis_data = &vis_data_vec[t];

        Ok(VisResult {
            svg: draw_svg(&vis_data, &input, attacker_weapon_index).to_string(),
            action: vis_data.state.action.clone(),
        })
    } else {
        Err(JsError::new(&format!("{:#}", error.unwrap())))
    }
}
