/* Fibonacci heaps are somewhat complex, but, there's an article in
   DDJ that explains them pretty well:
   http://www.ddj.com/articles/1997/9701/9701o/9701o.htm?topic=algoritms
   Introduction to algorithms by Corman and Rivest also goes over them.
   The original paper that introduced them is "Fibonacci heaps and their
   uses in improved network optimization algorithms" by Tarjan and
   Fredman (JACM 34(3), July 1987).
   Amortized and real worst case time for operations:
   ExtractMin: O(lg n) amortized. O(n) worst case.
   DecreaseKey: O(1) amortized.  O(lg n) worst case.
   Insert: O(1) amortized.
   Union: O(1) amortized.  */

#include <bits/stdc++.h>
using namespace std;

#define GCC_FIBONACCI_HEAP_H
namespace {

/* Forward definition */

template <class K, class V> class fibonacci_heap;

template <class K, class V, typename Compare = less<Key>> class fibonacci_node {
  typedef fibonacci_node<K, V> fibonacci_node_t;
  friend class fibonacci_heap<K, V>;

public:
  fibonacci_node()
      : m_parent(NULL), m_child(NULL), m_left(this), m_right(this), m_data(NULL), m_degree(0),
        m_mark(0) {}

  fibonacci_node(K key, V* data = NULL)
      : m_parent(NULL), m_child(NULL), m_left(this), m_data(NULL), m_degree(0), m_mark(0) {}

  int compare(fibonacci_node_t* other) {
    if (m_key < other->m_key)
      return -1;
    if (m_key > other->m_key)
      return 1;
    return 0;
  }

  int compare_data(K key) {
    return fibonacci_node_t(key).compare(this);
  }

  fibonacci_node_t* remove();

  void link(fibonacci_node_t* parent);

  K get_key() {
    return m_key;
  }

  V* get_data() {
    return m_data;
  }

private:
  void insert_after(fibonacci_node_t* b);

  void insert_before(fibonacci_node_t* b) {
    m_left->insert_after(b);
  }

  /* Parent node */
  fibonacci_node* m_parent;
  /* Child node */
  fibonacci_node* m_child;
  /* Left sibling */
  fibonacci_node* m_left;
  /* Right sibling */
  fibonacci_node* m_right;
  /* Key associated with node */
  K m_key;
  /* Data associated with node */
  V* m_data;
};

template <class K, class V> class fibonacci_heap {
  typedef fibonacci_node<K, V> fibonacci_node_t;
  friend class fibonacci_node<K, V>;

public:
fibonacci_heap(K global_min_key, pool_allocator* allocator = NULL) : m_nodes(0), private:
}

} // namespace
