// C++11
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <set>
#include <string>

using namespace std;

class SnakeCharmer {
public:
  vector<char> findSolution(int N, int V, string snake)
  {           
    char names[] = {'L','D','R','U'};  

    int n=N*N-1;
    vector<char> moves(n);
    
    for (int i=0,dir=0,L=1; i<n; )
    {
      for (int k=0; k<L && i<n; k++,i++) moves[i]=names[dir];
      dir=(dir+1)%4;
      for (int k=0; k<L && i<n; k++,i++) moves[i]=names[dir];
      dir=(dir+1)%4;
      L++;
    }
    
    return moves;
  }
};

int main() {
  SnakeCharmer prog;
  int N;
  int V;
  string snake;
  cin >> N;
  cin >> V;
  cin >> snake;
  
  vector<char> ret = prog.findSolution(N,V,snake);
  cout << ret.size() << endl;
  for (int i = 0; i < (int)ret.size(); ++i)
      cout << ret[i] << endl;
  cout.flush();
}