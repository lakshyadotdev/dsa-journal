# Arrays & Hashing: Theory Chapter

---

## 1. Prerequisites

### Must Know

**C++ basics**
- Variables, loops (`for`, `while`), `if/else`, functions, return values
- `int`, `long long`, `char`, `bool`, `string`
- Integer division and `%` (remainder), including that `-7 % 3` is negative in C++
- Passing by value vs. by reference (`vector<int> a` vs. `vector<int>& a`)

**`vector`**
- `vector<int> v;`, `vector<int> v(n, 0);`, `vector<int> v = {1, 2, 3};`
- `vector<vector<int>> grid(m, vector<int>(n, 0));`
- `v[i]`, `v.size()`, `v.push_back(x)`, `v.back()`, `v.empty()`
- `v.begin()` / `v.end()`, `v.erase(it)`
- Range-based `for`: `for (int x : v)` and `for (auto& x : v)`

**`string`**
- `s.size()`, `s[i]`, `s += "abc"`, `s.substr(pos, len)`, `s.find(ch, pos)`
- `to_string(int)` and `stoi(string)`
- Characters as small integers: `'c' - 'a'` gives a number from 0 to 25

**`pair` and `swap`**
- `pair<int,int> p = {1, 2};`, `p.first`, `p.second`
- `swap(a, b)`

**`unordered_map<K, V>`**
- Declaration, `mp[key]`, `mp.count(key)`, `mp.find(key)` compared with `mp.end()`, `mp.erase(key)`, `mp.size()`
- Iterating with `for (auto& [key, value] : mp)` (C++17)

**`unordered_set<T>`**
- Declaration, `insert()`, `count()`, `find()`, `erase()`, `size()`

**`sort`**
- `sort(v.begin(), v.end());`
- Sorting with a custom comparator lambda

**Concepts**
- Big-O notation for time and space: O(1), O(log n), O(n), O(n log n), O(n²)
- Recursion (base case, recursive case); needed for merge sort
- Integer overflow: `int` holds up to about 2.1 × 10⁹

### Useful but Not Required
- Lambdas and structured bindings
- `priority_queue` basics (the chapter introduces what it needs)
- `std::array`, `std::bitset`, `std::list`
- Iterators beyond `begin()`/`end()`
- Stable vs. unstable sorting (introduced below)

---

## 2. Core Concepts

### 2.1 Arrays and `vector`

An **array** stores elements in **contiguous memory**. Because every element has the same size, the address of element `i` is computed directly from the start address, so indexing is O(1).

```
index:   0    1    2    3    4
       [ 7 ][ 2 ][ 9 ][ 4 ][ 1 ]
```

`vector<T>` is a resizable array. Its main costs:

| Operation | Cost |
|---|---|
| `v[i]` read/write | O(1) |
| `push_back` | amortized O(1) |
| `pop_back` | O(1) |
| insert/erase at index `i` | O(n), because later elements shift |
| search for a value (unsorted) | O(n) |

Two ideas appear throughout this topic:

- **In-place** means the result is produced by rearranging or overwriting the input itself, using only O(1) extra memory.
- **Extra space** means building a new structure (another vector, a map) whose size grows with the input. This is usually a trade of memory for speed.

**Index ranges.** Many array tasks are cleaner with **half-open ranges** `[lo, hi)`, which include `lo` and exclude `hi`. The length is `hi - lo`, and adjacent ranges `[a, b)` and `[b, c)` do not overlap.

### 2.2 Strings as Arrays of Characters

A `string` behaves like a `vector<char>` with extra features. Characters are stored as small integers, so `'d' - 'a' == 3`. When the alphabet is small and fixed (for example, lowercase letters), you can use an array of size 26 indexed by `c - 'a'`.

### 2.3 Direct-Address Tables

If every possible key is a small integer in a known range, you can skip hashing entirely and use an array where the key is the index:

```cpp
int count[26] = {};          // key: letter 0..25, value: how many times seen
for (char c : s) count[c - 'a']++;
```

This gives true O(1) lookup with no collisions, but it only works when the key range is small. Hashing is the general version of this idea.

### 2.4 Hashing

**Goal:** store and retrieve `key → value` associations (or just a set of keys) in about O(1) time, even when keys are huge numbers or strings.

**How it works:**

1. A **hash function** `h(key)` converts a key into an integer.
2. That integer is reduced to a **bucket index**, e.g. `h(key) % numBuckets`.
3. The key (and value) is stored in that bucket.

```
key "cat" ──h──► 83715 ──% 8──► bucket 3
key "dog" ──h──► 12006 ──% 8──► bucket 6
key "act" ──h──► 55211 ──% 8──► bucket 3   ← collision with "cat"
```

**Collisions.** Two different keys can land in the same bucket, since there are far more possible keys than buckets. The common strategy is **chaining**: each bucket holds a small list of entries, and a lookup scans only that list.

**Load factor** is `entries / buckets`. If it grows too large, chains get long. Hash tables therefore **resize** (add buckets and redistribute entries) to keep chains short.

**Practical complexity:**

| Operation | Average | Worst case |
|---|---|---|
| insert / find / erase | O(1) | O(n) (all keys in one bucket) |

With a good hash function and resizing, the worst case is rare. You can treat `unordered_map` and `unordered_set` operations as O(1) on average.

**Requirements on keys.** A key type needs an **equality test** and a **hash function**. In C++, `int`, `long long`, `char`, `bool`, and `string` work out of the box. `pair<int,int>` and `vector<int>` do **not** work as keys in `unordered_map`/`unordered_set` without writing a custom hash.

**Ordered alternatives.** `map` and `set` keep keys sorted using a balanced tree: O(log n) per operation, with ordered iteration and any key type that supports `<`, including `pair` and `vector`.

| | `unordered_map/set` | `map/set` |
|---|---|---|
| Lookup | O(1) average | O(log n) |
| Iteration order | unspecified | sorted |
| Key types | need hash | need `<` |

**Maps vs. sets.** A *set* answers "is this key present?". A *map* answers "what value is associated with this key?". A set is a map without values.

### 2.5 Building a Hash Table Yourself (Chaining)

Some tasks ask you to build the structure rather than use the library. The minimal design:

- A fixed number of buckets, each a small container of entries
- A function mapping a key to a bucket index
- Each operation (insert, find, erase) computes the bucket index, then searches/modifies only that bucket

```cpp
const int B = 1009;                         // number of buckets (a prime is a common choice)
vector<vector<pair<int,int>>> table(B);     // each bucket: list of (key, value)

int bucketOf(int key) { return key % B; }   // assumes key >= 0

// Searching one bucket:
auto& bucket = table[bucketOf(key)];
for (auto& [k, v] : bucket) {
    if (k == key) { /* found: v is the stored value */ }
}
```

Each operation is O(chain length), which is O(1) on average when entries spread evenly. A hash set is the same structure storing keys only. Resizing exists to protect performance for very large inputs; at this level, understanding buckets, the index function, and collisions is the core.

### 2.6 Sorting Fundamentals

**Sorting** rearranges elements into a defined order. Key terms:

- **Comparison sort:** decides order only by comparing pairs of elements. The best possible worst case is O(n log n).
- **Stable:** equal elements keep their original relative order.
- **In-place:** uses O(1) extra memory (apart from recursion).

**Merge sort** (stable, O(n log n) always, O(n) extra space):

1. Split the range into two halves.
2. Recursively sort each half.
3. **Merge** the two sorted halves by repeatedly taking the smaller front element.

```cpp
void mergeSort(vector<int>& a, vector<int>& tmp, int lo, int hi) { // sorts [lo, hi)
    if (hi - lo <= 1) return;
    int mid = lo + (hi - lo) / 2;
    mergeSort(a, tmp, lo, mid);
    mergeSort(a, tmp, mid, hi);
    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi) tmp[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];
    while (i < mid) tmp[k++] = a[i++];
    while (j < hi)  tmp[k++] = a[j++];
    for (k = lo; k < hi; ++k) a[k] = tmp[k];
}
```

The recursion has about log₂ n levels, and each level does O(n) merging work, hence O(n log n).

**Quicksort** (idea): choose a **pivot**, **partition** the range into elements smaller than, equal to, and larger than the pivot (see §3.1), then recursively sort the "smaller" and "larger" parts. Average O(n log n); worst case O(n²) with consistently bad pivots (a random pivot makes this very unlikely). It is in-place.

**Heap-based sorting** relies on a **heap** (also called a priority queue): a structure giving O(log n) insertion and O(log n) removal of the largest (or smallest) element, with O(1) access to it. Repeatedly extracting from a heap yields sorted order in O(n log n) with O(1) extra space for the in-place version. In C++ you usually use `priority_queue` rather than writing a heap.

**Counting sort** (non-comparison): when values are small integers in a known range `[0, R)`, count how many times each value occurs, then write values back in order. O(n + R) time, O(R) space. It beats O(n log n) only when `R` is small relative to the input.

**Library sorting:**

```cpp
sort(v.begin(), v.end());                              // ascending
sort(v.begin(), v.end(), greater<int>());              // descending
sort(v.begin(), v.end(), [](int a, int b) { return a > b; });  // custom comparator
```

`std::sort` is O(n log n) and not stable (`stable_sort` is). A comparator must define a **strict** ordering: `<` or `>`, never `<=` or `>=`, otherwise behavior is undefined.

### 2.7 Prefix Sums

A **prefix sum array** stores running totals so that any contiguous range sum can be computed in O(1) after O(n) preprocessing.

For array `a` of length `n`, define `pre` of length `n + 1`:

```
pre[0] = 0
pre[i + 1] = pre[i] + a[i]          // pre[i] = sum of a[0..i-1]
```

Then the sum of `a[l..r]` (inclusive) is:

```
sum(l, r) = pre[r + 1] - pre[l]
```

```
a   :   3   1   4   1   5
pre : 0   3   4   8   9   14
sum(1,3) = pre[4] - pre[1] = 9 - 3 = 6   (1 + 4 + 1)
```

The `+1` shift and the leading `0` avoid special cases for ranges starting at index 0. Section 3.5 extends this idea to 2D and to other operations.

---

## 3. Patterns & General Techniques

### 3.1 In-Place Rearrangement (Read/Write Indices and Partitioning)

**What it is.** Reorganizing an array within itself using a small number of index variables that sweep through the array.

**Read/write index (compaction).** One index (`r`) reads every element. A second index (`w`) marks where the next accepted element should be written. The prefix `a[0..w)` always holds the accepted elements so far.

```cpp
int w = 0;
for (int r = 0; r < (int)a.size(); ++r) {
    if (a[r] >= 0) {          // some condition on a[r]
        a[w++] = a[r];
    }
}
// a[0..w) now holds the accepted elements, in original order
```

`w <= r` always holds, so a write never overwrites an unread element.

**Partitioning.** Divide the array into regions by comparison with a pivot value, maintaining an invariant for each region. The three-way version uses three indices:

```
[ < pivot ][ == pivot ][ unknown ][ > pivot ]
0        lt          i          gt        n-1
```

```cpp
int lt = 0, i = 0, gt = (int)a.size() - 1;
while (i <= gt) {
    if (a[i] < pivot)      swap(a[lt++], a[i++]);
    else if (a[i] > pivot) swap(a[i], a[gt--]);   // do NOT advance i: swapped-in element is unexamined
    else                   ++i;
}
```

**Key idea.** Every step shrinks the "unknown" region while preserving the invariants of the known regions.

**Complexity.** O(n) time, O(1) extra space; one pass.

### 3.2 Single-Pass Scanning with Running State

**What it is.** Walking through the data once while maintaining a few variables that summarize everything seen so far (a running minimum, a running maximum, a running count, a running total).

```cpp
int best = a[0];
for (int i = 1; i < (int)a.size(); ++i) {
    best = max(best, a[i]);           // state depends only on what has been scanned
}
```

Often the state is updated by comparing each element with its **neighbor** or with the stored summary. The art is deciding what the minimal useful state is.

**Candidate-elimination (Boyer–Moore voting).** A specific, widely useful example: maintain a **candidate** and a **counter**.

- If the counter is 0, adopt the current element as the candidate.
- If the current element equals the candidate, increment the counter; otherwise decrement it.

```cpp
int candidate = 0, cnt = 0;
for (int x : a) {
    if (cnt == 0) candidate = x;
    cnt += (x == candidate) ? 1 : -1;
}
```

*Why it works:* each decrement cancels one occurrence of the candidate against one different element. A value occurring **more than half** the time cannot be fully canceled, so it must remain as the candidate. The result is a *possible* candidate; a second counting pass verifies it. The idea generalizes: to find values occurring more than `n/k` times, keep up to `k - 1` candidates with counters. Verification is again a second pass.

**Complexity.** O(n) time, O(1) space (O(k) for the generalization).

### 3.3 Hash Sets and Fast Membership Lookup

**What it is.** Use a hash set (or map) to answer "have I seen this value / is this value present?" in O(1) average, instead of rescanning the array in O(n).

```cpp
unordered_set<int> seen;
for (int x : a) {
    if (seen.count(x)) { /* x was seen earlier */ }
    seen.insert(x);
}
```

**Key idea.** Trade O(n) memory for O(1) lookups. Two common access patterns:

1. **Query, then insert** while scanning: the structure holds exactly "everything before the current position."
2. **Insert everything first, then query:** the structure holds the whole input, enabling arbitrary lookups.

A map version stores extra information per key (such as the index where it was last seen):

```cpp
unordered_map<int,int> lastIndex;          // value -> index
for (int i = 0; i < (int)a.size(); ++i) {
    auto it = lastIndex.find(a[i]);
    if (it != lastIndex.end()) { /* it->second is an earlier index */ }
    lastIndex[a[i]] = i;
}
```

**Constraint validation.** Hash sets also express "no repeats within a group" conditions: keep one set per group and check membership before inserting.

**Complexity.** O(n) time on average, O(n) space.

### 3.4 Frequency Counting and Grouping by Key

**Frequency counting** maintains `value → count`:

```cpp
unordered_map<int,int> freq;
for (int x : a) freq[x]++;          // missing keys are created with value 0
```

When the key range is small and fixed, use the direct-address version (`int cnt[26]`) for speed and simplicity. Two frequency tables can be **compared**, **updated incrementally** (add one, subtract one), or **combined**.

**Grouping by key** generalizes counting: instead of a number, each key maps to a *collection*.

```cpp
unordered_map<int, vector<string>> groups;   // key -> everything sharing that key
for (const string& w : words) groups[(int)w.size()].push_back(w);
```

The decisive design question is **what to use as the key**. A key can be the value itself or something *computed* from it (here the length). Two items belong to the same group exactly when their computed keys are equal. The computed key must be of a hashable type; if it isn't, convert it (for example, to a `string`) or use an ordered `map`.

**Complexity.** Time O(n) average (plus the cost of computing each key), space O(number of distinct keys + stored items).

### 3.5 Prefix and Suffix Accumulation

**1D prefix sums** (§2.7) answer range-sum queries. **Suffix accumulations** run from the right instead: `suf[i]` summarizes `a[i..n)`.

**Generalization.** Prefix/suffix tables can store any **associative** accumulation (sum, product, max, min, ...). Range queries from a single prefix table (`pre[r+1] - pre[l]`) additionally require the operation to be **invertible** (sums are; max is not).

**Combining prefix and suffix.** To summarize *everything except position `i`*, combine the prefix over `[0, i)` with the suffix over `(i, n)`:

```cpp
int n = a.size();
vector<int> pre(n + 1, INT_MIN), suf(n + 2, INT_MIN);   // INT_MIN: identity for max
for (int i = 0; i < n; ++i)      pre[i + 1] = max(pre[i], a[i]);
for (int i = n - 1; i >= 0; --i) suf[i]     = max(suf[i + 1], a[i]);
// max of everything except a[i]: max(pre[i], suf[i + 1])
```

Each table's **identity value** (0 for sum, 1 for product, `INT_MIN` for max) represents "nothing accumulated yet." Accumulations can often be folded into running variables to save space.

**2D prefix sums.** For an `m × n` grid, let `P` be `(m+1) × (n+1)`, where `P[i][j]` is the sum of the rectangle with corners `(0,0)` and `(i-1,j-1)`:

```cpp
// build
for (int i = 0; i < m; ++i)
    for (int j = 0; j < n; ++j)
        P[i+1][j+1] = g[i][j] + P[i][j+1] + P[i+1][j] - P[i][j];

// query: sum of rectangle with top-left (r1,c1) and bottom-right (r2,c2), inclusive
int s = P[r2+1][c2+1] - P[r1][c2+1] - P[r2+1][c1] + P[r1][c1];
```

This is **inclusion–exclusion**: the two overlapping "extra" regions are subtracted, and their intersection, which got subtracted twice, is added back once.

**Complexity.** Build O(n) (or O(mn) in 2D) time and space; each query O(1). Use `long long` when sums may exceed `int` range.

### 3.6 Sorting and Ordering-Based Techniques

**Sorting as preprocessing.** Sorting costs O(n log n) but changes the structure of the data: equal values become adjacent, neighbors become the closest values, and order-based reasoning becomes possible.

```cpp
sort(a.begin(), a.end());
for (int i = 1; i < (int)a.size(); ++i) {
    if (a[i] == a[i - 1]) { /* equal values are adjacent */ }
}
```

**Custom ordering.** Sort by something other than the raw value, such as a stored count or a computed key:

```cpp
vector<pair<int,int>> items = {{3, 10}, {1, 20}};   // (value, count)
sort(items.begin(), items.end(),
     [](const auto& x, const auto& y) { return x.second > y.second; });  // by count, descending
```

**Selecting the k largest / smallest.** Several approaches, with different trade-offs:

| Approach | Time | Extra space |
|---|---|---|
| Sort everything, take first `k` | O(n log n) | O(1) or O(n) |
| Min-heap of size `k` (evict smallest when size exceeds `k`) | O(n log k) | O(k) |
| Bucket by score (when scores are bounded integers) | O(n) | O(n) |

```cpp
// min-heap of (score, item), keeping only the k best
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
for (auto& [item, score] : scores) {
    pq.push({score, item});
    if ((int)pq.size() > k) pq.pop();   // removes the smallest
}
```

`priority_queue` is a **max-heap** by default. Passing `greater<...>` makes it a min-heap.

**Bucketing.** When each element has an integer score between 0 and `n`, create `vector<vector<int>> buckets(n + 1)` and place each element in `buckets[score]`. Reading buckets from high to low then gives elements in decreasing score order without comparison sorting. This is counting sort applied to scores.

### 3.7 Index as Key (Direct Addressing, In-Place Marking, Grid Mapping)

**What it is.** Use array positions themselves as hash-table slots, so that the array doubles as both data and lookup structure.

**Value → index.** If all relevant values lie in `[0, n)` (or can be shifted into it), the value can determine *where it belongs*. Placing each value at its "home" index turns a later membership check into an array access:

```cpp
int n = a.size();
for (int i = 0; i < n; ++i) {
    while (0 <= a[i] && a[i] < n && a[a[i]] != a[i])
        swap(a[i], a[a[i]]);        // send a[i] to its home; repeat for the swapped-in value
}
// afterwards, position j holds j if j was present
```

The `a[a[i]] != a[i]` check prevents infinite loops with duplicates. Each swap places one value permanently, so the total number of swaps is O(n).

**Marking instead of moving.** Another variant records "value `v` is present" by *flagging* the element at index `v` (for example, by negating it) rather than moving elements. Any in-place encoding like this must still let you recover the original information.

**Mapping 2D or structured positions to flat keys.** A position `(r, c)` can be converted to a single index (`r * cols + c`), or to a block identifier via integer division:

```cpp
int blockId = (r / h) * (width / w) + (c / w);   // block of size h×w in a grid of given width
```

Fixed-size lookups per row, column, or block can then be stored in arrays of sets, arrays of `bool`, or bitmasks.

**Complexity.** O(n) time, O(1) extra space for the in-place versions; this works only under range guarantees on values.

### 3.8 Encoding Structured Data into a Single String

**What it is.** Converting a sequence of strings into one string so that it can be **uniquely decoded** back into the original sequence.

**The problem with delimiters.** Joining with a separator like `","` fails if the data itself may contain `","`. Ambiguity makes decoding impossible.

**Length-prefixed encoding.** Write each item's **length**, then a marker, then the item itself. A decoder reads the length first and then knows exactly how many characters to take, so the content never needs to be inspected for delimiters.

```cpp
// encode one field
out += to_string(field.size()) + '#' + field;        // e.g. "5#hello"

// decode one field starting at position i
int j = s.find('#', i);                              // marker location (first '#' after the digits)
int len = stoi(s.substr(i, j - i));
string field = s.substr(j + 1, len);
i = j + 1 + len;                                     // start of next field
```

Because the digits end at the first `#` after position `i`, and the content length is known, any characters (including `#` and digits) can appear in the content.

**Complexity.** O(total characters) time and space for both directions.

---

## 4. Combining Techniques

**Prefix sums + hash map.** Prefix sums convert "sum of a range" into "difference of two prefix values." A hash map can store which prefix values have already occurred (and how often). Together they let you reason about *all* ranges ending at the current position using O(1) lookups instead of O(n) scanning.

```
prefix accumulation  +  hash lookup of earlier prefix values
        ↓
range-based questions answered in one pass
```

**Frequency counting + bucketing/heap.** Counting produces `item → count`. A heap or buckets then *rank* items by count. Counting handles aggregation; ordering handles selection.

**Sorting + linear scan.** After sorting, related elements are adjacent or ordered, and one scan with running state (§3.2) can finish the job. The sort pays O(n log n); the scan adds O(n).

**Computed keys + hash grouping.** Defining a good key function (§3.4) decides *what counts as equivalent*. A hash map then does the grouping automatically.

**Prefix + suffix tables.** Two sweeps (left-to-right and right-to-left) produce two tables whose entries combine to give "everything except here" information with no per-position rescanning.

**Index-as-key + final scan.** After elements are placed at positions determined by their values (§3.7), one pass reading the array reveals which values are absent or misplaced.

**Hash set + running state.** A set can supply the "does it exist?" queries while a loop variable tracks the best result so far.

---

## 5. C++ Implementation Notes

- **Prefer `unordered_*` for speed, `map/set` for ordering or non-hashable keys.**
- **`mp[key]` inserts** a default value (0 for `int`) when `key` is absent. Use `count()` or `find()` for pure lookups.
- **Iterating:** `for (auto& [k, v] : mp)`. Order is unspecified.
- **Sizes:** `v.size()` returns an unsigned type. Cast with `(int)v.size()` when comparing against negative numbers or subtracting.
- **2D vectors:** `vector<vector<int>> g(m, vector<int>(n));`; dimensions are `g.size()` and `g[0].size()`.
- **Frequency arrays:** `int cnt[26] = {};` for lowercase letters; `vector<int> cnt(128)` for arbitrary ASCII.
- **Erasing from a `vector`:** `v.erase(v.begin() + i)` is O(n). Prefer in-place compaction (§3.1) when many removals are needed.
- **`swap(a[i], a[j])`** works for any swappable type, including `vector` and `string`.
- **Large sums:** use `long long` for prefix sums and totals.
- **Creating an empty-but-sized buffer:** `vector<int> tmp(n);` for merge sort.

---

## 6. Complexity & Tradeoffs

| Structure / Technique | Time | Extra Space | Notes |
|---|---|---|---|
| Array index | O(1) | — | |
| `vector` insert/erase in middle | O(n) | — | shifting |
| `push_back` | O(1) amortized | — | |
| `unordered_set/map` insert/find/erase | O(1) avg, O(n) worst | O(n) | |
| `set/map` operations | O(log n) | O(n) | sorted |
| Frequency table (array) | O(n) | O(R) | R = key range |
| Frequency table (hash map) | O(n) avg | O(distinct keys) | |
| Prefix sums: build / query | O(n) / O(1) | O(n) | |
| 2D prefix sums: build / query | O(mn) / O(1) | O(mn) | |
| In-place compaction / partitioning | O(n) | O(1) | |
| Boyer–Moore voting | O(n) | O(1) | needs a verification pass |
| Merge sort | O(n log n) | O(n) | stable |
| Quicksort | O(n log n) avg, O(n²) worst | O(log n) stack | in-place |
| Counting sort | O(n + R) | O(R) | small value range only |
| `std::sort` | O(n log n) | O(1)–O(log n) | |
| Heap push / pop | O(log n) | O(size) | |
| Top-k with heap of size k | O(n log k) | O(k) | |
| Bucket by count | O(n) | O(n) | |

**Main tradeoffs**

- **Time vs. space.** Hash structures and prefix tables spend O(n) memory to remove O(n) rescans. In-place techniques save memory but usually require more careful reasoning about invariants.
- **Hashing vs. sorting.** Hashing is O(n) average but gives no ordering; sorting is O(n log n) but gives ordering and needs no extra structure.
- **Hash map vs. direct-address array.** Use an array when the key range is small and known; use a hash map when keys are sparse or unbounded.
- **Average vs. worst case.** Hash operations are O(1) on average only if keys spread across buckets.
- **Heap vs. full sort for top-k.** A heap wins when `k` is much smaller than `n`; sorting is simpler when `k` is large or the extra log factor does not matter.

---

## 7. Important C++ Pitfalls

1. **`operator[]` on a map inserts.** `if (mp[x] == 0)` creates `x` in the map. Use `mp.count(x)` or `mp.find(x)` to avoid it.
2. **Unsigned `size()`.** `v.size() - 1` on an empty vector wraps around to a huge number. Write `(int)v.size() - 1`.
3. **Modifying a container while iterating over it.** Erasing from an `unordered_map` or `vector` inside a loop over it can invalidate iterators. Collect keys first, or use the erase-return-iterator form.
4. **`pair`/`vector` as `unordered` keys** does not compile without a custom hash. Use `map`/`set`, or convert the key to a `string`.
5. **Comparator must be a strict ordering.** `a <= b` in a sort comparator can crash or loop forever.
6. **`priority_queue` is a max-heap by default.** Use `greater<>` for a min-heap.
7. **Integer overflow.** Prefix sums, products, and running totals can exceed `int`. Use `long long`.
8. **Negative `%`.** `-3 % 5` is `-3` in C++. If you hash signed integers, normalize with `((x % B) + B) % B`.
9. **Off-by-one in prefix arrays.** Keep the convention `pre[i+1] = pre[i] + a[i]` and size `n+1`, and stay consistent with inclusive/exclusive range ends.
10. **Pass large containers by reference** (`vector<int>&` or `const vector<int>&`). Passing by value copies the whole thing.
11. **Swapping in place without advancing blindly.** In partition/placement loops, the element swapped *into* position `i` has not been examined yet; whether to advance `i` depends on the invariant.
12. **Vector initialization.** `vector<int> v(n)` creates `n` zeros; `vector<int> v{n}` creates a vector with the single element `n`.

---

## 8. Short Recap

- Arrays give O(1) indexing; most array techniques are about **how to sweep** (read/write indices, partitions, running state) or **how to precompute** (prefix/suffix tables).
- Hash tables give O(1) average lookup by mapping keys to buckets; collisions are handled by chaining. Sets answer *presence*, maps store *associations*, and the choice of **key** defines what you can express.
- Sorting changes the structure of the data (adjacency, order), counting sort and bucketing exploit small value ranges, and heaps support efficient selection.
- Array positions can act as hash slots when values are range-limited.
- Variable-length data can be packed unambiguously with length prefixes.
- Techniques compose: accumulation + lookup, counting + ranking, sorting + scanning, prefix + suffix.

I can also put this chapter into a downloadable file (Markdown or Word) if you'd like.
