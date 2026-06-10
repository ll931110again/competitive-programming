# Kinetic Data Structures (2005–June 2012)

Rust implementations of classic kinetic data structures from the literature.
Each module maps to a paper listed below.

## Framework

| Module | Paper |
|--------|-------|
| `framework` | Basch–Guibas–Hershberger (BGH99) certificate/event model |
| `motion` | Affine & bounded-degree polynomial trajectories |
| `sweepline` | Russel, Karavelas, Guibas (2007) — kinetic tournament / sweepline primitives |

## Structures

| Module | Paper | Notes |
|--------|-------|-------|
| `sorting` | Abam & de Berg, SoCG 2005 | Kinetic sorting / predecessor on a line |
| `convex_hull` | Abam & de Berg, SoCG 2005 | 2D upper convex hull, linear motion |
| `out_of_order` | Abam et al., ESA 2006 | Batch processing of certificate failures |
| `closest_pair` | Agarwal–Kaplan–Sharir, TALG 2008 | Directional certificate method (BGZ-style) |
| `spanner` | Abam et al., SoCG 2008 | Kinetic greedy spanner in ℝᵈ |
| `min_spanning_circle` | Demaine–Eisenstat–Guibas–Schulz, 2010 | Smallest enclosing circle |
| `delaunay` | Agarwal et al. SoCG 2010 + Kaplan–Rubin–Sharir SoCG 2010 | 2D Delaunay via edge-flip certificates |
| `black_box` | de Berg–Roeloffzen–Speckmann, SoCG 2011 | Black-box kinetic wrapper |
| `mesh_refinement` | Acar–Hudson–Türkoglu, SoCG 2011 | Interface + incremental refinement stub |
| `separating_circle` | Cheung–Daescu–Zivanic, COCOA 2011 | Red–blue minimum separating circle |

Russel (2007 PhD thesis) is the engineering companion to the `sweepline` module;
full CGAL-level exact arithmetic is not replicated here.

## Usage

```rust
use kinetic_data_structures::motion::LinearMotion2d;
use kinetic_data_structures::sorting::KineticSorting;

let motions = vec![
    LinearMotion2d::new(0.0, 0.0, 1.0, 0.0), // moves right
    LinearMotion2d::new(2.0, 0.0, 0.0, 0.0), // stationary
];
let mut kds = KineticSorting::new(motions);
kds.advance_to(1.5);
assert_eq!(kds.order(), vec![1, 0]); // point 1 passed point 0
```

## Tests

From the repo root (Buck2 — same graph as C++ `lib/` tests):

```bash
make test
# or Rust only:
../tools/buck2 test //kinetic_data_structures/...
```

Local Rust-only iteration still works:

```bash
cargo test
```

## Scope

Implementations target **linear motion** in low dimensions with `f64` arithmetic.
Higher-degree trajectories and exact predicates are left as extensions.
