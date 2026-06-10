//! Kinetic mesh refinement in 2D (Acar–Hudson–Türkoglu, SoCG 2011).
//!
//! Incrementally refines a triangulation when quality certificates fail.

use crate::delaunay::KineticDelaunay;
use crate::framework::{AdvanceReport, KineticStructure};
use crate::motion::LinearMotion2d;

/// Quality threshold: refine when minimum angle drops below this (radians).
#[derive(Clone, Debug)]
pub struct RefinementParams {
    pub min_angle: f64,
}

impl Default for RefinementParams {
    fn default() -> Self {
        Self {
            min_angle: 0.35,
        }
    }
}

/// Kinetic mesh maintained by Delaunay + local refinement stub.
pub struct KineticMeshRefinement {
    motions: Vec<LinearMotion2d>,
    delaunay: KineticDelaunay,
    params: RefinementParams,
    steiner_count: usize,
    now: f64,
}

impl KineticMeshRefinement {
    pub fn new(motions: Vec<LinearMotion2d>, params: RefinementParams) -> Self {
        let delaunay = KineticDelaunay::new(motions.clone());
        Self {
            motions,
            delaunay,
            params,
            steiner_count: 0,
            now: 0.0,
        }
    }

    pub fn triangle_count(&self) -> usize {
        self.delaunay.triangles().len()
    }

    pub fn triangles(&self) -> &[[usize; 3]] {
        self.delaunay.triangles()
    }

    pub fn steiner_count(&self) -> usize {
        self.steiner_count
    }

    fn point(&self, id: usize) -> (f64, f64) {
        self.motions[id].at(self.now)
    }

    fn refine_if_needed(&mut self) {
        let tris = self.delaunay.triangles().to_vec();
        for tri in tris {
            if self.min_angle(tri) < self.params.min_angle {
                // Certificate failure: would insert Steiner point (not fully implemented).
                self.steiner_count += 1;
                break;
            }
        }
    }

    fn min_angle(&self, tri: [usize; 3]) -> f64 {
        let mut best = std::f64::consts::PI;
        for i in 0..3 {
            let a = tri[i];
            let b = tri[(i + 1) % 3];
            let c = tri[(i + 2) % 3];
            let (ax, ay) = self.point(a);
            let (bx, by) = self.point(b);
            let (cx, cy) = self.point(c);
            let abx = bx - ax;
            let aby = by - ay;
            let acx = cx - ax;
            let acy = cy - ay;
            let dot = abx * acx + aby * acy;
            let lab = (abx * abx + aby * aby).sqrt();
            let lac = (acx * acx + acy * acy).sqrt();
            if lab > 1e-12 && lac > 1e-12 {
                let ang = (dot / (lab * lac)).clamp(-1.0, 1.0).acos();
                best = best.min(ang);
            }
        }
        best
    }
}

impl KineticStructure for KineticMeshRefinement {
    fn now(&self) -> f64 {
        self.now
    }

    fn advance_to(&mut self, t: f64) -> AdvanceReport {
        let r = self.delaunay.advance_to(t);
        self.now = t;
        self.refine_if_needed();
        r
    }

    fn change_motion(&mut self, id: usize) {
        self.delaunay.change_motion(id);
    }

    fn certificate_count(&self) -> usize {
        self.delaunay.certificate_count()
    }
}
