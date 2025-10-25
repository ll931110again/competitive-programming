#include <bits/stdc++.h>
#define maxn 150005
#define BUCKET_SIZE 400
using namespace std;
 
int N, L;
int initial[maxn], X[maxn];
int query_counter = 0;
 
struct Bucket {
    vector<int> elements;
    vector<int> steps_needed;
    vector<int> end_cover;
 
    void compute() {
        if (elements.empty()) {
            return;
        }
 
        steps_needed.resize(elements.size());
        end_cover.resize(elements.size());
 
        int last_value = elements.back();
 
        int j = elements.size() - 1;
        for (int i = elements.size() - 1; i >= 0; i--) {
            if (elements[i] + L + 1 > last_value) {
                steps_needed[i] = 1;
                end_cover[i] = elements[i] + L + 1;
            } else {
                while (elements[j] >= elements[i] + L + 1) {
                    j--;
                }
                j++;
                steps_needed[i] = 1 + steps_needed[j];
                end_cover[i] = end_cover[j];
            }
        }
    }
 
    pair<int,int> query(int pos) {
        int low = 0, high = elements.size() - 1, ans = 0;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (elements[mid] >= pos) {
                ans = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
 
        return make_pair(steps_needed[ans], end_cover[ans]);
    }
 
    void remove(int value) {
        int low = 0, high = elements.size() - 1, pos = 0;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (elements[mid] <= value) {
                pos = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
 
        for (int j = pos; j + 1 < elements.size(); j++) {
            elements[j] = elements[j + 1];
        }
 
        elements.pop_back();
        compute();
    }
 
    void add(int value) {
        int low = 0, high = elements.size() - 1, ins_pos = 0;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (elements[mid] <= value) {
                ins_pos = mid + 1;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
 
        if (elements.back() <= value) {
            ins_pos = elements.size();
        }
 
        elements.push_back(0);
        for (int i = elements.size() - 1; i > ins_pos; i--) {
            elements[i] = elements[i - 1];
        }
        elements[ins_pos] = value;
        compute();
    }
 
    void init(vector<int> values) {
        elements = values;
        compute();
    }
};
 
Bucket buckets[BUCKET_SIZE];
int num_buckets;
bool filled = false;
 
void build_init() {
    if (filled) {
        int pos = 0;
        for (int i = 0; i < num_buckets; i++) {
            for (auto x : buckets[i].elements) {
                X[pos++] = x;
            }
        }        
    } else {
        for (int i = 0; i < N; i++) {
            X[i] = initial[i];
        }
        filled = true;
    }
 
    num_buckets = 0;
    for (int i = 0; i < N; i += BUCKET_SIZE) {
        vector<int> values;
        for (int j = i; j < N && j - i < BUCKET_SIZE; j++) {
            values.push_back(X[j]);
        }
        buckets[num_buckets++].init(values);
    }
}
 
void init(int _n, int _l, int _x[]) {
    N = _n;
    L = _l;
    for (int i = 0; i < N; i++) {
        initial[i] = _x[i];
    }
 
    build_init();
}
 
int update(int idx, int new_value) {
    query_counter++;
    // Rebalance the buckets if they become too unbalanced
    if (query_counter >= BUCKET_SIZE) {
        build_init();
        query_counter = 0;
    }
 
    int old_value = initial[idx];
    initial[idx] = new_value;
 
    // delete the old value from the bucket
    for (int i = 0; i < num_buckets; i++) {
        if (buckets[i].elements.front() <= old_value && old_value <= buckets[i].elements.back()) {
            buckets[i].remove(old_value);
 
            // delete empty buckets
            if (buckets[i].elements.empty()) {
                for (int j = i; j + 1 < num_buckets; j++) {
                    buckets[j].elements = buckets[j + 1].elements;
                    buckets[j].steps_needed = buckets[j + 1].steps_needed;
                    buckets[j].end_cover = buckets[j + 1].end_cover;
                }
                num_buckets--;
            }
            break;
        }
    }
 
    // insert the new value to the right bucket
    for (int i = 0; i < num_buckets; i++) {
        if (i == 0 && new_value <= buckets[i].elements.front()) {
            buckets[i].add(new_value);
            break;
        }
 
        if (buckets[i].elements.empty()) {
            buckets[i].add(new_value);
            break;
        }
 
        if (i == num_buckets - 1) {
            buckets[i].add(new_value);
            break;
        }
 
        if (buckets[i].elements.front() <= new_value && new_value <= buckets[i + 1].elements.front()) {
            buckets[i].add(new_value);
            break;
        }
    }
 
    // compute the answer
    int steps = 0, end_cover = 0, bucket_idx = 0;
    while (bucket_idx < num_buckets) {
        auto q = buckets[bucket_idx].query(end_cover);
        steps += q.first;
        end_cover = q.second;
        while (bucket_idx < num_buckets && buckets[bucket_idx].elements.back() < end_cover) {
            bucket_idx++;
        }
    }
 
    return steps;
}
