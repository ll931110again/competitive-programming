// Google Test harness for 2251A edge–cloud scheduling.

#include "codeforces/cf2251/scheduler.hh"
#include "codeforces/cf2251/simulator.hh"
#include "codeforces/cf2251/task_table.hh"

#include <gtest/gtest.h>

#include <cmath>
#include <random>

using edge_cloud::compute_score;
using edge_cloud::example1;
using edge_cloud::Policy;
using edge_cloud::Scenario;
using edge_cloud::Scheduler;
using edge_cloud::SimResult;
using edge_cloud::Simulator;
using edge_cloud::TaskTable;
using edge_cloud::transfer_ms;

TEST(TaskTable, InterpolatesAndClamps) {
  TaskTable t;
  const double a[] = {2, -1, -1, -1, -1, -1};
  const double b[] = {6, -1, -1, -1, -1, -1};
  t.add_row(1, a);
  t.add_row(5, b);
  t.finalize();
  EXPECT_DOUBLE_EQ(t.lookup(edge_cloud::Step::PrefillPre, 1), 2);
  EXPECT_DOUBLE_EQ(t.lookup(edge_cloud::Step::PrefillPre, 5), 6);
  EXPECT_DOUBLE_EQ(t.lookup(edge_cloud::Step::PrefillPre, 3), 4);
  EXPECT_DOUBLE_EQ(t.lookup(edge_cloud::Step::PrefillPre, 0), 2);
  EXPECT_DOUBLE_EQ(t.lookup(edge_cloud::Step::PrefillPre, 9), 6);
}

TEST(TaskTable, TransferFormula) {
  EXPECT_DOUBLE_EQ(transfer_ms(2, 1, 125000, 1), 3);
  EXPECT_DOUBLE_EQ(transfer_ms(2, 1, 125000, 4), 6);
}

TEST(Score, Example1Components) {
  const auto s = compute_score(0.022222222, 0.0625, 30, 15, 0, 0.5, 0.5, 0, 45, 30, 0, 1);
  EXPECT_NEAR(s.score, 500.0, 1e-5);
  EXPECT_NEAR(s.tdr, 30.0, 1e-12);
  EXPECT_NEAR(s.tpot, 0.0, 1e-12);
}

TEST(Simulator, Example1MatchesOfficialTrace) {
  Scheduler sched;
  Simulator sim;
  const SimResult res = sim.run(example1(), sched);
  ASSERT_TRUE(res.ok) << res.error;
  EXPECT_NEAR(res.last_time, 45.0, 1e-9);
  EXPECT_NEAR(res.score.tdr, 30.0, 1e-9);
  EXPECT_NEAR(res.score.score, 500.0, 1e-3);
  EXPECT_EQ(res.tokens, 1);
}

TEST(Simulator, TwoCloudsFinish) {
  Scenario sc = example1();
  sc.cfg.k = 2;
  sc.cfg.dist_base = 1;
  sc.cfg.tp_base = 0;
  sc.reqs = {{0.0, 4, 2}, {0.0, 8, 2}};
  Scheduler sched;
  Simulator sim;
  const SimResult res = sim.run(sc, sched);
  ASSERT_TRUE(res.ok) << res.error;
  EXPECT_EQ(res.tokens, 4);
  EXPECT_GT(res.score.score, 0.0);
}

TEST(Stress, RandomCasesStayLegal) {
  std::mt19937_64 rng(7);
  std::uniform_int_distribution<int> dk(1, 4);
  std::uniform_int_distribution<int> dr(1, 12);
  std::uniform_int_distribution<int> dlin(1, 16);
  std::uniform_int_distribution<int> dlout(1, 4);
  std::uniform_int_distribution<int> dlayers(1, 6);
  std::uniform_real_distribution<double> dgap(0.0, 20.0);
  for (int trial = 0; trial < 80; trial++) {
    Scenario sc = example1();
    sc.cfg.k = dk(rng);
    sc.cfg.num_layers = dlayers(rng);
    sc.cfg.dist_base = 2;
    sc.cfg.tp_base = 0;
    sc.reqs.clear();
    double t = 0;
    const int r = dr(rng);
    for (int i = 0; i < r; i++) {
      if (i) {
        t += dgap(rng);
      }
      sc.reqs.push_back({t, dlin(rng), dlout(rng)});
    }
    Scheduler sched;
    Simulator sim;
    const SimResult res = sim.run(sc, sched);
    ASSERT_TRUE(res.ok) << "trial " << trial << " " << res.error;
  }
}

TEST(Scheduler, Example1FirstAssignment) {
  Scheduler sched;
  sched.init(example1().cfg);
  edge_cloud::FrameEvent ev;
  ev.type = edge_cloud::FrameEvent::Type::Arr;
  ev.raw = "ARR 0 4";
  const auto asg = sched.on_frame({ev});
  ASSERT_EQ(asg.size(), 1u);
  EXPECT_EQ(asg[0].format(), "E P PRE 0 0");
}

TEST(Stress, SuperlinearTableCutsGroupSize) {
  Scenario sc = example1();
  sc.cfg.k = 1;
  sc.cfg.dist_base = 5;
  sc.cfg.tp_base = 0;
  sc.cfg.table = {};
  const double c1[] = {3, 10, 2, 1, 1, 1};
  const double c2[] = {3, 10, 2, 3, 3, 3};
  const double c4[] = {3, 10, 2, 20, 20, 20};
  const double c8[] = {3, 10, 2, 80, 80, 80};
  sc.cfg.table.add_row(1, c1);
  sc.cfg.table.add_row(2, c2);
  sc.cfg.table.add_row(4, c4);
  sc.cfg.table.add_row(8, c8);
  sc.cfg.table.finalize();
  sc.reqs = {{0, 4, 2}, {0, 4, 2}, {0, 4, 2}, {0, 4, 2}};
  Policy smart;
  smart.auto_tune = false;
  Policy full = smart;
  full.smart_batch = false;
  Scheduler a;
  a.set_policy(smart);
  Scheduler b;
  b.set_policy(full);
  Simulator sim;
  const SimResult ga = sim.run(sc, a);
  const SimResult gb = sim.run(sc, b);
  ASSERT_TRUE(ga.ok) << ga.error;
  ASSERT_TRUE(gb.ok) << gb.error;
  EXPECT_GT(ga.score.score, gb.score.score);
}

TEST(Stress, BatchBeatsSerialOnOverlap) {
  Scenario sc = example1();
  sc.cfg.k = 2;
  sc.cfg.dist_base = 2;
  sc.cfg.tp_base = 0;
  sc.reqs = {{0.0, 4, 4}, {0.5, 4, 4}};
  Policy batched;
  batched.auto_tune = false;
  Policy serial = batched;
  serial.batch = false;
  Scheduler a;
  a.set_policy(batched);
  Scheduler b;
  b.set_policy(serial);
  Simulator sim;
  const SimResult ga = sim.run(sc, a);
  const SimResult gb = sim.run(sc, b);
  ASSERT_TRUE(ga.ok) << ga.error;
  ASSERT_TRUE(gb.ok) << gb.error;
  EXPECT_GE(ga.score.score + 1e-9, gb.score.score);
}
