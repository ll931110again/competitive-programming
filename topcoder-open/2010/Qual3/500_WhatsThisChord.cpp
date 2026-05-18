#include <bits/stdc++.h>
using namespace std;

// TCO 2010 Qual 3 Div I 500 - WhatsThisChord
// https://archive.topcoder.com/ProblemDetail/rd/14278/pm/10927
//
// Map frets to pitch classes; match the played set against the 24
// major/minor triads (C major/minor shifted by root).

class WhatsThisChord {
public:
  string classify(vector<int> chord) {
    static const int tuning[] = {4, 9, 2, 7, 11, 4}; // E A D G B E
    static const string names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

    vector<bool> played(12);
    int distinct = 0;
    for (int i = 0; i < 6; i++) {
      if (chord[i] < 0)
        continue;
      int note = (tuning[i] + chord[i]) % 12;
      if (!played[note]) {
        played[note] = true;
        ++distinct;
      }
    }
    if (distinct != 3)
      return "";

    for (int root = 0; root < 12; root++) {
      int major[] = {root, (root + 4) % 12, (root + 7) % 12};
      if (matches(played, major))
        return names[root] + " Major";
      int minor[] = {root, (root + 3) % 12, (root + 7) % 12};
      if (matches(played, minor))
        return names[root] + " Minor";
    }
    return "";
  }

private:
  static bool matches(const vector<bool>& played, const int triad[3]) {
    for (int i = 0; i < 3; i++)
      if (!played[triad[i]])
        return false;
    return true;
  }
};
