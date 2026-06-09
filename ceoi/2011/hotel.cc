/*
 * Solution for CEOI 2011 Day 2: Hotel.
 *
 * Greedily pair offers (sorted by price) with the cheapest available room that
 * fits, then take the o most profitable pairs.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

struct Room {
  int capacity;
  int upkeep;
};

struct Offer {
  int price;
  int min_capacity;
  int index;
};

struct Pairing {
  int profit;
  int offer_index;
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m, o;
  cin >> n >> m >> o;
  set<pair<int, int>> rooms;
  for (int i = 0; i < n; i++) {
    int upkeep, capacity;
    cin >> upkeep >> capacity;
    rooms.insert({capacity, upkeep});
  }

  vector<Offer> offers(m);
  for (int i = 0; i < m; i++) {
    cin >> offers[i].price >> offers[i].min_capacity;
    offers[i].index = i;
  }
  sort(offers.begin(), offers.end(),
       [](const Offer& lhs, const Offer& rhs) { return lhs.price > rhs.price; });

  vector<Pairing> pairings;
  pairings.reserve(m);
  for (const Offer& offer : offers) {
    auto it = rooms.lower_bound({offer.min_capacity, 0});
    if (it == rooms.end()) {
      continue;
    }
    pairings.push_back({offer.price - it->second, offer.index});
    rooms.erase(it);
  }

  sort(pairings.begin(), pairings.end(),
       [](const Pairing& lhs, const Pairing& rhs) { return lhs.profit > rhs.profit; });

  long long profit = 0;
  for (int i = 0; i < min(o, static_cast<int>(pairings.size())); i++) {
    if (pairings[i].profit > 0) {
      profit += pairings[i].profit;
    }
  }

  cout << profit << '\n';
  return 0;
}
