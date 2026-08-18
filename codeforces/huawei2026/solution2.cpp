#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
using namespace std;

struct RequestState {
  int rid;
  int assigned; // remote computer assigned
  int stage;    // 0: need P_PRE, 1: need P_PROC, 2: need P_POST, 3: need D_PRE, 4: need D_PROC, 5:
                // need D_POST
  bool waiting_up;
  bool waiting_down;
  int next_stage_after_up;
  int next_stage_after_down;
  int tokens_produced;
  bool finished;

  RequestState(int id, int K)
      : rid(id), assigned(id % K), stage(0), waiting_up(false), waiting_down(false),
        next_stage_after_up(-1), next_stage_after_down(-1), tokens_produced(0), finished(false) {}
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  string line;
  // read first line: K S latency_in_ms bandwidth_gbps bytes_per_token num_layers
  if (!getline(cin, line))
    return 0;
  stringstream ss(line);
  int K;
  double S, latency_in_ms, bandwidth_gbps;
  int bytes_per_token, num_layers;
  if (!(ss >> K >> S >> latency_in_ms >> bandwidth_gbps >> bytes_per_token >> num_layers))
    return 0;

  // second line: scoring parameters (ignore)
  if (!getline(cin, line))
    return 0;

  // third line: N
  if (!getline(cin, line))
    return 0;
  int N;
  stringstream ss2(line);
  if (!(ss2 >> N))
    return 0;

  // read N rows of task-time table (ignore)
  for (int i = 0; i < N; ++i) {
    if (!getline(cin, line))
      break;
  }

  bool local_free = true;
  vector<bool> remote_free(K, true);
  vector<RequestState> requests; // index by rid

  auto ensure_request = [&](int rid) {
    if (rid >= (int)requests.size()) {
      requests.resize(rid + 1, RequestState(rid, K));
    }
  };

  auto handle_tdn = [&](const string& server, const string& task_spec) {
    // Free the resource that completed the task
    if (server == "E") {
      local_free = true;
    } else if (!server.empty() && server[0] == 'C') {
      // server format: C<number>
      int k = 0;
      stringstream ss(server.substr(1));
      if (ss >> k) {
        if (k >= 0 && k < K) {
          remote_free[k] = true;
        }
      }
    }

    stringstream ss(task_spec);
    string first;
    ss >> first;
    if (first == "P") {
      string second;
      ss >> second;
      if (second == "PRE") {
        int remote, rid;
        if (ss >> remote >> rid) {
          ensure_request(rid);
          RequestState& req = requests[rid];
          if (!req.finished) {
            req.waiting_up = true;
            req.next_stage_after_up = 1; // after UP, can do P_PROC
          }
        }
      } else if (second == "PROC") {
        // ls le remote rid
        int ls, le, remote, rid;
        if (ss >> ls >> le >> remote >> rid) {
          ensure_request(rid);
          RequestState& req = requests[rid];
          if (!req.finished) {
            req.waiting_down = true;
            req.next_stage_after_down = 2; // after DOWN, can do P_POST
          }
        }
      } else if (second == "POST") {
        int remote, rid;
        if (ss >> remote >> rid) {
          ensure_request(rid);
          RequestState& req = requests[rid];
          if (!req.finished) {
            req.stage = 3; // ready for D_PRE
            req.waiting_up = req.waiting_down = false;
            req.next_stage_after_up = req.next_stage_after_down = -1;
          }
        }
      }
    } else if (first == "D") {
      string second;
      ss >> second;
      if (second == "PRE") {
        int dummy, m;
        if (ss >> dummy >> m) { // dummy is -1
          vector<int> rids(m);
          for (int i = 0; i < m; ++i) {
            if (!(ss >> rids[i]))
              break;
          }
          for (int rid : rids) {
            ensure_request(rid);
            RequestState& req = requests[rid];
            if (!req.finished) {
              req.waiting_up = true;
              req.next_stage_after_up = 4; // after UP, can do D_PROC
            }
          }
        }
      } else if (second == "PROC") {
        int remote, m;
        if (ss >> remote >> m) {
          vector<int> rids(m);
          for (int i = 0; i < m; ++i) {
            if (!(ss >> rids[i]))
              break;
          }
          for (int rid : rids) {
            ensure_request(rid);
            RequestState& req = requests[rid];
            if (!req.finished) {
              req.waiting_down = true;
              req.next_stage_after_down = 5; // after DOWN, can do D_POST
            }
          }
        }
      } else if (second == "POST") {
        int dummy, m;
        if (ss >> dummy >> m) { // dummy is -1
          vector<int> rids(m);
          for (int i = 0; i < m; ++i) {
            if (!(ss >> rids[i]))
              break;
          }
          for (int rid : rids) {
            ensure_request(rid);
            RequestState& req = requests[rid];
            if (!req.finished) {
              req.tokens_produced += 1;
              // ready for another D_PRE (assuming more tokens)
              req.stage = 3;
              req.waiting_up = req.waiting_down = false;
              req.next_stage_after_up = req.next_stage_after_down = -1;
            }
          }
        }
      }
    }
  };

  auto handle_xdn = [&](const string& direction, int remote, int size, const string& typ, int m,
                        const vector<int>& rids) {
    for (int rid : rids) {
      ensure_request(rid);
      RequestState& req = requests[rid];
      if (req.finished)
        continue;
      if (typ == "PRE") {
        if (direction == "UP") {
          if (req.waiting_up) {
            req.waiting_up = false;
            req.stage = req.next_stage_after_up;
            req.next_stage_after_up = req.next_stage_after_down = -1;
          }
        } else { // DOWN
          if (req.waiting_down) {
            req.waiting_down = false;
            req.stage = req.next_stage_after_down;
            req.next_stage_after_up = req.next_stage_after_down = -1;
          }
        }
      } else { // DEC
        if (direction == "UP") {
          if (req.waiting_up) {
            req.waiting_up = false;
            req.stage = req.next_stage_after_up;
            req.next_stage_after_up = req.next_stage_after_down = -1;
          }
        } else { // DOWN
          if (req.waiting_down) {
            req.waiting_down = false;
            req.stage = req.next_stage_after_down;
            req.next_stage_after_up = req.next_stage_after_down = -1;
          }
        }
      }
    }
  };

  auto handle_fin = [&](int rid) {
    ensure_request(rid);
    RequestState& req = requests[rid];
    if (!req.finished) {
      req.finished = true;
    }
  };

  // function to try schedule one task, returns true if scheduled and pushes assignment to out
  auto try_schedule = [&](vector<string>& out) -> bool {
    // local computer
    if (local_free) {
      for (size_t i = 0; i < requests.size(); ++i) {
        RequestState& req = requests[i];
        if (req.finished)
          continue;
        if (req.waiting_up || req.waiting_down)
          continue;
        if (req.stage == 0) { // need P_PRE
          out.push_back("E P PRE " + to_string(req.assigned) + " " + to_string(req.rid));
          local_free = false;
          return true;
        } else if (req.stage == 2) { // need P_POST
          out.push_back("E P POST " + to_string(req.assigned) + " " + to_string(req.rid));
          local_free = false;
          return true;
        } else if (req.stage == 3) { // need D_PRE
          out.push_back("E D PRE -1 1 " + to_string(req.rid));
          local_free = false;
          return true;
        } else if (req.stage == 5) { // need D_POST
          out.push_back("E D POST -1 1 " + to_string(req.rid));
          local_free = false;
          return true;
        }
      }
    }
    // remote computers
    for (int k = 0; k < K; ++k) {
      if (remote_free[k]) {
        for (size_t i = 0; i < requests.size(); ++i) {
          RequestState& req = requests[i];
          if (req.finished)
            continue;
          if (req.waiting_up || req.waiting_down)
            continue;
          if (req.assigned != k)
            continue;
          if (req.stage == 1) { // need P_PROC
            out.push_back("C" + to_string(k) + " P PROC 0 " + to_string(num_layers) + " " +
                          to_string(k) + " " + to_string(req.rid));
            remote_free[k] = false;
            return true;
          } else if (req.stage == 4) { // need D_PROC
            out.push_back("C" + to_string(k) + " D PROC " + to_string(k) + " 1 " +
                          to_string(req.rid));
            remote_free[k] = false;
            return true;
          }
        }
      }
    }
    return false;
  };

  // main loop
  while (true) {
    string timestamp_line;
    if (!getline(cin, timestamp_line))
      break;
    if (timestamp_line == "END")
      break;

    string event_count_line;
    if (!getline(cin, event_count_line))
      break;
    int e;
    {
      stringstream ss(event_count_line);
      if (!(ss >> e))
        break;
    }

    vector<string> events;
    events.reserve(e);
    for (int i = 0; i < e; ++i) {
      string event_line;
      if (!getline(cin, event_line))
        break;
      events.push_back(event_line);
    }
    // process events
    for (const string& ev : events) {
      if (ev.empty())
        continue;
      if (ev.substr(0, 3) == "ARR") {
        stringstream ss(ev);
        string dummy;
        int rid, lin;
        ss >> dummy >> rid >> lin;
        ensure_request(rid);
      } else if (ev.substr(0, 3) == "TDN") {
        // split into server, task_spec, dur (ignore dur)
        size_t first_space = ev.find(' ');
        size_t second_space = ev.find(' ', first_space + 1);
        if (second_space == string::npos)
          continue;
        string server = ev.substr(0, first_space);
        string task_spec = ev.substr(first_space + 1, second_space - first_space - 1);
        // dur = ev.substr(second_space + 1);
        handle_tdn(server, task_spec);
      } else if (ev.substr(0, 3) == "XDN") {
        stringstream ss(ev);
        string direction;
        int remote, size;
        string typ;
        int m;
        ss >> direction >> remote >> size >> typ >> m;
        vector<int> rids(m);
        for (int i = 0; i < m; ++i) {
          if (!(ss >> rids[i]))
            break;
        }
        handle_xdn(direction, remote, size, typ, m, rids);
      } else if (ev.substr(0, 3) == "FIN") {
        stringstream ss(ev);
        string dummy;
        int rid;
        ss >> dummy >> rid;
        handle_fin(rid);
      }
      // else ignore
    }
    // after processing events, try to schedule as many tasks as possible in this frame
    vector<string> assignments;
    while (try_schedule(assignments)) {
      // continue scheduling until no more can be scheduled
    }
    // output count and assignments
    cout << assignments.size() << '\n';
    for (const string& assign : assignments) {
      cout << assign << '\n';
    }
    cout << flush;
  }

  return 0;
}