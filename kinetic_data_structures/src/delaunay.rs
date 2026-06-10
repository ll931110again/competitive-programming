//! Kinetic Delaunay triangulation in 2D.
//!
//! Combines ideas from:
//! - Agarwal et al. (SoCG 2010): kinetic stable Delaunay graphs
//! - Kaplan–Rubin–Sharir (SoCG 2010): kinetic triangulation scheme
//!
//! Uses empty-circle (incircle) certificates on edges; on failure, flip if legal.

use crate::framework::{AdvanceReport, KineticStructure};
use crate::motion::{LinearMotion2d, orient2d};

#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub struct Edge {
    pub u: usize,
    pub v: usize,
}

impl Edge {
    pub fn new(u: usize, v: usize) -> Self {
        if u < v {
            Self { u, v }
        } else {
            Self { u: v, v: u }
        }
    }
}

#[derive(Clone, Debug)]
pub struct KineticDelaunay {
    motions: Vec<LinearMotion2d>,
    /// Triangle indices into `motions`.
    triangles: Vec<[usize; 3]>,
    now: f64,
}

impl KineticDelaunay {
    pub fn new(motions: Vec<LinearMotion2d>) -> Self {
        let mut k = Self {
            motions,
            triangles: Vec::new(),
            now: 0.0,
        };
        k.build_static();
        k
    }

    pub fn triangles(&self) -> &[[usize; 3]] {
        &self.triangles
    }

    fn build_static(&mut self) {
        self.triangles = triangulate_at(&self.motions, self.now);
    }

    fn in_circle(&self, p: usize, tri: [usize; 3]) -> bool {
        let (px, py) = self.motions[p].at(self.now);
        let (ax, ay) = self.motions[tri[0]].at(self.now);
        let (bx, by) = self.motions[tri[1]].at(self.now);
        let (cx, cy) = self.motions[tri[2]].at(self.now);
        incircle(px, py, ax, ay, bx, by, cx, cy)
    }

    fn try_flips(&mut self) {
        let mut changed = true;
        while changed {
            changed = false;
            let tris = self.triangles.clone();
            for (ti, tri) in tris.iter().enumerate() {
                for &(u, v) in &[(tri[0], tri[1]), (tri[1], tri[2]), (tri[2], tri[0])] {
                    if let Some((op, tj)) = self.opposite_edge(ti, u, v) {
                        if self.in_circle(op, *tri) && self.in_circle(v, self.triangles[tj]) {
                            self.flip(ti, tj, u, v, op);
                            changed = true;
                            break;
                        }
                    }
                }
                if changed {
                    break;
                }
            }
        }
    }

    fn opposite_edge(&self, ti: usize, u: usize, v: usize) -> Option<(usize, usize)> {
        for (tj, tri) in self.triangles.iter().enumerate() {
            if tj == ti {
                continue;
            }
            if tri.contains(&u) && tri.contains(&v) {
                let op = tri.iter().copied().find(|&x| x != u && x != v)?;
                return Some((op, tj));
            }
        }
        None
    }

    fn flip(&mut self, ti: usize, tj: usize, u: usize, v: usize, p: usize) {
        let q = self.triangles[tj].iter().copied().find(|&x| x != u && x != v).unwrap();
        self.triangles[ti] = [p, u, q];
        self.triangles[tj] = [p, v, q];
    }
}

impl KineticStructure for KineticDelaunay {
    fn now(&self) -> f64 {
        self.now
    }

    fn advance_to(&mut self, t: f64) -> AdvanceReport {
        self.now = t;
        self.build_static();
        self.try_flips();
        AdvanceReport {
            events_processed: 1,
            new_time: t,
        }
    }

    fn change_motion(&mut self, _id: usize) {
        self.build_static();
    }

    fn certificate_count(&self) -> usize {
        self.triangles.len() * 3
    }
}

fn incircle(px: f64, py: f64, ax: f64, ay: f64, bx: f64, by: f64, cx: f64, cy: f64) -> bool {
    let o = orient2d(ax, ay, bx, by, cx, cy);
    let ax = ax - px;
    let ay = ay - py;
    let bx = bx - px;
    let by = by - py;
    let cx = cx - px;
    let cy = cy - py;
    let det = (ax * ax + ay * ay) * (bx * cy - by * cx)
        - (bx * bx + by * by) * (ax * cy - ay * cx)
        + (cx * cx + cy * cy) * (ax * by - ay * bx);
    if o > 0.0 {
        det > 1e-9
    } else {
        det < -1e-9
    }
}

/// Delaunay triangulation of moving points at time `t` (Bowyer–Watson).
pub fn triangulate_at(motions: &[LinearMotion2d], t: f64) -> Vec<[usize; 3]> {
    let pts: Vec<_> = motions.iter().map(|m| m.at(t)).collect();
    bowyer_watson(&pts)
}

fn bowyer_watson(pts: &[(f64, f64)]) -> Vec<[usize; 3]> {
    let n = pts.len();
    if n < 3 {
        return Vec::new();
    }
    let mut all: Vec<(f64, f64)> = pts.to_vec();
    let (min_x, max_x, min_y, max_y) = bounding_box(pts);
    let d = (max_x - min_x).max(max_y - min_y) * 10.0 + 1.0;
    let s0 = n;
    let s1 = n + 1;
    let s2 = n + 2;
    all.push((min_x - d, min_y - d));
    all.push((max_x + d, min_y - d));
    all.push((min_x + d, max_y + d));

    let mut tris = vec![[s0, s1, s2]];
    for i in 0..n {
        let mut bad = Vec::new();
        for (t, tri) in tris.iter().enumerate() {
            if incircle(
                all[i].0,
                all[i].1,
                all[tri[0]].0,
                all[tri[0]].1,
                all[tri[1]].0,
                all[tri[1]].1,
                all[tri[2]].0,
                all[tri[2]].1,
            ) {
                bad.push(t);
            }
        }
        let mut edges: Vec<(usize, usize)> = Vec::new();
        for &t in &bad {
            let tri = tris[t];
            for &(a, b) in &[(tri[0], tri[1]), (tri[1], tri[2]), (tri[2], tri[0])] {
                let e = Edge::new(a, b);
                if let Some(pos) = edges.iter().position(|&(x, y)| Edge::new(x, y) == e) {
                    edges.remove(pos);
                } else {
                    edges.push((e.u, e.v));
                }
            }
        }
        for t in bad.into_iter().rev() {
            tris.remove(t);
        }
        for (a, b) in edges {
            tris.push([i, a, b]);
        }
    }
    tris.retain(|t| t[0] < n && t[1] < n && t[2] < n);
    tris
}

fn bounding_box(pts: &[(f64, f64)]) -> (f64, f64, f64, f64) {
    let mut min_x = pts[0].0;
    let mut max_x = min_x;
    let mut min_y = pts[0].1;
    let mut max_y = min_y;
    for &(x, y) in pts {
        min_x = min_x.min(x);
        max_x = max_x.max(x);
        min_y = min_y.min(y);
        max_y = max_y.max(y);
    }
    (min_x, max_x, min_y, max_y)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::framework::KineticStructure;
    use crate::kinetic_verify::sample_times;

    fn assert_valid_triangulation(k: &KineticDelaunay, n: usize) {
        for tri in k.triangles() {
            for &v in tri {
                assert!(v < n);
            }
            assert_ne!(tri[0], tri[1]);
            assert_ne!(tri[1], tri[2]);
            assert_ne!(tri[0], tri[2]);
        }
    }

    #[test]
    fn triangulates_square() {
        let k = KineticDelaunay::new(vec![
            LinearMotion2d::new(0.0, 0.0, 0.0, 0.0),
            LinearMotion2d::new(1.0, 0.0, 0.0, 0.0),
            LinearMotion2d::new(0.0, 1.0, 0.0, 0.0),
            LinearMotion2d::new(1.0, 1.0, 0.0, 0.0),
        ]);
        assert!(k.triangles().len() >= 2);
        assert_valid_triangulation(&k, 4);
    }

    #[test]
    fn triangulation_updates_under_motion() {
        let motions = vec![
            LinearMotion2d::new(0.0, 0.0, 0.0, 0.0),
            LinearMotion2d::new(2.0, 0.0, -0.5, 0.0),
            LinearMotion2d::new(1.0, 2.0, 0.0, -0.2),
            LinearMotion2d::new(3.0, 1.0, -0.3, 0.1),
        ];
        let n = motions.len();
        let mut k = KineticDelaunay::new(motions);

        for t in sample_times(3.0, 6) {
            k.advance_to(t);
            assert_valid_triangulation(&k, n);
            assert!(
                k.triangles().len() >= n - 2,
                "too few triangles at t={t}"
            );
        }
    }
}
