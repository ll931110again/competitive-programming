//! Kinetic data structures library (2005–2012 literature).
//!
//! See `README.md` for paper-to-module mapping.

pub mod motion;

pub mod framework;
pub mod sweepline;

pub mod sorting;
pub mod convex_hull;
pub mod closest_pair;
pub mod spanner;
pub mod min_spanning_circle;
pub mod delaunay;
pub mod black_box;
pub mod mesh_refinement;
pub mod separating_circle;

pub use framework::{AdvanceReport, CertId, KineticStructure, ScheduledEvent};
pub use motion::{LinearMotion1d, LinearMotion2d};

/// Brute-force references for validating kinetic structures in tests.
pub mod kinetic_verify;
