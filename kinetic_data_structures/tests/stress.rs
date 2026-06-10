//! Stress tests: random linear motions, many time steps, kinetic vs brute force.

use kinetic_data_structures::black_box::{BlackBoxKds, BlackBoxOracle, PredictionInterval};
use kinetic_data_structures::closest_pair::KineticClosestPair;
use kinetic_data_structures::convex_hull::KineticUpperHull;
use kinetic_data_structures::delaunay::KineticDelaunay;
use kinetic_data_structures::framework::KineticStructure;
use kinetic_data_structures::kinetic_verify::{
    brute_argmin, brute_closest_pair, brute_delaunay, brute_greedy_spanner,
    brute_min_enclosing_circle, brute_separating_circle, brute_sorted_order,
    brute_upper_hull, circles_match, normalize_edges, sample_times, triangle_edges,
    unordered_pair, StressRng, EPS,
};
use kinetic_data_structures::mesh_refinement::{KineticMeshRefinement, RefinementParams};
use kinetic_data_structures::min_spanning_circle::KineticMinSpanningCircle;
use kinetic_data_structures::motion::LinearMotion2d;
use kinetic_data_structures::separating_circle::{Color, KineticSeparatingCircle};
use kinetic_data_structures::sorting::KineticSorting;
use kinetic_data_structures::spanner::KineticSpanner;
use kinetic_data_structures::sweepline::KineticTournament;

const SEEDS: u64 = 8;
const STRESS_MIN_N: usize = 50;
const STRESS_MAX_N: usize = 100;
const TIME_END: f64 = 5.0;
const TIME_STEPS: usize = 5;

struct LinearOracle {
    motions: Vec<LinearMotion2d>,
}

impl BlackBoxOracle for LinearOracle {
    fn position(&self, id: usize, t: f64) -> (f64, f64) {
        self.motions[id].at(t)
    }

    fn prediction_interval(&self, _id: usize, from: f64) -> PredictionInterval {
        PredictionInterval {
            valid_until: from + 0.5,
        }
    }
}

fn stress_times(seed: u64) -> Vec<f64> {
    let mut rng = StressRng::new(seed);
    let mut times = sample_times(TIME_END, TIME_STEPS);
    // Add a few random probe times.
    for _ in 0..3 {
        times.push(rng.uniform(0.0, TIME_END));
    }
    times.sort_by(|a, b| a.partial_cmp(b).unwrap());
    times.dedup_by(|a, b| (*a - *b).abs() < 1e-9);
    times
}

#[test]
fn stress_sorting() {
    for seed in 0..SEEDS {
        let mut rng = StressRng::new(seed);
        let n = rng.usize_in(STRESS_MIN_N, STRESS_MAX_N);
        let motions = rng.random_motions_1d(n);
        let mut k = KineticSorting::new(motions.clone());

        for t in stress_times(seed) {
            k.advance_to(t);
            assert_eq!(
                k.order(),
                brute_sorted_order(&motions, t),
                "sorting seed={seed} t={t}"
            );
        }
    }
}

#[test]
fn stress_tournament() {
    for seed in 0..SEEDS {
        let mut rng = StressRng::new(seed.wrapping_add(1_000));
        let n = rng.usize_in(STRESS_MIN_N, STRESS_MAX_N);
        let motions = rng.random_motions_1d(n);
        let mut k = KineticTournament::new(motions.clone());

        for t in stress_times(seed) {
            k.advance_to(t);
            let winner = k.winner();
            let expect = brute_argmin(&motions, t);
            assert_eq!(winner, expect, "tournament seed={seed} t={t}");
            let wval = k.winner_value();
            let eval = motions[expect].at(t);
            assert!((wval - eval).abs() < EPS, "tournament value seed={seed} t={t}");
        }
    }
}

#[test]
fn stress_convex_hull() {
    for seed in 0..SEEDS {
        let mut rng = StressRng::new(seed.wrapping_add(2_000));
        let n = rng.usize_in(STRESS_MIN_N, STRESS_MAX_N);
        let motions = rng.random_motions_2d(n);
        let mut k = KineticUpperHull::new(motions.clone());

        for t in stress_times(seed) {
            k.advance_to(t);
            assert_eq!(
                k.hull(),
                brute_upper_hull(&motions, t).as_slice(),
                "hull seed={seed} t={t}"
            );
        }
    }
}

#[test]
fn stress_closest_pair() {
    for seed in 0..SEEDS {
        let mut rng = StressRng::new(seed.wrapping_add(3_000));
        let n = rng.usize_in(STRESS_MIN_N, STRESS_MAX_N);
        let motions = rng.random_motions_2d(n);
        let mut k = KineticClosestPair::new(motions.clone());

        for t in stress_times(seed) {
            k.advance_to(t);
            let (a, b) = k.closest_pair();
            let (ea, eb, ed) = brute_closest_pair(&motions, t);
            assert_eq!(
                unordered_pair(a, b),
                unordered_pair(ea, eb),
                "closest pair seed={seed} t={t}"
            );
            assert!(
                (k.distance_sq() - ed).abs() < EPS,
                "distance seed={seed} t={t}"
            );
        }
    }
}

#[test]
fn stress_spanner() {
    for seed in 0..SEEDS {
        let mut rng = StressRng::new(seed.wrapping_add(4_000));
        let n = rng.usize_in(STRESS_MIN_N, STRESS_MAX_N);
        let motions = rng.random_motions_2d(n);
        let stretch = rng.uniform(2.0, 4.0);
        let mut k = KineticSpanner::new(motions.clone(), stretch);

        for t in stress_times(seed) {
            k.advance_to(t);
            let got = normalize_edges(k.edges());
            let expect = normalize_edges(&brute_greedy_spanner(&motions, stretch, t));
            assert_eq!(got, expect, "spanner seed={seed} t={t}");
        }
    }
}

#[test]
fn stress_min_spanning_circle() {
    for seed in 0..SEEDS {
        let mut rng = StressRng::new(seed.wrapping_add(5_000));
        let n = rng.usize_in(STRESS_MIN_N, STRESS_MAX_N);
        let motions = rng.random_motions_2d(n);
        let mut k = KineticMinSpanningCircle::new(motions.clone());

        for t in stress_times(seed) {
            k.advance_to(t);
            let got = k.circle();
            let expect = brute_min_enclosing_circle(&motions, t);
            assert!(
                circles_match(got, expect, EPS),
                "min circle seed={seed} t={t}: got {:?} expect {:?}",
                got,
                expect
            );
        }
    }
}

#[test]
fn stress_delaunay() {
    for seed in 0..SEEDS {
        let mut rng = StressRng::new(seed.wrapping_add(6_000));
        let n = rng.usize_in(STRESS_MIN_N, STRESS_MAX_N);
        let motions = rng.random_motions_2d(n);
        let mut k = KineticDelaunay::new(motions.clone());

        for t in stress_times(seed) {
            k.advance_to(t);
            let got = triangle_edges(k.triangles());
            let expect = triangle_edges(&brute_delaunay(&motions, t));
            assert_eq!(got, expect, "delaunay seed={seed} t={t}");
        }
    }
}

#[test]
fn stress_separating_circle() {
    for seed in 0..SEEDS {
        let mut rng = StressRng::new(seed.wrapping_add(7_000));
        let n = rng.usize_in(STRESS_MIN_N, STRESS_MAX_N);
        let motions = rng.random_motions_2d(n);
        let mut colors = rng.random_colors(n);
        // Ensure at least one red and one blue.
        colors[0] = Color::Red;
        colors[1] = Color::Blue;

        let mut k = KineticSeparatingCircle::new(motions.clone(), colors.clone());

        for t in stress_times(seed) {
            k.advance_to(t);
            let got = k.circle();
            let expect = brute_separating_circle(&motions, &colors, t);
            match (got, expect) {
                (Some(g), Some(e)) => {
                    assert!(
                        circles_match(g, e, EPS),
                        "separating circle seed={seed} t={t}: got {:?} expect {:?}",
                        g,
                        e
                    );
                }
                (None, None) => {}
                _ => panic!(
                    "separating circle presence mismatch seed={seed} t={t}: got {:?} expect {:?}",
                    got, expect
                ),
            }
        }
    }
}

#[test]
fn stress_black_box_hull() {
    for seed in 0..SEEDS {
        let mut rng = StressRng::new(seed.wrapping_add(8_000));
        let n = rng.usize_in(STRESS_MIN_N, STRESS_MAX_N);
        let motions = rng.random_motions_2d(n);
        let hull = KineticUpperHull::new(motions.clone());
        let oracle = Box::new(LinearOracle {
            motions: motions.clone(),
        });
        let mut bb = BlackBoxKds::new(hull, oracle, n);

        for t in stress_times(seed) {
            bb.advance_to(t);
            assert_eq!(
                bb.inner().hull(),
                brute_upper_hull(&motions, t).as_slice(),
                "black-box hull seed={seed} t={t}"
            );
        }
    }
}

#[test]
fn stress_mesh_refinement_delaunay() {
    for seed in 0..SEEDS {
        let mut rng = StressRng::new(seed.wrapping_add(9_000));
        let n = rng.usize_in(STRESS_MIN_N, STRESS_MAX_N);
        let motions = rng.random_motions_2d(n);
        let params = RefinementParams {
            min_angle: rng.uniform(0.2, 0.8),
        };
        let mut k = KineticMeshRefinement::new(motions.clone(), params);

        for t in stress_times(seed) {
            k.advance_to(t);
            let got = triangle_edges(k.triangles());
            let expect = triangle_edges(&brute_delaunay(&motions, t));
            assert_eq!(got, expect, "mesh/delaunay seed={seed} t={t}");
        }
    }
}
