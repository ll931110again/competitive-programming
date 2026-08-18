#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <string>
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
  ss >> K >> S >> latency_in_ms >> bandwidth_gbps >> bytes_per_token >> num_layers;

  // second line: scoring parameters (ignore)
  if (!getline(cin, line))
    return 0;

  // third line: N
  if (!getline(cin, line))
    return 0;
  int N = stoi(line);

  // read N rows of task-time table (ignore)
  for (int i = 0; i < N; ++i) {
    if (!getline(cin, line))
      break;
  }

  bool local_free = true;
  vector<bool> remote_free(K, true);
  map<int, RequestState> requests; // rid -> RequestState

  auto handle_tdn = [&](const string& server, const string& task_spec) {
    // Free the resource that completed the task
    if (server == "E") {
      local_free = true;
    } else if (server.size() > 1 && server[0] == 'C') {
      // server format: C<number>
      int k = stoi(server.substr(1));
      if (k >= 0 && k < K) {
        remote_free[k] = true;
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
        ss >> remote >> rid;
        auto it = requests.find(rid);
        if (it != requests.end() && !it->second.finished) {
          it->second.waiting_up = true;
          it->second.next_stage_after_up = 1; // after UP, can do P_PROC
        }
      } else if (second == "PROC") {
        // ls le remote rid
        int ls, le, remote, rid;
        ss >> ls >> le >> remote >> rid;
        auto it = requests.find(rid);
        if (it != requests.end() && !it->second.finished) {
          it->second.waiting_down = true;
          it->second.next_stage_after_down = 2; // after DOWN, can do P_POST
        }
      } else if (second == "POST") {
        int remote, rid;
        ss >> remote >> rid;
        auto it = requests.find(rid);
        if (it != requests.end() && !it->second.finished) {
          it->second.stage = 3; // ready for D_PRE
          it->second.waiting_up = it->second.waiting_down = false;
          it->second.next_stage_after_up = it->second.next_stage_after_down = -1;
        }
      }
    } else if (first == "D") {
      string second;
      ss >> second;
      if (second == "PRE") {
        int dummy, m;
        ss >> dummy >> m; // dummy is -1
        vector<int> rids(m);
        for (int i = 0; i < m; ++i)
          ss >> rids[i];
        for (int rid : rids) {
          auto it = requests.find(rid);
          if (it != requests.end() && !it->second.finished) {
            it->second.waiting_up = true;
            it->second.next_stage_after_up = 4; // after UP, can do D_PROC
          }
        }
      } else if (second == "PROC") {
        int remote, m;
        ss >> remote >> m;
        vector<int> rids(m);
        for (int i = 0; i < m; ++i)
          ss >> rids[i];
        for (int rid : rids) {
          auto it = requests.find(rid);
          if (it != requests.end() && !it->second.finished) {
            it->second.waiting_down = true;
            it->second.next_stage_after_down = 5; // after DOWN, can do D_POST
          }
        }
      } else if (second == "POST") {
        int dummy, m;
        ss >> dummy >> m; // dummy is -1
        vector<int> rids(m);
        for (int i = 0; i < m; ++i)
          ss >> rids[i];
        for (int rid : rids) {
          auto it = requests.find(rid);
          if (it != requests.end() && !it->second.finished) {
            it->second.tokens_produced += 1;
            // ready for another D_PRE (assuming more tokens)
            it->second.stage = 3;
            it->second.waiting_up = it->second.waiting_down = false;
            it->second.next_stage_after_up = it->second.next_stage_after_down = -1;
          }
        }
      }
    }
  };

  auto handle_xdn = [&](const string& direction, int remote, int size, const string& typ, int m,
                        const vector<int>& rids) {
    for (int rid : rids) {
      auto it = requests.find(rid);
      if (it == requests.end() || it->second.finished)
        continue;
      if (typ == "PRE") {
        if (direction == "UP") {
          if (it->second.waiting_up) {
            it->second.waiting_up = false;
            it->second.stage = it->second.next_stage_after_up;
            it->second.next_stage_after_up = it->second.next_stage_after_down = -1;
          }
        } else { // DOWN
          if (it->second.waiting_down) {
            it->second.waiting_down = false;
            it->second.stage = it->second.next_stage_after_down;
            it->second.next_stage_after_up = it->second.next_stage_after_down = -1;
          }
        }
      } else { // DEC
        if (direction == "UP") {
          if (it->second.waiting_up) {
            it->second.waiting_up = false;
            it->second.stage = it->second.next_stage_after_up;
            it->second.next_stage_after_up = it->second.next_stage_after_down = -1;
          }
        } else { // DOWN
          if (it->second.waiting_down) {
            it->second.waiting_down = false;
            it->second.stage = it->second.next_stage_after_down;
            it->second.next_stage_after_up = it->second.next_stage_after_down = -1;
          }
        }
      }
    }
  };

  auto handle_fin = [&](int rid) {
    auto it = requests.find(rid);
    if (it != requests.end()) {
      it->second.finished = true;
    }
  };

  // function to try schedule one task, returns true if scheduled and pushes assignment to out
  auto try_schedule = [&](vector<string>& out) -> bool {
    // local computer
    if (local_free) {
      for (auto& kv : requests) {
        RequestState& req = kv.second;
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
        for (auto& kv : requests) {
          RequestState& req = kv.second;
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
        if (requests.find(rid) == requests.end()) {
          requests.emplace(rid, RequestState(rid, K));
        }
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
        for (int i = 0; i < m; ++i)
          ss >> rids[i];
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