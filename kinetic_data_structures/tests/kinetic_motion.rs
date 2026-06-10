//! Cross-cutting tests: kinetic structures stay consistent while time advances.

use kinetic_data_structures::closest_pair::KineticClosestPair;
use kinetic_data_structures::convex_hull::KineticUpperHull;
use kinetic_data_structures::framework::KineticStructure;
use kinetic_data_structures::kinetic_verify::{
    assert_order_sorted, brute_closest_pair, brute_sorted_order, brute_upper_hull,
    sample_times,
};
use kinetic_data_structures::motion::{LinearMotion1d, LinearMotion2d};
use kinetic_data_structures::sorting::KineticSorting;
use kinetic_data_structures::sweepline::KineticTournament;

#[test]
fn sorting_invariant_while_stepping_forward() {
    let scalar = vec![
        LinearMotion1d::new(-2.0, 0.8),
        LinearMotion1d::new(0.0, 1.2),
        LinearMotion1d::new(5.0, -0.4),
        LinearMotion1d::new(1.0, 0.0),
    ];
    let mut k = KineticSorting::new(scalar.clone());
    let mut prev_t = 0.0;

    for t in sample_times(10.0, 40) {
        assert!(t >= prev_t);
        k.advance_to(t);
        let order = k.order();
        assert_order_sorted(&scalar, &order, t);
        assert_eq!(order, brute_sorted_order(&scalar, t));
        prev_t = t;
    }
}

#[test]
fn tournament_and_sorting_agree_on_two_points() {
    let scalar = vec![
        LinearMotion1d::new(0.0, 2.0),
        LinearMotion1d::new(10.0, -1.0),
    ];
    let mut sort = KineticSorting::new(scalar.clone());
    let mut tour = KineticTournament::new(scalar.clone());

    for t in sample_times(8.0, 16) {
        sort.advance_to(t);
        tour.advance_to(t);
        assert_eq!(sort.order()[0], tour.winner());
    }
}

#[test]
fn hull_and_closest_pair_under_coordinated_motion() {
    let motions = vec![
        LinearMotion2d::new(0.0, 0.0, 1.0, 0.0),
        LinearMotion2d::new(3.0, 0.0, -0.5, 0.0),
        LinearMotion2d::new(1.5, 2.0, 0.0, -0.3),
    ];
    let mut hull = KineticUpperHull::new(motions.clone());
    let mut pair = KineticClosestPair::new(motions.clone());

    for t in sample_times(5.0, 12) {
        hull.advance_to(t);
        pair.advance_to(t);

        assert_eq!(hull.hull(), brute_upper_hull(&motions, t).as_slice());

        let (a, b) = pair.closest_pair();
        let (ea, eb, _) = brute_closest_pair(&motions, t);
        assert_eq!((a.min(b), a.max(b)), (ea.min(eb), ea.max(eb)));
    }
}

#[test]
fn advance_is_monotone_in_time() {
    let mut k = KineticSorting::new(vec![
        LinearMotion1d::new(0.0, 1.0),
        LinearMotion1d::new(5.0, -0.5),
    ]);
    let times = [0.0, 1.0, 2.5, 2.5, 4.0, 10.0];
    for &t in &times {
        let before = k.now();
        k.advance_to(t);
        assert!((k.now() - t).abs() < 1e-12);
        assert!(k.now() + 1e-12 >= before);
    }
}
