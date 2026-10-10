# Sliding Window: Theory Chapter

A standalone handbook chapter on the sliding window technique in C++.

## 1. Prerequisites

### Must Know

**C++ basics**

- `vector<int>`: declaration, initialization (`vector<int> v(n, 0)`), indexing with `[]`, `size()`, `push_back()`, range-based `for`, passing by `const vector<int>&`
- `string`: indexing with `s[i]`, `size()`, iterating over characters, `char` arithmetic (`c - 'a'` gives 0 to 25 for lowercase letters)
- `unordered_map<K,V>`: declaration, `[]` (inserts a default value if the key is missing), `count()`, `find()`, `erase()`, `size()`
- `unordered_set<T>`: declaration, `insert()`, `count()`, `erase()`, `size()`
- `deque<int>`: `push_back()`, `pop_back()`, `pop_front()`, `front()`, `back()`, `empty()`
- `min()` and `max()`, `INT_MAX` / `INT_MIN` from `<climits>`
- `for` and `while` loops, and loops with two index variables

**Concepts**

- Big-O basics: O(1), O(n), O(n log n), O(n²)
- Arrays are contiguous and indexed from 0; a contiguous piece of an array or string is called a **subarray** / **substring**
- Basic sorting and the idea of binary search on a sorted array

### Useful but Not Required

- Two-pointer technique on sorted arrays
- Prefix sums
- Amortized analysis (this chapter explains what is needed)

---

## 2. Core Concepts

### 2.1 Contiguous ranges and windows

A **window** is a contiguous range of a sequence, described by two indices `left` and `right`. This chapter uses the **inclusive** convention: the window `[left, right]` contains every element from `left` to `right`, and its length is `right - left + 1`.

```text
index:   0  1  2  3  4  5  6  7
value:   4  2  7  1  9  3  5  8
                [  window  ]
                left=2  right=4   -> elements 7, 1, 9  (length 3)
```

There are O(n²) possible windows in a sequence of length n. A naive approach examines each window and recomputes whatever property it needs (a sum, a count of distinct values, a maximum) from scratch. For a window of length k, that recomputation costs O(k), so the naive total is O(n² · k) or at best O(n²).

### 2.2 The sliding window idea

The **sliding window technique** moves a window across the sequence while **reusing information** from the previous window instead of recomputing it. Moving the window changes only its edges:

```text
before:  [ a  b  c  d ]  e
after:     a [ b  c  d  e ]
           ^leaves      ^enters
```

Only `a` leaves and only `e` enters. If the window's property can be **updated** for one entry and one exit, each move costs O(1) (or a small cost) instead of O(k).

### 2.3 Window state

The **window state** is whatever summary of the current window you maintain. It must support two operations:

- **add(x):** incorporate the element entering at the right end
- **remove(x):** undo the contribution of the element leaving at the left end

Common kinds of state:

| State | add(x) | remove(x) |
| --- | --- | --- |
| Sum | `sum += x` | `sum -= x` |
| Count of an element | `cnt[x]++` | `cnt[x]--` |
| Number of distinct values | increment when `cnt[x]` becomes 1 | decrement when `cnt[x]` becomes 0 |
| Maximum / minimum | needs a special structure (Section 3.5) | needs a special structure |

A useful rule: a quantity is easy to maintain if its removal is the exact inverse of its addition. Sums and counts have this property. Maximum and minimum do not, because removing the current maximum requires knowing the next largest element.

### 2.4 The two kinds of windows

**Fixed-size window.** The length `k` is given. The window always has exactly `k` elements, so `right` determines `left` (`left = right - k + 1`). Every step adds one element and removes one.

**Variable-size window.** The length is not given. The window grows and shrinks, and its size is decided by a **validity condition** on the window state. Both `left` and `right` only move forward (never backward).

### 2.5 Why variable windows are O(n): amortized cost

In a variable window, `right` advances once per outer iteration, and `left` advances inside an inner loop. The inner loop may run many times in one iteration, but `left` can never move past `right`, and neither pointer ever moves backward. So across the whole run:

- `right` moves at most n times
- `left` moves at most n times
- total pointer moves are at most 2n

The total work is therefore O(n), even though there is a loop inside a loop. This is **amortized** analysis: the cost of one iteration may be large, but the average over the whole run is O(1) per element.

---

## 3. Patterns & General Techniques

### 3.1 Fixed-size window

**What it is.** Maintain a window of constant length `k` and examine its state at each position.

**How it works.** Build the first window of `k` elements. Then repeat: add the element at `right`, remove the element at `right - k`, and record or compare the state.

**Complexity.** O(n) time if add/remove are O(1); O(1) extra space for a sum, O(alphabet) for counts.

```cpp
#include <vector>
#include <algorithm>
using namespace std;

// Largest sum over all windows of length k (assumes 1 <= k <= nums.size()).
long long maxWindowSum(const vector<int>& nums, int k) {
    long long sum = 0;
    for (int i = 0; i < k; i++) sum += nums[i];   // first window
    long long best = sum;
    for (int right = k; right < (int)nums.size(); right++) {
        sum += nums[right];        // element enters
        sum -= nums[right - k];    // element leaves
        best = max(best, sum);
    }
    return best;
}
```

### 3.2 Variable-size window (expand and shrink)

**What it is.** Move `right` forward one step at a time to **expand** the window. Whenever the window violates (or satisfies) a condition, move `left` forward to **shrink** it.

**Underlying idea.** The window always represents a candidate range. `right` explores new candidates; `left` repairs or tightens the window. Because neither pointer moves backward, every element is added at most once and removed at most once.

There are two common orientations, which differ in *what the condition means*:

1. **Shrink while invalid** (the window must stay valid). Expand `right`; while the window is invalid, shrink `left` until it is valid again. After shrinking, the window is valid, and you may record a result.
2. **Shrink while valid** (find the smallest window that satisfies a requirement). Expand `right`; while the window satisfies the requirement, record its size and shrink `left` to see whether a smaller one still satisfies it.

The distinction matters: in orientation 1 you record *after* the shrinking loop, in orientation 2 you record *inside* it.

**General template (orientation 1):**

```cpp
int left = 0;
for (int right = 0; right < n; right++) {
    add(nums[right]);                 // update window state
    while (!isValid()) {              // violated: repair
        remove(nums[left]);
        left++;
    }
    // window [left, right] is valid here; update the answer
    best = max(best, right - left + 1);
}
```

**General template (orientation 2):**

```cpp
int left = 0;
for (int right = 0; right < n; right++) {
    add(nums[right]);
    while (satisfiesRequirement()) {  // requirement met: try to tighten
        best = min(best, right - left + 1);
        remove(nums[left]);
        left++;
    }
}
```

**Complexity.** O(n) time (amortized, Section 2.5) when add, remove and the condition check are O(1). Space depends on the state.

**Monotonicity requirement.** Shrinking by moving `left` only makes sense if the validity condition behaves predictably: removing elements from the left should move the window *toward* the desired condition, and the condition should not flip back and forth unpredictably as the window changes. Sums of non-negative numbers, counts, and distinct-value limits all have this property. A sum that includes negative numbers does not (shrinking can increase or decrease it), and the technique in this form does not apply.

### 3.3 Frequency state for windows

**What it is.** Maintain a map `value → count` describing the multiset of elements inside the window.

**How it works.** `add(x)` increments `cnt[x]`; `remove(x)` decrements it. For characters from a small known alphabet, a fixed array replaces the hash map and is faster and simpler.

```cpp
#include <unordered_map>
#include <vector>
using namespace std;

unordered_map<char,int> cnt;          // general key type
cnt[c]++;                             // add
if (--cnt[c] == 0) cnt.erase(c);      // remove; erase keeps size() = number of distinct values

vector<int> freq(26, 0);              // lowercase letters only
freq[c - 'a']++;                      // add
freq[c - 'a']--;                      // remove
```

Useful derived quantities, each maintainable in O(1):

- **Number of distinct values** in the window: `cnt.size()` if zero counts are erased, or a separate counter updated when a count goes `0 → 1` or `1 → 0`.
- **Highest frequency** of any value in the window: increases are easy to track with `maxFreq = max(maxFreq, cnt[x])` on add. Decreases on remove are harder to track exactly; Section 3.7 discusses a relaxation that makes this unnecessary in some settings.

### 3.4 Comparing a window against a target (the match counter)

**What it is.** Given a **target** multiset (for example, the character counts of a string `t`), decide whether the window contains at least or exactly the required counts.

**Naive way.** Compare the whole window map with the whole target map at every step. That costs O(alphabet) per step.

**Counter idea.** Keep `need[x]` (target counts) and `have[x]` (window counts), and a single integer `formed` = the number of distinct values `x` whose requirement is currently satisfied. Update `formed` only when `have[x]` crosses the threshold `need[x]`:

```cpp
unordered_map<char,int> need, have;
int formed = 0;                        // satisfied distinct values
int required = need.size();            // total distinct values to satisfy

// add(c)
have[c]++;
if (need.count(c) && have[c] == need[c]) formed++;

// remove(c)
if (need.count(c) && have[c] == need[c]) formed--;
have[c]--;

// window satisfies the target when formed == required
```

The order inside `remove` matters: check **before** decrementing, because the threshold is lost at the moment the count drops below `need[c]`.

For an *exact-count* comparison with a fixed alphabet, a related approach is to compare two fixed-size arrays directly (`freq1 == freq2` on `array<int,26>` or `vector<int>` is element-wise comparison, O(26)), which is still O(1) with respect to n.

**Complexity.** O(1) per add/remove; O(|target alphabet|) space.

### 3.5 Monotonic deque (window maximum / minimum)

**The difficulty.** For a sum, removing the leaving element is a subtraction. For a maximum, if the leaving element *was* the maximum, the new maximum is unknown without looking at the remaining elements, which could cost O(k).

**What it is.** A **monotonic deque** stores candidate indices of the current window so that their values are in **decreasing** order from front to back (for a maximum). The front is always the index of the current maximum.

**Underlying idea.** If a new element `x` arrives, any earlier element in the window that is **smaller than or equal to** `x` can never be the maximum again: `x` is larger and will also stay in the window *longer* (it is further right). Such elements can be discarded permanently. What remains is a decreasing sequence of values, and the leftmost one is the maximum.

**Operations for each new index `right`:**

1. **Expire:** if the front index is outside the window (`front <= right - k`), `pop_front()`.
2. **Dominate:** while the back element's value is `<= nums[right]`, `pop_back()`.
3. **Insert:** `push_back(right)`.
4. The maximum is `nums[dq.front()]`.

The deque stores **indices**, not values, because indices are needed to decide when an element has left the window.

```cpp
#include <deque>
#include <vector>
using namespace std;

// Prints the maximum of every window of length k.
void windowMaxima(const vector<int>& a, int k, vector<int>& out) {
    deque<int> dq;                                  // indices; values decreasing
    for (int i = 0; i < (int)a.size(); i++) {
        if (!dq.empty() && dq.front() <= i - k) dq.pop_front();   // expire
        while (!dq.empty() && a[dq.back()] <= a[i]) dq.pop_back(); // dominate
        dq.push_back(i);
        if (i >= k - 1) out.push_back(a[dq.front()]);
    }
}
```

For a **minimum**, flip the comparison so values are increasing from front to back.

**Complexity.** Each index is pushed once and popped at most once, so total time is O(n) amortized. Space is O(k).

**Alternative.** A `multiset` or a priority queue with lazy deletion also yields window maxima, at O(log n) per step. The deque is the O(n) method.

### 3.6 Last-seen index map (jumping the left pointer)

**What it is.** Instead of shrinking `left` one step at a time by removing elements, store the **most recent index** at which each value appeared: `unordered_map<char,int> last`. When a value that is already inside the window reappears, move `left` directly past its previous occurrence:

```cpp
int left = 0;
unordered_map<char,int> last;
for (int right = 0; right < n; right++) {
    char c = s[right];
    if (last.count(c) && last[c] >= left) left = last[c] + 1;  // jump
    last[c] = right;
    // window [left, right] now has no repeated value
}
```

The check `last[c] >= left` is essential: a stored index to the left of `left` refers to an element that has already left the window and must be ignored. This is the same technique as 3.2, with a different way of repairing the window; time is O(n) and space O(alphabet).

### 3.7 Lazy shrinking (window that never shrinks)

Some tasks only ask for the **maximum possible length**, so the window does not need to be exactly valid at every moment; it only needs its **size** to be correct whenever a new record is possible. In this variant, when the window becomes invalid, `left` moves by **one step** (an `if` instead of a `while`) so that the window slides forward at constant size rather than shrinking. The window size never decreases; it increases only when a genuinely better window is found.

Because the answer is the final window size, a stale tracked value (such as an out-of-date `maxFreq`, which is only ever allowed to be too large) can never cause an invalid window to be counted as a larger record. This removes the need to recompute difficult statistics on removal. This is a relaxation of 3.2, applicable only when the objective is a maximum length and the state's stale value can only err on the safe side.

```cpp
int left = 0, best = 0;
for (int right = 0; right < n; right++) {
    add(a[right]);
    if (windowIsInvalid()) {        // 'if', not 'while'
        remove(a[left]);
        left++;
    }
    best = max(best, right - left + 1);
}
```

### 3.8 Running summary in a single pass

**What it is.** Some problems about a range "ending at the current position" or a pair of positions `i < j` can be handled in one forward pass by remembering a **summary of everything seen so far** (for example, the smallest value seen so far, or the best result so far) rather than re-examining earlier elements.

```cpp
int smallest = INT_MAX;                    // summary of the prefix
for (int x : a) {
    // use 'smallest' together with x to update an answer
    smallest = min(smallest, x);           // then fold x into the summary
}
```

The window here is conceptually "everything before the current position", and the summary is its state. Time O(n), space O(1).

### 3.9 Windows over sorted data: searching for the window position

When the sequence is **sorted** and the window has a fixed length `k`, the window's contents are fully determined by its starting index `start`, where `0 <= start <= n - k`. Instead of sliding step by step, you may **search** over `start` using binary search, provided you can compare two neighboring candidate windows and discard half of the possible starts.

```cpp
int lo = 0, hi = n - k;            // candidate window starts
while (lo < hi) {
    int mid = lo + (hi - lo) / 2;
    // compare window starting at mid with window starting at mid+1
    if (/* window at mid is not better than at mid+1 */) lo = mid + 1;
    else hi = mid;
}
// lo is the best start
```

The search space is the set of `n - k + 1` possible window positions, not the elements. Time is O(log(n - k)) comparisons. Binary search works only if the comparison is consistent across the sorted order (the answer to "is `mid + 1` better?" changes from yes to no at most once).

---

## 4. Combining Techniques

```text
Variable window (3.2)  +  Frequency map (3.3)   ->  windows constrained by repeated / distinct values
Variable window (3.2)  +  Match counter (3.4)   ->  windows that must contain a required multiset
Fixed window (3.1)     +  Frequency array (3.3) ->  windows compared by exact content
Fixed window (3.1)     +  Monotonic deque (3.5) ->  extremum of every window in O(n)
```

The general relationship: **the window mechanism (fixed or variable) decides which elements are inside**, while the **state structure decides what you know about them**. The two are independent design choices. Changing the question you ask of a window usually means changing the state structure, not the pointer movement. Similarly, the monotonic deque is simply another kind of state, one specialized for extrema.

---

## 5. Complexity & Tradeoffs

| Technique | Time | Extra space |
| --- | --- | --- |
| Naive recomputation of every window | O(n · k) or O(n²) | O(1) to O(k) |
| Fixed window, O(1) state update | O(n) | O(1) for sums; O(alphabet) for counts |
| Variable window (expand/shrink) | O(n) amortized | depends on state |
| Frequency map (`unordered_map`) | O(1) average per op, O(n) worst case | O(distinct values in window) |
| Frequency array (fixed alphabet) | O(1) worst case per op | O(alphabet size) |
| Monotonic deque | O(n) amortized | O(k) |
| `multiset` for window extremum | O(n log k) | O(k) |
| Binary search on window start | O(log(n - k)) | O(1) |

Tradeoffs worth knowing:

- **Array vs `unordered_map`:** when the key set is small and known (e.g., 26 lowercase letters), an array is faster and has no hashing worst case.
- **Check cost matters:** the O(n) bound of a variable window assumes the validity check is O(1). A check that scans a whole map at each step (O(alphabet)) gives O(n · alphabet), which is acceptable for small alphabets but should be recognized as a different cost.
- **Deque vs ordered structures:** the deque gives O(n) but supports only the sliding-extremum pattern; ordered containers are more general and slower.

---

## 6. Important C++ Pitfalls

- **`size()` is unsigned.** `v.size() - k` wraps around to a huge number when `k > v.size()`. Cast first: `(int)v.size() - k`.
- **`unordered_map::operator[]` inserts.** Reading `cnt[x]` for a missing key creates it with value 0 and changes `size()`. Use `count()` or `find()` to test existence without inserting.
- **Zero-count entries.** After decrementing a count to 0 the key stays in the map. If you use `cnt.size()` as the distinct-value count, `erase` the key at 0.
- **Sum overflow.** Sums of many `int` values can exceed the `int` range; use `long long` for accumulators when the input could be large.
- **Window length and boundaries.** With inclusive indices the length is `right - left + 1`. Mixing inclusive and half-open conventions is the most common source of off-by-one errors.
- **Deque stores indices.** Comparing values requires `a[dq.back()]`, not `dq.back()`.
- **Empty containers.** Calling `front()`, `back()`, or `pop_front()` on an empty `deque` is undefined behavior; guard with `!dq.empty()`.
- **Comparing `char` arithmetic.** `c - 'a'` is an `int`; make sure the index is within `0..25` before using it with a size-26 array.
- **Windows larger than the input.** If `k` may exceed `n`, decide explicitly what should happen before entering a loop that assumes `k <= n`.

---

## 7. Recap

- A window is a contiguous range `[left, right]`; sliding means updating the window's **state** with one add and one remove instead of recomputing.
- **Fixed windows** have a given length; **variable windows** grow with `right` and shrink with `left` according to a validity condition.
- Both pointers only move forward, so total work is O(n) when state updates are O(1).
- The state can be a sum, a frequency map, a match counter, or a monotonic deque for extrema; the right choice depends on what must be known about the window.
- On sorted data, a fixed-length window position can itself be searched with binary search.
