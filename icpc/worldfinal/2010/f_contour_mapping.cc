#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct V2 {
    double x, y;
};

static double dist2(const V2& a, const V2& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

struct Tri {
    int a, b, c;
};

static uint64_t edgeKey(int u, int v) {
    if (u > v) swap(u, v);
    return (uint64_t)(uint32_t)u << 32 | (uint32_t)v;
}

struct EdgeInfo {
    int t1_third; // third vertex height of first incident triangle (or -1)
    int t2_third; // third vertex height of second incident triangle (or -1)
    double len;
    EdgeInfo() : t1_third(-1), t2_third(-1), len(0) {}
};

static long long sumK(long long k1, long long k2) { return (k1 + k2) * (k2 - k1 + 1) / 2; }

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int s;
    int p;
    int d;
    int hInc;
    int caseNum = 1;

    while (cin >> s) {
        if (s == 0) break;
        cin >> p >> d >> hInc;

        vector<vector<int> > idx(s);
        int totalPts = 0;
        for (int i = 0; i < s; i++) {
            int len = (i % 2 == 0) ? p : (p + 1);
            idx[i].resize(len);
            for (int j = 0; j < len; j++) idx[i][j] = totalPts++;
        }

        vector<int> H(totalPts);
        for (int i = 0; i < s; i++) {
            int len = (i % 2 == 0) ? p : (p + 1);
            for (int j = 0; j < len; j++) cin >> H[idx[i][j]];
        }
        // Elevation data is followed by an empty line, but we read using operator>>
        // which naturally skips whitespace; no special handling needed.

        const double dy = (sqrt(3.0) / 2.0) * d;
        vector<V2> P(totalPts);
        for (int i = 0; i < s; i++) {
            int len = (i % 2 == 0) ? p : (p + 1);
            double y = i * dy;
            double x0 = (i % 2 == 0) ? (0.5 * d) : 0.0;
            for (int j = 0; j < len; j++) {
                int id = idx[i][j];
                P[id] = {x0 + j * (double)d, y};
            }
        }

        vector<Tri> tris;
        tris.reserve(s * p * 2);
        map<uint64_t, EdgeInfo> edgeMap;

        auto addEdge = [&](int u, int v, int thirdH) {
            uint64_t k = edgeKey(u, v);
            EdgeInfo& ei = edgeMap[k];
            if (ei.len == 0) ei.len = dist2(P[u], P[v]);
            if (ei.t1_third == -1) ei.t1_third = thirdH;
            else if (ei.t2_third == -1) ei.t2_third = thirdH;
        };

        for (int i = 0; i + 1 < s; i++) {
            const vector<int>& A = idx[i];
            const vector<int>& B = idx[i + 1];
            if ((int)A.size() == p && (int)B.size() == p + 1) {
                // A shorter (p), B longer (p+1)
                // triangles: (A[k], B[k], B[k+1]) for k=0..p-1
                for (int k = 0; k < p; k++) {
                    int t0 = A[k], t1 = B[k], t2 = B[k + 1];
                    tris.push_back({t0, t1, t2});
                    addEdge(t0, t1, H[t2]);
                    addEdge(t1, t2, H[t0]);
                    addEdge(t2, t0, H[t1]);
                }
                // triangles: (A[k], A[k+1], B[k+1]) for k=0..p-2
                for (int k = 0; k < p - 1; k++) {
                    int t0 = A[k], t1 = A[k + 1], t2 = B[k + 1];
                    tris.push_back({t0, t1, t2});
                    addEdge(t0, t1, H[t2]);
                    addEdge(t1, t2, H[t0]);
                    addEdge(t2, t0, H[t1]);
                }
            } else {
                // A longer (p+1), B shorter (p)
                // triangles: (B[k], A[k], A[k+1]) for k=0..p-1
                for (int k = 0; k < p; k++) {
                    int t0 = B[k], t1 = A[k], t2 = A[k + 1];
                    tris.push_back({t0, t1, t2});
                    addEdge(t0, t1, H[t2]);
                    addEdge(t1, t2, H[t0]);
                    addEdge(t2, t0, H[t1]);
                }
                // triangles: (B[k], B[k+1], A[k+1]) for k=0..p-2
                for (int k = 0; k < p - 1; k++) {
                    int t0 = B[k], t1 = B[k + 1], t2 = A[k + 1];
                    tris.push_back({t0, t1, t2});
                    addEdge(t0, t1, H[t2]);
                    addEdge(t1, t2, H[t0]);
                    addEdge(t2, t0, H[t1]);
                }
            }
        }

        double totalLen = 0.0;

        // Edge-on-contour contributions
        for (map<uint64_t, EdgeInfo>::const_iterator it = edgeMap.begin(); it != edgeMap.end(); ++it) {
            uint64_t k = it->first;
            int u = (int)(k >> 32);
            int v = (int)(k & 0xffffffffu);
            int hu = H[u], hv = H[v];
            if (hu != hv) continue;
            if (hu % hInc != 0) continue;
            const EdgeInfo& ei = it->second;
            // If the whole region around the edge is level at this height, do not draw it
            // (contours are shown only for plateau boundaries).
            bool flat1 = (ei.t1_third != -1 && ei.t1_third == hu);
            bool flat2 = (ei.t2_third != -1 && ei.t2_third == hu);
            if (flat1 && flat2) continue;
            totalLen += ei.len;
        }

        // Interior segment contributions per triangle, summed over contour levels (multiples of hInc).
        for (size_t ti = 0; ti < tris.size(); ti++) {
            int v0 = tris[ti].a, v1 = tris[ti].b, v2 = tris[ti].c;
            int a = v0, b = v1, c = v2;
            int ha = H[a], hb = H[b], hc = H[c];

            // Handle completely flat triangle: only boundaries are drawn; edges handled globally.
            if (ha == hb && hb == hc) continue;

            // Sort vertices by height: lo (L), mid (M), hi (H)
            int lo = a, mid = b, hi = c;
            if (H[lo] > H[mid]) swap(lo, mid);
            if (H[mid] > H[hi]) swap(mid, hi);
            if (H[lo] > H[mid]) swap(lo, mid);
            int h0 = H[lo], h1 = H[mid], h2 = H[hi];

            // No interior segments if contour levels don't fall strictly between min/max.
            if (h0 == h2) continue;

            if (h0 == h1 && h1 < h2) {
                // Two lows equal. For L in (h0,h2): segment is parallel to edge(lo,mid),
                // length(L) = (h2 - L)/(h2 - h0) * |lo-mid|.
                double baseLen = dist2(P[lo], P[mid]);
                long long k1 = (h0 / hInc) + 1;
                long long k2 = (h2 - 1) / hInc;
                if (k1 <= k2) {
                    double alpha = -baseLen / (double)(h2 - h0);
                    double beta = -alpha * (double)h2;
                    long long cnt = k2 - k1 + 1;
                    long long sumk = sumK(k1, k2);
                    totalLen += alpha * (double)hInc * (double)sumk + beta * (double)cnt;
                }
                continue;
            }

            if (h0 < h1 && h1 == h2) {
                // Two highs equal. For L in (h0,h2): segment parallel to edge(mid,hi),
                // length(L) = (L - h0)/(h2 - h0) * |mid-hi|.
                double baseLen = dist2(P[mid], P[hi]);
                long long k1 = (h0 / hInc) + 1;
                long long k2 = (h2 - 1) / hInc;
                if (k1 <= k2) {
                    double alpha = baseLen / (double)(h2 - h0);
                    double beta = -alpha * (double)h0;
                    long long cnt = k2 - k1 + 1;
                    long long sumk = sumK(k1, k2);
                    totalLen += alpha * (double)hInc * (double)sumk + beta * (double)cnt;
                }
                continue;
            }

            // All distinct: piecewise linear with peak length at level h1.
            // At L=h1, segment connects 'mid' to intersection on edge (lo,hi).
            {
                double t = (double)(h1 - h0) / (double)(h2 - h0);
                V2 pc = {P[lo].x + t * (P[hi].x - P[lo].x), P[lo].y + t * (P[hi].y - P[lo].y)};
                double Lmax = dist2(P[mid], pc);

                // (h0, h1): len = (L - h0)/(h1 - h0) * Lmax
                long long k1 = (h0 / hInc) + 1;
                long long k2 = (h1 - 1) / hInc;
                if (k1 <= k2) {
                    double alpha = Lmax / (double)(h1 - h0);
                    double beta = -alpha * (double)h0;
                    long long cnt = k2 - k1 + 1;
                    long long sumk = sumK(k1, k2);
                    totalLen += alpha * (double)hInc * (double)sumk + beta * (double)cnt;
                }

                // (h1, h2): len = (h2 - L)/(h2 - h1) * Lmax
                k1 = (h1 / hInc) + 1;
                k2 = (h2 - 1) / hInc;
                if (k1 <= k2) {
                    double alpha = -Lmax / (double)(h2 - h1);
                    double beta = -alpha * (double)h2;
                    long long cnt = k2 - k1 + 1;
                    long long sumk = sumK(k1, k2);
                    totalLen += alpha * (double)hInc * (double)sumk + beta * (double)cnt;
                }

                // If h1 itself is a contour level, include it once.
                if (h1 % hInc == 0) {
                    if (h0 < h1 && h1 < h2) totalLen += Lmax;
                }
            }
        }

        long long ans = (long long)floor(totalLen + 0.5);
        cout << "Case " << caseNum++ << ": " << ans << "\n";
    }
    return 0;
}

