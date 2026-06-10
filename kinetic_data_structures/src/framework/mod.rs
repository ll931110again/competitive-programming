//! Core kinetic data structure framework (BGH99).

pub mod event_queue;
pub mod out_of_order;

use std::cmp::Ordering;

/// Result of advancing simulation time.
#[derive(Clone, Debug, PartialEq)]
pub struct AdvanceReport {
    pub events_processed: usize,
    pub new_time: f64,
}

/// A certificate identified by an opaque id; validity is checked externally.
#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub struct CertId(pub u64);

/// Scheduled certificate failure.
#[derive(Clone, Copy, Debug, PartialEq)]
pub struct ScheduledEvent {
    pub time: f64,
    pub cert: CertId,
}

impl Eq for ScheduledEvent {}

impl PartialOrd for ScheduledEvent {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        self.time.partial_cmp(&other.time)
    }
}

impl Ord for ScheduledEvent {
    fn cmp(&self, other: &Self) -> Ordering {
        self.partial_cmp(other).unwrap_or(Ordering::Equal)
    }
}

/// Common interface for kinetic data structures.
pub trait KineticStructure {
    /// Current simulation time.
    fn now(&self) -> f64;

    /// Advance to time `t` (must be >= now), processing all certificate failures.
    fn advance_to(&mut self, t: f64) -> AdvanceReport;

    /// Change trajectory of object `id`; reschedule affected certificates.
    fn change_motion(&mut self, id: usize);

    /// Number of certificates currently stored (compactness proxy).
    fn certificate_count(&self) -> usize;
}
