use kinetic_data_structures::{
    black_box::{BlackBoxKds, BlackBoxOracle, PredictionInterval},
    closest_pair::KineticClosestPair,
    convex_hull::KineticUpperHull,
    delaunay::KineticDelaunay,
    framework::KineticStructure,
    mesh_refinement::{KineticMeshRefinement, RefinementParams},
    min_spanning_circle::KineticMinSpanningCircle,
    motion::LinearMotion2d,
    separating_circle::{Color, KineticSeparatingCircle},
    sorting::KineticSorting,
    spanner::KineticSpanner,
    sweepline::KineticTournament,
};

struct LinearOracle {
    motions: Vec<LinearMotion2d>,
}

impl BlackBoxOracle for LinearOracle {
    fn position(&self, id: usize, t: f64) -> (f64, f64) {
        self.motions[id].at(t)
    }

    fn prediction_interval(&self, _id: usize, from: f64) -> PredictionInterval {
        PredictionInterval {
            valid_until: from + 1.0,
        }
    }
}

#[test]
fn full_pipeline_smoke() {
    let motions = vec![
        LinearMotion2d::new(0.0, 0.0, 1.0, 0.0),
        LinearMotion2d::new(5.0, 0.0, -0.5, 0.0),
        LinearMotion2d::new(2.0, 3.0, 0.0, 0.0),
    ];

    let mut sort = KineticSorting::from_2d_x(&motions);
    sort.advance_to(4.0);
    assert_eq!(sort.order().len(), 3);

    let mut hull = KineticUpperHull::new(motions.clone());
    hull.advance_to(2.0);
    assert!(!hull.hull().is_empty());

    let mut cp = KineticClosestPair::new(motions.clone());
    cp.advance_to(3.0);
    let (a, b) = cp.closest_pair();
    assert_ne!(a, b);

    let mut sp = KineticSpanner::new(motions.clone(), 2.0);
    sp.advance_to(1.0);
    assert!(!sp.edges().is_empty());

    let mut msc = KineticMinSpanningCircle::new(motions.clone());
    msc.advance_to(1.0);
    assert!(msc.circle().r >= 0.0);

    let delaunay = KineticDelaunay::new(motions.clone());
    assert!(!delaunay.triangles().is_empty());

    let hull2 = KineticUpperHull::new(motions.clone());
    let oracle = Box::new(LinearOracle {
        motions: motions.clone(),
    });
    let mut bb = BlackBoxKds::new(hull2, oracle, motions.len());
    bb.advance_to(0.5);

    let mesh = KineticMeshRefinement::new(motions.clone(), RefinementParams::default());
    assert!(mesh.triangle_count() >= 1);

    let sep = KineticSeparatingCircle::new(
        motions[..2].to_vec(),
        vec![Color::Red, Color::Blue],
    );
    assert!(sep.circle().is_some());

    let mut kt = KineticTournament::new(vec![
        kinetic_data_structures::LinearMotion1d::new(0.0, 1.0),
        kinetic_data_structures::LinearMotion1d::new(10.0, 0.0),
    ]);
    kt.advance_to(5.0);
    assert_eq!(kt.winner(), 0);
}
