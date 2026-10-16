# Binary Search: Theory Chapter

---

## 1. Prerequisites

### Must Know

**C++ basics**

- `vector<int>`: declaration, initialization (`vector<int> v(n, 0)`, `vector<int> v = {1, 2, 3}`), indexing with `[]`, `size()`, `push_back()`, `begin()`, `end()`, `back()`
- `vector<vector<int>>`: declaring, `v.size()` for rows, `v[0].size()` for columns, `v[i][j]` access
- `string`: indexing, `size()`, comparison with `<`, `==`
- Passing containers to functions by reference (`const vector<int>& v`)
- `int` vs `long long`: ranges (about 2.1 × 10⁹ vs 9.2 × 10¹⁸), integer division and its truncation, casting with `(long long)x`
- `while` and `for` loops, `break`, `return` from inside loops
- `pair<int,int>`, `make_pair`, `.first`, `.second`
- Lambdas (basic form): `[&](int x) { return x > 5; }`
- `map<K, V>`: declaration, `[]`, `insert`, `find()`, `count()`

**Concepts**

- Big-O basics: O(1), O(log n), O(n), O(n log n)
- Sorted order: ascending, non-decreasing, strictly increasing
- Integer arithmetic: floor division, remainder (`%`)

### Useful but Not Required

- Logarithms: "how many times can n be halved before reaching 1"
- Writing a loop with an invariant (a condition true before every iteration)
- Linear scan and its O(n) cost
- Basic familiarity with `sort`
- Floating-point types (`double`) and their precision limits

---

## 2. Core Concepts

### 2.1 The Idea: Discarding Half

Binary search is an algorithm that finds something in a search space by **repeatedly eliminating half of the remaining candidates** using a single comparison.

It works only when the search space has **structure**: examining one position tells you which side of that position the answer cannot be on. The classic structure is a sorted array.

Consider a sorted array and a target of 23:

```text
index:   0   1   2   3   4   5   6   7   8
value:   2   5   8  12  16  23  38  56  72

Step 1: look at the middle (index 4, value 16).
        16 < 23, so everything at index 0..4 is too small.
        Remaining candidates: index 5..8

Step 2: look at the middle of 5..8 (index 6, value 38).
        38 > 23, so everything at index 6..8 is too large.
        Remaining candidates: index 5..5

Step 3: look at index 5. Value 23. Found.
```

Each step removes about half the candidates, so the number of steps is about log₂(n).

| n | steps (≈ log₂ n) |
| --- | --- |
| 1,000 | 10 |
| 1,000,000 | 20 |
| 1,000,000,000 | 30 |

### 2.2 The Requirement: Monotonic Structure

The real requirement of binary search is not "the array is sorted." It is the following:

> You can define a **yes/no question** about each position (or value) such that the answers form a **block of one answer followed by a block of the other**.

```text
position:   0  1  2  3  4  5  6  7  8
answer:     F  F  F  F  F  T  T  T  T
                         ↑
                  boundary: first True
```

This is called a **monotonic predicate**. Once the answer flips, it never flips back. Binary search finds the boundary between the two blocks.

For a sorted array and a target `t`, a natural predicate is "`a[i] >= t`". Since the array is sorted, this is `F F F … F T T T … T`.

This view is more general and more useful than "search for a value," because:

- the boundary may exist even when no element equals the target,
- the positions need not be array indices; they can be numbers in a range,
- the predicate can be any computation, not only a comparison with an array element.

### 2.3 Search Space

The **search space** is the set of candidates the answer may lie in. It is described by two bounds, `lo` and `hi`. Choosing these correctly is half the work:

- The search space must **contain the answer**.
- It should not be wastefully large (though a huge space costs only a few extra steps, since the cost is logarithmic).
- You must decide whether each bound is **inclusive** or **exclusive**, and stay consistent.

### 2.4 Invariants

An **invariant** is a statement that is true at the start of every loop iteration. A correct binary search is simply a loop that maintains an invariant while shrinking the space.

Examples of invariants (for a predicate `F…FT…T`):

- "Everything left of `lo` is False."
- "Everything at `hi` and to the right is True."

If you write down the invariant first, the `lo`/`hi` updates and the loop condition follow almost mechanically. Most binary search bugs come from not having a clear invariant.

### 2.5 Computing the Midpoint

```cpp
int mid = lo + (hi - lo) / 2;   // safe
```

`(lo + hi) / 2` can overflow `int` when `lo` and `hi` are both large. The form above cannot. Integer division rounds down (for non-negative values), so `mid` is biased toward `lo`. This matters for correctness in some loop shapes (Section 3.2).

---

## 3. Patterns & General Techniques

### 3.1 Technique: Exact-Match Search

**What it is.** Find the position of a given value in a sorted sequence, or report that it is absent.

**How it works.** Compare `a[mid]` with the target:

- equal → done,
- `a[mid] < target` → the target, if present, is to the right,
- `a[mid] > target` → the target, if present, is to the left.

**Underlying idea.** Every comparison has three outcomes, two of which let you discard a half (and `mid` itself).

**C++ (inclusive bounds `[lo, hi]`):**

```cpp
int binarySearch(const vector<int>& a, int target) {
    int lo = 0, hi = (int)a.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] == target) return mid;
        else if (a[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;   // not found
}
```

**Why `lo <= hi`:** with inclusive bounds, a single remaining candidate (`lo == hi`) still needs to be checked. The loop ends when the space is empty (`lo > hi`).

**Why `mid + 1` and `mid - 1`:** `mid` has already been examined, so it is excluded. This guarantees the space strictly shrinks, which guarantees termination.

**Complexity.** Time O(log n), space O(1).

**Small example.** For `a = {1, 3, 5, 7}` and target `4`:
`lo=0, hi=3 → mid=1 (3<4) → lo=2 → mid=2 (5>4) → hi=1 → lo>hi → return -1`.

When the loop ends without finding the target, `lo` has an extra meaning: it is the index where the target **would be inserted** to keep the array sorted. This observation leads to the next technique.

---

### 3.2 Technique: Boundary Search (First True / Last False)

**What it is.** Given a monotonic predicate `F…FT…T` over a range, find the **first position where it is True**. This is the most general and reusable form of binary search.

**How it works.** Keep an interval that is guaranteed to contain the boundary. Test the midpoint:

- predicate True at `mid` → the boundary is at `mid` or to its left (`mid` itself might be the answer, so keep it),
- predicate False at `mid` → the boundary is strictly to the right.

**C++ (half-open interval `[lo, hi)`, answer in `[lo, hi]`):**

```cpp
// Returns the first index in [0, n) where pred is true, or n if none.
int firstTrue(int n, auto pred) {
    int lo = 0, hi = n;                  // hi = n means "none is true"
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (pred(mid)) hi = mid;         // mid could be the answer: keep it
        else           lo = mid + 1;     // mid is False: discard it
    }
    return lo;                           // lo == hi == boundary
}
```

**Invariants.** Everything before `lo` is False; everything at `hi` or after is True (with an imaginary True just past the end). When `lo == hi`, they meet at the boundary.

**Why this form is easy to get right.**

- `hi = mid` (not `mid - 1`) because `mid` may be the answer.
- `lo = mid + 1` because `mid` is known False.
- The loop condition is `lo < hi`, because when `lo == hi` the answer is determined.
- With `mid` rounded down, `mid < hi` always holds inside the loop, so `hi = mid` always shrinks the space. No infinite loop.

**The mirror image: last True.** If the predicate is `T…TF…F` and you want the last True, use the *upper* midpoint to avoid an infinite loop:

```cpp
int lastTrue(int n, auto pred) {         // pred: T...TF...F over [0, n)
    int lo = -1, hi = n - 1;             // lo = -1 means "none is true"
    while (lo < hi) {
        int mid = lo + (hi - lo + 1) / 2;   // round UP
        if (pred(mid)) lo = mid;            // mid could be the answer
        else           hi = mid - 1;
    }
    return lo;
}
```

If you write `lo = mid` with a rounded-down `mid`, then when `hi == lo + 1` we get `mid == lo` and the loop never progresses. **Whenever a branch assigns `mid` (not `mid ± 1`) to a bound, check which direction `mid` rounds.**

**Lower bound and upper bound.** Two standard boundaries in a sorted array `a` and value `x`:

| Name | Meaning | Predicate (first true) |
| --- | --- | --- |
| lower bound | first index with `a[i] >= x` | `a[i] >= x` |
| upper bound | first index with `a[i] > x` | `a[i] > x` |

Consequences:

- `upper - lower` = number of occurrences of `x`.
- `lower` is the insertion position that keeps the array sorted (before any equal elements).
- `x` is present iff `lower < n && a[lower] == x`.

**Complexity.** Time O(log n) predicate evaluations, space O(1). If evaluating the predicate costs O(c), total time is O(c · log n).

---

### 3.3 Technique: Binary Search on the Answer (Value Space)

**What it is.** Apply boundary search not to an array, but to a **range of candidate numeric answers**, using a computed predicate.

**The core idea.** Sometimes there is no array to search. Instead, you have a numeric quantity `x` that you are trying to choose, and a way to **check whether a given `x` is acceptable**. If acceptability is monotonic in `x` (a larger `x` is never harder to satisfy, or never easier), then the acceptable values form `F…FT…T`, and the smallest (or largest) acceptable `x` can be found by boundary search.

```text
candidate x:   1  2  3  4  5  6  7  8  9 ...
feasible(x):   F  F  F  F  T  T  T  T  T ...
                          ↑
                  smallest feasible x
```

**Components.**

1. **Search range** `[lo, hi]`: a lower and upper limit that is guaranteed to bracket the answer.
2. **Check function** `feasible(x) → bool`: a deterministic computation, often a simple loop over the input.
3. **Monotonicity**: if `feasible(x)` is True then `feasible(x')` is True for all `x' > x` (or the reverse).
4. **Boundary search** over the range.

**C++ (smallest feasible integer in `[lo, hi]`):**

```cpp
long long smallestFeasible(long long lo, long long hi, auto feasible) {
    // Assumes feasible(hi) is true.
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (feasible(mid)) hi = mid;
        else               lo = mid + 1;
    }
    return lo;
}
```

For the *largest* feasible value (predicate `T…TF…F`), use the upper-midpoint form from Section 3.2.

**Choosing the range.** Reason about the extremes:

- What is the smallest value the answer could possibly be?
- What is a value that is certainly feasible (or certainly the largest sensible)?
Over-wide bounds are safe but cost a few extra iterations. Overly narrow bounds make the algorithm wrong. Use `long long` when bounds or sums can exceed `int`.

**Verifying monotonicity.** Before using this technique, convince yourself of the monotonic direction. A useful test: "If `x` works, does making `x` bigger (or smaller) still work?" If not, the predicate is not monotonic and binary search will give wrong results.

**Complexity.** Time O(C · log R), where R is the size of the range `hi − lo + 1` and C is the cost of one check. Space is usually O(1).

**Floating-point note.** If the answer may be real-valued, loop a fixed number of times (e.g., 50–100 iterations) instead of testing `lo < hi`, because doubles do not reach exact equality reliably:

```cpp
for (int iter = 0; iter < 100; iter++) {
    double mid = (lo + hi) / 2;
    if (pred(mid)) hi = mid; else lo = mid;
}
```

**Integer square root as a general illustration of the idea.** To find the largest integer `r` with `r * r <= n`: the predicate `r*r <= n` is `T…TF…F` over `r`, so the last-True form applies. Use `long long` for `r * r`.

---

### 3.4 Technique: Index Mapping (Treating Structured Data as Linear)

**What it is.** If data is stored in a structure that has a natural linear ordering but is not a flat array, you can binary search it by converting between a single linear index and the structure's real coordinates.

**Row-major mapping for a grid with `R` rows and `C` columns:**

```text
linear index k  ↔  (row, col)

row = k / C
col = k % C
k   = row * C + col
```

```text
C = 4, linear index 6:   row = 6/4 = 1, col = 6%4 = 2
```

If the grid, read row by row, forms a single sorted sequence, then it can be searched like a 1D array of size `R * C`:

```cpp
int lo = 0, hi = R * C - 1;
while (lo <= hi) {
    int mid = lo + (hi - lo) / 2;
    int val = grid[mid / C][mid % C];
    // compare val with target and update lo/hi as in Section 3.1
}
```

**General principle.** Binary search needs only: (1) a way to address "the k-th element," and (2) the monotonic structure. The storage layout is irrelevant as long as `k → element` is O(1).

**Complexity.** O(log(R·C)) = O(log R + log C). Space O(1).

---

### 3.5 Technique: Searching Under Partial Order (Eliminating Half by Local Reasoning)

**What it is.** Binary search does not need the entire sequence to be globally sorted. It only needs this: **from the value(s) at `mid` and a few reference points, you can decide which half is guaranteed to exclude the answer.**

**How to reason about it.**

1. Identify what you are looking for (a value, a boundary, a minimum, a peak, etc.).
2. Identify **what structural fact** holds on each side of `mid` (for instance, "this half is sorted", "this half is increasing", "the values here are all ≥ some reference value").
3. Use a comparison at `mid` to determine which half contains the target or boundary, and discard the other.

**Template:**

```text
while the space has more than one candidate:
    mid = middle
    use a[mid] (and maybe a[lo], a[hi], a[mid±1]) to determine
        which half CANNOT contain the answer
    discard that half (keeping mid if mid may be the answer)
```

**Guidelines.**

- Decide beforehand what comparison separates "answer is in the left part" from "answer is in the right part." Write this out in words before coding.
- Decide whether `mid` may be the answer. If yes, keep it (`hi = mid` or `lo = mid`); if not, discard it (`mid ± 1`).
- A comparison against a **fixed reference** (such as an endpoint of the current space) often turns a messy structure into a clean monotonic predicate.
- Check the **degenerate cases**: equal values, a space with 1 or 2 elements, a space where the structure is trivial (e.g., fully sorted).

**Duplicates weaken the guarantee.** When equal values are allowed, a single comparison can sometimes fail to tell you which half to discard (if `a[lo] == a[mid] == a[hi]`, you learn nothing about structure). In such cases you can still safely discard a **provably useless element**, typically one endpoint, at the cost of shrinking the space by 1 instead of by half. The consequence is that the worst case degrades from O(log n) to **O(n)**. This is an important theoretical point: a binary search's guarantee comes entirely from the information each comparison provides, and duplicates reduce that information.

**Complexity.** O(log n) when each step discards half; O(n) worst case when some steps can only discard a constant number of elements.

---

### 3.6 Technique: Binary Search over a Split Position

**What it is.** Instead of searching for an element, search for **how many elements to place on one side of a split**. The predicate is evaluated by comparing elements just left and just right of the split.

**Idea.** Suppose you have two sorted sequences `A` (size `m`) and `B` (size `n`), and you want to divide the combined elements into a "left group" and a "right group" of prescribed sizes, such that every element in the left group is ≤ every element in the right group.

Choose `i` = number of elements taken from `A` for the left group. The remaining left-group elements come from `B`: `j = k − i`, where `k` is the required left-group size.

```text
A:  a0 a1 … a(i-1) | a(i) …
B:  b0 b1 … b(j-1) | b(j) …
               left | right
```

The split is valid when:

```text
A[i-1] <= B[j]     and     B[j-1] <= A[i]
```

(treating out-of-range elements as −∞ on the left side and +∞ on the right side).

If `A[i-1] > B[j]`, too many elements were taken from `A`: reduce `i`. If `B[j-1] > A[i]`, too few: increase `i`. This is a monotonic decision over `i`, so boundary search applies.

**Practical points.**

- Search over the **smaller** sequence to keep `j` in valid range and keep the search space small.
- Handle boundaries (`i = 0`, `i = m`, `j = 0`, `j = n`) using sentinels: `INT_MIN` / `INT_MAX`.
- The search range for `i` is `[max(0, k − n), min(k, m)]`.

**Complexity.** O(log(min(m, n))), space O(1).

**General principle.** The thing being binary searched need not be a data element or a candidate answer value. It can be a *count*, *size*, or *cut position* that defines a configuration, so long as the validity of the configuration is monotonic in that quantity.

---

### 3.7 Technique: Binary Search via STL and Ordered Containers

**What it is.** Using the standard library to avoid hand-written loops when the plain sorted-sequence structure applies.

| Function | Returns | Requirement |
| --- | --- | --- |
| `binary_search(b, e, x)` | `bool`: is `x` present? | sorted range |
| `lower_bound(b, e, x)` | iterator to first element `>= x` | sorted range |
| `upper_bound(b, e, x)` | iterator to first element `> x` | sorted range |

```cpp
#include <algorithm>
vector<int> a = {1, 3, 3, 5, 8};

auto it = lower_bound(a.begin(), a.end(), 3);   // points to a[1]
int idx = it - a.begin();                       // 1

auto it2 = upper_bound(a.begin(), a.end(), 3);  // points to a[3]
int cnt = it2 - it;                             // 2 occurrences of 3

bool present = binary_search(a.begin(), a.end(), 5);   // true
```

If `lower_bound`/`upper_bound` returns `a.end()`, there is no such element. **Never dereference `a.end()`.**

**Custom comparison.** For ranges sorted in another way, or searching by a key (such as a pair's first member), supply a comparator:

```cpp
vector<pair<int,int>> p = {{1,10}, {4,20}, {7,30}};
// first pair whose .first >= 4
auto it = lower_bound(p.begin(), p.end(), 4,
    [](const pair<int,int>& e, int key) { return e.first < key; });
```

For `lower_bound`, the comparator takes `(element, value)` and returns whether the element comes **before** the value. For `upper_bound`, the arguments swap: `(value, element)` and it returns whether the value comes before the element.

**Ordered containers.** `map` and `set` keep keys sorted and provide logarithmic search:

```cpp
map<int, string> m;
m[10] = "a"; m[20] = "b"; m[30] = "c";

auto it = m.lower_bound(15);   // first key >= 15  → key 20
auto jt = m.upper_bound(20);   // first key >  20  → key 30

// Greatest key <= x:
auto kt = m.upper_bound(x);
if (kt != m.begin()) { --kt; /* kt is the greatest key <= x */ }
```

`map::lower_bound` and `map::upper_bound` are O(log n). The free function `std::lower_bound` on a `map`'s iterators is *not* logarithmic, because map iterators are not random-access. Always use the member function for containers.

**Complexity.** O(log n) for all of the above on random-access ranges and ordered containers.

---

### 3.8 Technique: Interface-Based Search (Searching via Queries)

**What it is.** Sometimes you cannot see the data directly but can only ask a function a question, such as "how does the value at position `i` compare?" or "is `x` too high, too low, or correct?" Binary search applies unchanged: the function plays the role of the comparison.

```cpp
// Suppose cmp(x) returns -1, 0, or 1 describing x relative to a hidden target.
int lo = 1, hi = N;
while (lo <= hi) {
    int mid = lo + (hi - lo) / 2;
    int r = cmp(mid);
    if (r == 0) return mid;
    if (r < 0)  hi = mid - 1;   // interpret per the function's contract
    else        lo = mid + 1;
}
```

Read the function's contract carefully: the sign convention (which result means "go left") is defined by the function, not by assumption.

**Costs.** Each query may be expensive or limited in number. Binary search minimizes the number of queries to O(log n). This is also why wasting queries (for example, re-querying the same position, or reading the length of a hidden structure one query at a time) matters in this setting. If the size of the structure is unknown but queries are allowed, you can first find an upper limit by doubling (`1, 2, 4, 8, …`) until the bound is exceeded, then binary search within that bound. That takes O(log n) queries in total.

---

## 4. Combining Techniques

### 4.1 Boundary Search + Greedy/Simulation Check

```text
Binary search on the answer (3.3)
              +
a linear scan that simulates/validates one candidate
              ↓
O(n log R) solution to optimization problems
```

The outer binary search picks a candidate; the inner scan decides feasibility in O(n). The combination converts "find the best value" into "answer yes/no repeatedly," which is often far easier to compute.

### 4.2 Boundary Search + Sorted Preprocessing

```text
Sort (O(n log n))  +  lower_bound / upper_bound queries (O(log n) each)
              ↓
fast repeated queries on static data
```

Pay the sorting cost once, then answer many queries logarithmically. Sorting is useful whenever repeated lookups follow a one-time setup.

### 4.3 Ordered Containers + Boundary Queries

```text
map<key, value>  +  lower_bound / upper_bound
              ↓
"nearest key at or before / at or after x" lookups with updates
```

When data arrives in an order that keeps the keys sorted (for example, timestamps that only increase), a `vector` of pairs that is appended to is already sorted and can be binary searched directly without a tree-based container. When insertions can arrive in any order, an ordered container maintains order for you.

### 4.4 Index Mapping + Exact-Match Search

```text
linear ↔ 2D index conversion (3.4)  +  exact-match search (3.1)
              ↓
binary search over any row-major sorted layout
```

### 4.5 Partial-Order Reasoning + Boundary Search

```text
local structural reasoning (3.5)  +  first-true form (3.2)
              ↓
search in structures that are almost-sorted or piecewise-sorted
```

Find a predicate (often a comparison against a fixed reference point) that turns the piecewise structure into a single `F…FT…T` pattern; then the standard loop applies.

---

## 5. C++ Implementation Notes

**A recommended default loop.** Prefer the half-open, first-True form (Section 3.2). It has one loop condition (`lo < hi`), one update per branch, and an obvious invariant. Most variants (lower bound, insertion position, smallest feasible value) are special cases.

**Choosing the form.**

| Situation | Suggested loop |
| --- | --- |
| Is `x` in the array, and where? | inclusive `[lo, hi]`, `while (lo <= hi)`, return on match |
| Smallest index/value with a property | half-open first-True, `hi = mid` / `lo = mid + 1` |
| Largest index/value with a property | last-True form, upper midpoint, `lo = mid` / `hi = mid - 1` |
| Real-valued answer | fixed iteration count |

**Lambdas as predicates.** Passing the check as a lambda keeps the search loop generic:

```cpp
auto ok = [&](long long x) {
    // compute and return whether x is feasible
};
```

Using `[&]` captures surrounding variables by reference so the lambda can read the input data.

**Sentinels.** `INT_MIN`/`INT_MAX` (from `<climits>`) simplify out-of-range handling. For `long long` computations use `LLONG_MIN`/`LLONG_MAX`.

**Ceiling division of positive integers.**

```cpp
int ceilDiv = (a + b - 1) / b;    // ceil(a / b) for a >= 0, b > 0
```

---

## 6. Complexity & Tradeoffs

| Technique | Time | Space |
| --- | --- | --- |
| Exact-match search | O(log n) | O(1) |
| Boundary search (array) | O(log n) | O(1) |
| Binary search on answer | O(C · log R) | O(1) (plus check's needs) |
| Index mapping on an R×C grid | O(log(R·C)) | O(1) |
| Partial-order search (distinct values) | O(log n) | O(1) |
| Partial-order search (with duplicates) | O(n) worst case, O(log n) typical | O(1) |
| Split-position search over two sequences | O(log min(m, n)) | O(1) |
| `lower_bound`/`upper_bound` on vector | O(log n) | O(1) |
| `map::lower_bound` | O(log n) | O(1) |
| Sort + repeated binary searches | O(n log n + q log n) | O(1) or O(n) |

**Tradeoffs.**

- **Binary search vs linear scan.** Binary search requires monotonic structure. If the data is unsorted and you search only once, sorting first (O(n log n)) is *slower* than a single linear scan (O(n)). Binary search pays off with repeated queries or when the structure is already present.
- **Iterations vs range size.** A range of 10⁹ needs about 30 iterations; a range of 10¹⁸ needs about 60. So extra-wide bounds are cheap, but the cost of each check dominates the total.
- **Array vs ordered container.** A sorted `vector` has fast, cache-friendly search but O(n) insertion. A `map`/`set` supports O(log n) insertion and search but with higher constant factors and no random access.
- **Space.** The loops themselves use O(1) extra space. Recursion-based binary search uses O(log n) stack space, so iteration is preferred.

---

## 7. Important C++ Pitfalls

1. **Overflow in the midpoint.** Use `lo + (hi - lo) / 2`, not `(lo + hi) / 2`.
2. **Overflow in checks.** Computations such as `mid * mid` or running sums over many elements can exceed `int`. Use `long long`.
3. **Infinite loops.** Occur when a branch assigns `lo = mid` or `hi = mid` and `mid` can equal the bound being assigned. Match the midpoint rounding to the update style (Section 3.2).
4. **Off-by-one in loop condition.** `lo <= hi` pairs with inclusive bounds and `mid ± 1`; `lo < hi` pairs with half-open/first-True and `hi = mid`. Mixing them is the most common bug.
5. **Dereferencing `end()`.** Check `it != v.end()` before using the result of `lower_bound`/`upper_bound`.
6. **Subtracting iterators.** `it - v.begin()` gives an index for vectors. Do not use it on `map` or `set` iterators.
7. **Empty containers.** `(int)a.size() - 1` is `-1` for an empty array, which is a valid "empty" inclusive range. But `a.size() - 1` with `size_t` wraps around to a huge number. Cast to `int` first.
8. **`lower_bound` on a non-sorted range or with a mismatched comparator.** Results are undefined. The range must be sorted consistently with the comparator used.
9. **Negative division.** `/` and `%` on negative integers truncate toward zero in C++. Keep midpoints in non-negative ranges, or reason carefully if bounds can be negative.
10. **Floating-point equality.** Never loop `while (lo != hi)` on doubles. Use a fixed number of iterations or an epsilon comparison.

---

## 8. Recap

- Binary search works whenever a **monotonic yes/no predicate** splits a search space into two blocks.
- The most general form is **boundary search**: find the first True (or last True) in `F…FT…T`.
- The searched space can be **array indices, numeric answer values, linearly mapped coordinates, or split counts**.
- Correctness comes from **invariants** and from consistent choices about **inclusive vs exclusive bounds** and **midpoint rounding**.
- Each step costs one comparison or one check; the total is **O(log range) × cost of one check**.
- The STL provides `lower_bound`, `upper_bound`, and `binary_search` for sorted ranges, plus member versions on `map`/`set`.

---
