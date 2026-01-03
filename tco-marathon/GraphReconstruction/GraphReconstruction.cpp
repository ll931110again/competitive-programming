// C++11
#include <algorithm>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <map>
#include <sstream>
#include <vector>
#include <set>
#include <string>
#include <random>
#include <vector>
#define MAXE 25
using namespace std;

struct Path {
  int from;
  int to;
  int len;  // -1 incidates there's no path

  Path(int from_, int to_, int len_) : from(from_), to(to_), len(len_) {}

  Path(string& input) {
    stringstream ss;
    ss << input;
    ss >> from >> to >> len;
  }
};

class ConnectedComponent {
public:
  ConnectedComponent(int N, vector<Path> paths)
      : N_(N), paths_(paths) {
    edges_.clear();
    edges_.resize(N_);
    for (int i = 0; i < edges_.size(); i++) {
      edges_[i].resize(N_);
      for (int j = 0; j < N_; j++) {
        edges_[i][j] = (i == j) ? 0 : -1;
      }
    }
    adj_.clear();
    adj_.resize(N_);

    for (auto path : paths_) {
      adj_[path.from].push_back(make_pair(path.to, path.len));
      adj_[path.to].push_back(make_pair(path.from, path.len));
      
      if (path.len == 1) {
        edges_[path.from][path.to] = 1;
        edges_[path.to][path.from] = 1;
      } else if (path.len == -1 || path.len > 1) {
        edges_[path.from][path.to] = 0;
        edges_[path.to][path.from] = 0;
      }
    }

    for (int i = 0; i < N_; i++) {
      for (auto& p : adj_[i]) {
        for (auto& q : adj_[i]) {
          // If length difference is > 1, there's no edge between u and v
          if (abs(p.second - q.second) > 1) {
            int u = p.first, v = p.first;
            edges_[u][v] = edges_[v][u] = 0;
          }
        }
      }
    }

    // If there's only one edge left, assign it to be 1
    for (int i = 0; i < N_; i++) {
      int oneCount = 0, zeroCount = 0;
      for (int j = 0; j < N_; j++) if (edges_[i][j] == 1) {
        oneCount++;
      } else if (edges_[i][j] == 0) {
        zeroCount++;
      }

      if (oneCount == 0 && zeroCount == N - 2) {
        for (int j = 0; j < N_; j++) if (edges_[i][j] < 0) {
          edges_[i][j] = edges_[j][i] = 1;
        }
      }
    }
  }

  vector< vector<int> > findEdges() {
    int edgeCount = 0;
    for (int i = 0; i < N_; i++) {
      for (int j = i + 1; j < N_; j++) if (edges_[i][j] < 0) {
        edgeCount++;
      }
    }

    if (edgeCount > MAXE) {
      // exceeded number of unknown edges, simply assign 0 for all remaining edges
      for (int i = 0; i < N_; i++) {
        for (int j = 0; j < N_; j++) if (edges_[i][j] < 0) {
          edges_[i][j] = 0;
        }
      }
      return edges_;
    }

    int weight = N_;
    auto optimalEdges = edges_;

    for (int mask = 0; mask < (1 << edgeCount); mask++) {
      int wc = 0;
      for (int i = 0; i < edgeCount; i++) if (mask & (1 << i)) {
        wc++;
      }
      if (wc >= weight) {
        continue;
      }

      auto myEdges = edges_;
      int k = 0;
      for (int i = 0; i < N_; i++) {
        for (int j = i + 1; j < N_; j++) if (myEdges[i][j] < 0) {
          if (mask & (1 << k)) {
            myEdges[i][j] = myEdges[j][i] = 1;
          } else {
            myEdges[i][j] = myEdges[j][i] = 0;
          }
          k++;
        }
      }

      if (check(myEdges)) {
        weight = wc;
        optimalEdges = myEdges;
      }
    }

    return optimalEdges;
  }

private:
  int N_;
  vector<Path> paths_;
  vector<vector<int>> edges_;
  vector< vector<pair<int,int> >> adj_;

  bool check(vector<vector<int>>& adjGraph) {
    vector<vector<int>> dist(N_);
    for (int i = 0; i < N_; i++) {
      dist[i].resize(N_);
      for (int j = 0; j < N_; j++) {
        if (i == j) {
          dist[i][j] = 0;
        } else if (adjGraph[i][j] > 0) {
          dist[i][j] = 1;
        } else {
          dist[i][j] = N_;
        }
      }
    }

    for (int k = 0; k < N_; k++) {
      for (int i = 0; i < N_; i++) {
        for (int j = 0; j < N_; j++) {
          dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
        }
      }
    }

    for (auto& path : paths_) {
      if (path.len && N_ && dist[path.from][path.to] != path.len) {
        return false;
      }
    }

    return true;
  }
};

class GraphReconstruction {
public:
  vector<string> findSolution(int N, double C, int K, vector<string> paths)
  {           
    vector<Path> myPaths;
    for (auto path : paths) {
      myPaths.push_back(Path(path));
    }

    vector< vector<int> > adj;
    adj.resize(N);
    for (int i = 0; i < N; i++) {
      adj[i].resize(N);
      for (int j = 0; j < N; j++) {
        adj[i][j] = (i == j) ? 0 : -1;
      }
    }

    for (auto path : myPaths) {
      if (path.len == 1) {
        adj[path.from][path.to] = 1;
        adj[path.to][path.from] = 1;
      } else if (path.len == -1 || path.len > 1) {
        adj[path.from][path.to] = 0;
        adj[path.to][path.from] = 0;
      }
    }

    vector<int> vis(N), pos(N);
    for (int i = 0; i < N; i++) {
      vis[i] = -1;
    }

    int ccount = 0;
    for (int i = 0; i < N; i++) if (vis[i] < 0) {
      vector<int> nodes;
      queue<int> q;
      
      nodes.push_back(i);
      vis[i] = ccount;
      q.push(i);

      while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (auto& path : myPaths) {
          if (path.len < 0) {
            continue;
          }

          if (path.from == u || path.to == u) {
            int v = (path.from == u) ? path.to : path.from;
            if (vis[v] < 0) {
              vis[v] = ccount;
              q.push(v);
              nodes.push_back(v);
            }
          }
        }
      }

      sort(nodes.begin(), nodes.end());
      for (int i = 0; i < nodes.size(); i++) {
        pos[nodes[i]] = i;
      }

      vector<Path> cpaths;
      for (auto& path : myPaths) {
        if (path.len < 0) {
          continue;
        }

        if (vis[path.from] != ccount || vis[path.to] != ccount) {
          continue;
        }

        cpaths.push_back(Path(pos[path.from], pos[path.to], path.len));
      }

      auto component = ConnectedComponent((int) nodes.size(), cpaths);
      auto subgraph = component.findEdges();

      for (int i = 0; i < subgraph.size(); i++) {
        for (int j = 0; j < subgraph[i].size(); j++) {
          adj[nodes[i]][nodes[j]] = subgraph[i][j];
        }
      }

      ccount++;
    }

    // Assign everything else to be 0 (since they are on different components)
    for (int i = 0; i < N; i++) {
      for (int j = i + 1; j < N; j++) if (adj[i][j] < 0) {
        adj[i][j] = adj[j][i] = 0;
      }
    }

    vector<string> out;

    for (int i=0; i<N; i++)
    {
      string row;
      for (int j = 0; j < N; j++) if (adj[i][j] > 0) {
        row += '1';
      } else {
        row += '0';
      }
      out.push_back(row);
    }

    return out;
  }
};

int main() {
  GraphReconstruction prog;
  int N;
  double C;
  int K;
  int NumPaths;
  vector<string> paths;
  string path;
  cin >> N;
  cin >> C;
  cin >> K;
  cin >> NumPaths;

  // skip first empty line
  getline(std::cin, path);
  for (int i=0; i<NumPaths; i++)
  {
    getline(std::cin, path);
    paths.push_back(path);
  }
  
  vector<string> ret = prog.findSolution(N,C,K,paths);
  cout << ret.size() << endl;
  for (int i = 0; i < (int)ret.size(); ++i)
      cout << ret[i] << endl;
  cout.flush();
}