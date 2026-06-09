/*
 * Solution for CEOI 2011 Day 2: Teams.
 *
 * Sort children by minimum team size, then DP for the maximum number of consecutive
 * teams. Binary search the smallest feasible maximum team size.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

struct Child {
  int id;
  int requirement;
};

int child_count;
vector<Child> children;

vector<int> teams_count;
vector<int> last_valid;
vector<int> previous_team_end;

int max_teams_with_limit(int limit) {
  teams_count.assign(child_count + 1, -1);
  last_valid.assign(child_count + 1, 0);
  previous_team_end.assign(child_count + 1, -1);

  teams_count[0] = 0;
  last_valid[0] = 0;
  for (int i = 1; i <= child_count; i++) {
    int requirement = children[i - 1].requirement;
    int start = max(0, i - limit);
    int best_index = -1;
    if (start <= i - requirement) {
      best_index = last_valid[i - requirement];
    }
    if (best_index >= 0) {
      teams_count[i] = teams_count[best_index] + 1;
      previous_team_end[i] = best_index;
    }
    if (teams_count[i] >= 0) {
      if (teams_count[i] < teams_count[last_valid[i - 1]]) {
        teams_count[i] = -1;
      } else {
        last_valid[i] = i;
      }
    } else {
      last_valid[i] = last_valid[i - 1];
    }
  }
  return teams_count[child_count];
}

void build_teams(int limit, int team_total, vector<vector<int>>& teams) {
  teams.assign(team_total, {});
  int index = child_count;
  for (int team_id = team_total - 1; team_id >= 0; team_id--) {
    int start = previous_team_end[index];
    for (int child_index = start + 1; child_index <= index; child_index++) {
      teams[team_id].push_back(children[child_index - 1].id);
    }
    index = start;
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> child_count;
  children.resize(child_count);
  for (int i = 0; i < child_count; i++) {
    children[i] = {i + 1, 0};
    cin >> children[i].requirement;
  }
  sort(children.begin(), children.end(),
       [](const Child& lhs, const Child& rhs) { return lhs.requirement < rhs.requirement; });

  int max_teams = max_teams_with_limit(child_count);
  int low = 1;
  int high = child_count;
  while (low < high) {
    int mid = (low + high) / 2;
    if (max_teams_with_limit(mid) == max_teams) {
      high = mid;
    } else {
      low = mid + 1;
    }
  }

  max_teams_with_limit(low);
  vector<vector<int>> teams;
  build_teams(low, max_teams, teams);

  cout << max_teams << '\n';
  for (const vector<int>& team : teams) {
    cout << team.size();
    for (int child_id : team) {
      cout << ' ' << child_id;
    }
    cout << '\n';
  }
  return 0;
}
