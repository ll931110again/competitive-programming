//! Global event queue for kinetic simulation.

use std::cmp::Ordering;
use std::collections::BinaryHeap;

use super::ScheduledEvent;

#[derive(Clone, Copy, Debug, PartialEq)]
struct HeapEntry(ScheduledEvent);

impl Eq for HeapEntry {}

impl PartialOrd for HeapEntry {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        // min-heap by time
        other.0.time.partial_cmp(&self.0.time)
    }
}

impl Ord for HeapEntry {
    fn cmp(&self, other: &Self) -> Ordering {
        self.partial_cmp(other).unwrap_or(Ordering::Equal)
    }
}

/// Priority queue of certificate failure times.
#[derive(Clone, Debug, Default)]
pub struct EventQueue {
    heap: BinaryHeap<HeapEntry>,
}

impl EventQueue {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn push(&mut self, event: ScheduledEvent) {
        self.heap.push(HeapEntry(event));
    }

    pub fn peek_time(&self) -> Option<f64> {
        self.heap.peek().map(|e| e.0.time)
    }

    pub fn pop(&mut self) -> Option<ScheduledEvent> {
        self.heap.pop().map(|e| e.0)
    }

    pub fn len(&self) -> usize {
        self.heap.len()
    }

    pub fn is_empty(&self) -> bool {
        self.heap.is_empty()
    }
}
