# Two Pointers: Theory Chapter

---

## 1. Prerequisites

### Must Know

**C++ basics**

- `vector<int>`, `vector<vector<int>>`, `string`: declaration, indexing with `[]`, `size()`, `push_back()`, `back()`
- `string`: `+=` with a `char`, `push_back()`, `substr()`, constructing an empty string
- Range-based `for` and index-based `for` loops
- `while` loops with compound conditions (`while (l < r && ...)`)
- Passing vectors by reference (`vector<int>& nums`, `const string& s`) vs. by value
- Writing small helper functions that take indices as parameters
- `swap(a, b)`
- `sort(v.begin(), v.end())` (ascending, O(n log n))
- `reverse(v.begin(), v.end())` and `reverse(s.begin() + a, s.begin() + b)` (the range is half-open: `[a, b)`)
- `max()` and `min()` from `<algorithm>`
- `isalnum()`, `tolower()` from `<cctype>`
- The `%` operator, including the case `k > n`
- Integer types: `int` vs. `long long`, overflow awareness, and casting inside an expression: `(long long)a + b`

**Concepts**

- Big-O notation (O(1), O(n), O(n log n), O(n²))
- Zero-based indexing and valid index range `[0, n-1]`
- "In-place" means modifying the input using O(1) extra memory
- A sorted array is "non-decreasing" (equal neighbors are allowed)

### Useful but Not Required

- `unordered_set` / `unordered_map` (for comparison with non-pointer approaches)
- Iterators (`begin()`, `end()`) and `erase`
- Loop invariants as a formal idea (introduced informally here)
- Linked list nodes and `->` access (the idea transfers, but this chapter uses arrays and strings)

---

## 2. Core Concepts

### 2.1 What is a "pointer" here?

A **pointer** is any variable that stores a **position** in a sequence, usually an integer index. It is not necessarily a C++ pointer (`int*`).

```cpp
int l = 0;                   // position of the left element
int r = (int)nums.size()-1;  // position of the right element
```

The **two pointers technique** processes one or more sequences by maintaining two (or a few) positions and moving them by a rule, so that each pointer travels through its sequence **at most once**.

### 2.2 Why it works: eliminating work

Brute force considers every pair of positions `(i, j)`, which is O(n²) pairs. Two pointers applies when the structure of the data lets one comparison **rule out many pairs at once**.

Take a sorted array `a` and the question "is `a[l] + a[r] < k`?" Suppose it is, with `l < r`. Every element between `l` and `r` is at most `a[r]`, so `a[l] + a[x] ≤ a[l] + a[r] < k` for every `x` in `(l, r]`. One comparison settles `r - l` pairs without visiting them.

> **Core idea:** each pointer move is justified by a fact showing the skipped positions cannot matter. If you cannot justify a move this way, the technique is not valid there.

### 2.3 Loop invariants (informal)

An **invariant** is a statement that is true before every iteration. Examples:

- "Everything left of `l` has already been processed."
- "The answer, if it exists, lies within `[l, r]`."
- "`nums[0..write-1]` is the cleaned output so far."

If you can state an invariant and show each move preserves it, correctness follows. When there is a bug, the invariant is usually where the reasoning went wrong.

### 2.4 Why it is efficient

Each iteration moves at least one pointer, and pointers never move backward. Total moves are bounded by the sequence length, so the traversal is **O(n)**. Sorting, if needed, adds O(n log n).

---

## 3. Patterns & General Techniques

### Technique 1: Opposite-Direction (Converging) Pointers

**What it is:** One pointer starts at the left end, one at the right end, and they move toward each other until they meet or cross.

```text
index:   0   1   2   3   4   5   6
         ↑                       ↑
         l                       r
         →→→                 ←←←
```

**How it works:**

1. Initialize `l = 0`, `r = n-1`.
2. Loop while `l < r`.
3. Evaluate something about `(l, r)`.
4. Move `l` right, `r` left, or both.

**Underlying idea:** Each step discards at least one end of the region `[l, r]`, because a comparison proves the discarded position cannot be part of a better or valid answer (§2.2).

**Two flavors:**

- *Symmetric processing:* both pointers compare or swap mirrored positions and move every step.
- *Decision-driven:* which pointer moves depends on a comparison, relying on sorted order or on a quantity that changes monotonically as pointers move.

```cpp
// Symmetric: reverse in place
void reverseArr(vector<int>& a) {
    int l = 0, r = (int)a.size() - 1;
    while (l < r) {
        swap(a[l], a[r]);
        ++l; --r;
    }
}
```

```cpp
// Decision-driven skeleton
int l = 0, r = (int)a.size() - 1;
while (l < r) {
    // examine a[l], a[r]
    if (/* left end is provably useless */) ++l;
    else                                    --r;
}
```

**Complexity:** Time O(n), space O(1).

---

### Technique 2: Same-Direction Pointers (Read/Write and Fast/Slow)

**Variant A: read/write pointers (in-place filtering).**
`read` scans every element. `write` marks where the next kept element goes.

```cpp
int write = 0;
for (int read = 0; read < (int)a.size(); ++read) {
    if (a[read] != 0) {        // condition for "keep"
        a[write] = a[read];
        ++write;
    }
}
// a[0..write-1] holds the kept elements; write is the new length
```

*Invariant:* `a[0..write-1]` contains exactly the kept elements seen so far, in order. Since `write ≤ read`, overwriting never destroys unread data.

**Variant B: fast/slow pointers (optional reading).**
`slow` moves one step and `fast` two per iteration. Their relative distance carries information. This is most prominent on linked structures.

```cpp
int slow = 0, fast = 0;
while (/* fast can advance */) { slow += 1; fast += 2; }
// when fast reaches the end, slow is near the middle
```

**Complexity:** Time O(n), space O(1).

---

### Technique 3: One Pointer per Sequence (Merge-Style Traversal)

**What it is:** The input is **two** (or more) sequences. Each gets its own pointer, and a third index may mark where output is written.

```text
a:  [ 1, 4, 7 ]      b:  [ 2, 3, 9, 10 ]
      i →                  j →
out: [ ... ]  ← k
```

**How it works:**

1. Keep `i` over `a`, `j` over `b`, `k` for the destination.
2. While **both** have elements remaining, choose one by a rule, write it, advance only that pointer (and `k`).
3. When one sequence runs out, the other may have **leftovers**. Handle them with a second loop.

```cpp
vector<int> out;
int i = 0, j = 0;
while (i < (int)a.size() && j < (int)b.size()) {
    if (a[i] <= b[j]) out.push_back(a[i++]);
    else              out.push_back(b[j++]);
}
while (i < (int)a.size()) out.push_back(a[i++]);  // leftovers of a
while (j < (int)b.size()) out.push_back(b[j++]);  // leftovers of b
```

The choice rule can be anything ("take the smaller", "alternate"). The structure (loop while both remain, then drain leftovers) stays the same.

**Writing into a buffer in place.** If the destination shares storage with an input, write order matters. A write is **safe** if it never lands on a position not yet read. Choose the direction in which the write position moves through space that is already free or already consumed, and state this as an invariant before coding.

**Complexity:** Time O(n + m). Space O(1) extra if in place, O(n + m) if building a new container.

---

### Technique 4: Sort First, Then Use Pointers

**What it is:** If the original order does not matter for the question, sorting can **create** the structure that justifies pointer movement.

**How it works:** After sorting, moving a pointer right never decreases the value it points to, and moving left never increases it. Each pointer move then has a predictable effect on any sum or comparison involving its element.

```cpp
sort(a.begin(), a.end());   // O(n log n)
int l = 0, r = (int)a.size() - 1;
// a[l] only grows as l moves right; a[r] only shrinks as r moves left
```

**Tradeoff:** Sorting costs O(n log n) and destroys original order and indices. If positions are needed, store them first:

```cpp
vector<pair<int,int>> p;                 // {value, original index}
for (int i = 0; i < n; ++i) p.push_back({a[i], i});
sort(p.begin(), p.end());                // by value, then index
```

**Complexity:** O(n log n) overall (sort dominates). Space O(1) extra, or O(n) if indices are stored.

---

### Technique 5: Fixing Elements, Pointers on the Rest (k-Element Search)

**What it is:** To search for a *k*-element combination, fix elements with outer loops and run a converging scan on the remaining range. Each fixed element reduces the search by one dimension.

```text
sort
for i:                      // fix 1st
    for j > i:              // fix 2nd (only for k ≥ 4)
        l = j+1, r = n-1    // converging scan for the rest
```

```cpp
sort(a.begin(), a.end());
for (int i = 0; i < (int)a.size(); ++i) {
    int l = i + 1, r = (int)a.size() - 1;
    while (l < r) {
        // use a[i], a[l], a[r]; move l or r
    }
}
```

| Elements | Structure | Time |
| --- | --- | --- |
| 2 | one scan | O(n) after sort |
| 3 | 1 loop + scan | O(n²) |
| 4 | 2 loops + scan | O(n³) |
| k | (k−2) loops + scan | O(n^(k−1)) |

**Details for larger k:**

- **Duplicate skipping (Technique 6) must be applied at every fixed level**, not only in the innermost scan.
- **Overflow risk grows with k.** Cast *before* adding: `(long long)a[i] + a[j] + a[l] + a[r]`. Casting the final sum is too late.
- **Early pruning** (optional): in a sorted array, if the smallest possible total from the current position already exceeds the target (with non-negative values), later positions cannot succeed. This improves practical speed, not worst-case complexity.

---

### Technique 6: Skipping Duplicates

**What it is:** When the input has repeated values and the output must not repeat results, skip equal neighbors after processing a value.

**Why it works:** In a sorted array, equal values are adjacent. After handling one occurrence, advancing past all equal neighbors guarantees the same value is never used in the same role twice.

```cpp
// Outer loop index
for (int i = 0; i < n; ++i) {
    if (i > 0 && a[i] == a[i-1]) continue;
    // ... process a[i]
}

// Moving pointer
++l;
while (l < r && a[l] == a[l-1]) ++l;
```

**Details:**

- Compare with the **previous** element (`a[i-1]`) so the first occurrence is still processed.
- Keep the bounds check (`l < r`) *before* array access.
- Requires equal values to be adjacent, so it relies on sorting (Technique 4).

**Complexity:** No added asymptotic cost; pointers still only move forward.

---

### Technique 7: Accumulated State and Prefix/Suffix Summaries

**What it is:** Maintain extra information about the region each pointer has already passed (running maximum, minimum, sum, or best-so-far answer).

**Underlying idea:** The pointers tell you *where* you are. The state tells you *what you know* about what you have covered. Decisions use both.

**On-the-fly version (O(1) space):**

```cpp
int l = 0, r = (int)a.size() - 1;
int leftBest = 0, rightBest = 0, answer = 0;
while (l < r) {
    leftBest  = max(leftBest,  a[l]);   // summary of a[0..l]
    rightBest = max(rightBest, a[r]);   // summary of a[r..n-1]
    // choose which pointer to move using the summaries,
    // update 'answer', then advance that pointer
    if (leftBest < rightBest) ++l; else --r;
}
```

**Precomputed version (O(n) space):** compute the summary for every position first.

```cpp
vector<int> prefMax(n), sufMax(n);
prefMax[0] = a[0];
for (int i = 1; i < n; ++i)     prefMax[i] = max(prefMax[i-1], a[i]);
sufMax[n-1] = a[n-1];
for (int i = n-2; i >= 0; --i)  sufMax[i]  = max(sufMax[i+1],  a[i]);
// prefMax[i] = max of a[0..i];  sufMax[i] = max of a[i..n-1]
```

**Tradeoff:** The precomputed version is easier to reason about because each position's information is explicit. The on-the-fly version reduces space to O(1) once you can justify the pointer moves (Technique 8).

**Tracking a best answer:** many scans maintain `best = max(best, candidate)` at each step. The pointers drive the traversal and `best` records the extreme value seen.

**Complexity:** O(n) time. Space O(1) on-the-fly, O(n) precomputed.

---

### Technique 8: Justifying Moves for Functions of Two Positions

Many scans optimize a value `f(l, r)` that depends on **both** positions, not just a sum. Moving a pointer is valid only if the pairs you skip are **no better** than a pair you keep.

**Method:**

1. Write `f(l, r)` explicitly. Identify what changes as `r - l` shrinks and what changes as individual elements change.
2. Pick the move you want, say discarding position `l`.
3. Show that for **every** partner `x` in the remaining range, `f(l, x) ≤` some pair already evaluated or still to be evaluated.
4. If step 3 holds, discarding `l` loses nothing. If it does not, the move is invalid.

This is the same logic as §2.2, applied to quantities where the comparison is not a simple inequality on a sum.

---

### Technique 9: Filtering on the Fly

**What it is:** When some elements should be ignored (for example, non-alphanumeric characters), the pointers **skip** irrelevant elements inside the loop instead of building a cleaned copy.

```cpp
while (l < r) {
    while (l < r && !isalnum((unsigned char)s[l])) ++l;
    while (l < r && !isalnum((unsigned char)s[r])) --r;
    // both s[l], s[r] are now relevant: compare tolower(...) of each
    ++l; --r;
}
```

**Tradeoff:** A cleaned copy is simpler but uses O(n) space. Skipping in place uses O(1) space but needs careful bounds checks.

---

### Technique 10: Index-Parameterized Helpers

**What it is:** Write a helper that processes only the range `[l, r]`, with `l` and `r` as parameters, so the main loop can query **different sub-ranges** without copying.

```cpp
bool check(const string& s, int l, int r) {
    while (l < r) {
        if (s[l] != s[r]) return false;
        ++l; --r;
    }
    return true;
}
```

**Why it is useful:**

- No copying: `const string&` plus two indices is O(1), while `substr` costs O(length).
- It lets the main scan **branch**: when one comparison leaves more than one valid next step, each option can be tested by calling the helper on the remaining range.
- **Cost rule:** if the main scan branches a bounded number of times and each branch costs one linear scan, total time stays O(n). If it could branch at every step, cost multiplies. Count the branch points.

**Complexity:** O(n) per helper call, O(1) space.

---

### Technique 11: Reversal as a Building Block, and Index Arithmetic

**Reversal of a subrange.** Reversing `[a, b)` is a converging-pointer operation (Technique 1) and is available as `std::reverse`.

```cpp
vector<int> v = {1, 2, 3, 4, 5};
reverse(v.begin(), v.end());         // {5, 4, 3, 2, 1}
reverse(v.begin(), v.begin() + 2);   // reverses indices 0 and 1 only
```

**Composition of reversals.** Reversing a concatenation of two blocks reverses the block order **and** the contents of each block:

```text
[ X | Y ]  --reverse whole-->  [ Yʳ | Xʳ ]
```

Reversing each block afterward undoes the internal reversal, leaving the blocks swapped. A few reversals of well-chosen subranges can express a rearrangement at O(length) cost each, with O(1) extra space.

**Modular index arithmetic.** Shifting position `i` by `k` with wraparound gives `(i + k) % n`.

- `k` may exceed `n`, so reduce it first with `k %= n`. Shifting by `n` returns everything to its place.
- `n == 0` makes `% n` undefined. Guard against it.

```cpp
int n = v.size();
k %= n;                     // k in [0, n-1]
int dest = (i + k) % n;     // wrapped destination index
```

**Complexity:** Reversal-based rearrangement is O(n) time, O(1) space. A copy-based approach using `(i + k) % n` is O(n) time, O(n) space.

---

### Technique 12: Greedy Choice after Sorting

**What it is:** A **greedy** algorithm builds an answer by repeatedly making the choice that looks best now and never revisiting it. It is correct only if a locally best choice cannot hurt the final result.

**Exchange argument (the standard justification).** Suppose an optimal answer makes a different choice than the greedy one. Show you can **swap** that choice for the greedy one without making the answer worse. If this always works, greedy is optimal.

**How sorting helps:** it turns "best remaining element" into a position (the front or back). A pointer at each end gives O(1) access to both the smallest and largest remaining elements.

```cpp
// Neutral example: how many items fit if you take the cheapest first?
sort(cost.begin(), cost.end());
int count = 0;
long long total = 0;
for (int x : cost) {
    if (total + x > budget) break;
    total += x;
    ++count;
}
```

The justification: replacing any chosen item with a cheaper unchosen one never raises the total.

**Complexity:** O(n log n) for sorting, O(n) for the scan.

---

## 4. Combining Techniques

```text
Sorting (T4) + Converging pointers (T1)
     ↓
Order-aware pair search in O(n) after sorting
```

Sorting makes pointer moves predictable; converging pointers exploit that.

```text
Sorting (T4) + Fix elements (T5) + Converging pointers (T1) + Duplicate skipping (T6)
     ↓
Enumerate unique k-element combinations in O(n^(k-1))
```

Fixing reduces dimension, the scan finds partners efficiently, and duplicate skipping keeps results unique.

```text
Converging pointers (T1) + Accumulated state (T7)
     ↓
Decisions based on current positions and everything already passed, in O(1) space
```

```text
Move justification (T8) + Accumulated state or prefix/suffix arrays (T7)
     ↓
Optimizing a function of two positions in linear time
```

```text
Read/write pointers (T2) + Sorted order (T4)
     ↓
In-place deduplication or compaction in one pass
```

Equal values are adjacent, so one comparison with the last kept element suffices.

```text
One pointer per sequence (T3) + Sorted order
     ↓
Linear-time merge of ordered data in O(n + m)
```

```text
Index-parameterized helper (T10) + Converging pointers (T1)
     ↓
A scan that can resume on a sub-range after a decision point, still linear if branching is bounded
```

```text
Reversal of subranges (T11) + Modular index arithmetic
     ↓
In-place rearrangement with O(1) extra space
```

```text
Sort (T4) + Greedy choice (T12) + Pointers at both ends (T1)
     ↓
O(n log n) selection or pairing strategies
```

---

## 5. C++ Implementation Notes

- **Cast `size()`.** `v.size()` is unsigned. Write `int r = (int)v.size() - 1;`, otherwise `size() - 1` on an empty vector wraps to a huge number.
- **Loop conditions:** `l < r` for pairs of distinct positions; `l <= r` when a single middle position should also be processed. Decide deliberately.
- **Sorting:** `sort(v.begin(), v.end());` ascending. Descending: `sort(v.rbegin(), v.rend());` or `greater<int>()`.
- **Collecting results:** `vector<vector<int>> res; res.push_back({x, y, z});`
- **Characters:** pass `(unsigned char)c` to `isalnum` and `tolower` to avoid undefined behavior with negative `char` values.
- **Strings:** prefer index parameters over `substr` inside loops, since `substr` copies.
- **Early `return` vs. tracking a result:** `return` for existence checks, a `best` variable for extreme-value scans.

---

## 6. Complexity & Tradeoffs

| Technique | Time | Extra Space | Notes |
| --- | --- | --- | --- |
| Converging pointers | O(n) | O(1) | Each move needs a justification |
| Read/write pointers | O(n) | O(1) | Modifies input in place |
| Fast/slow pointers | O(n) | O(1) | Relative speed carries information |
| One pointer per sequence | O(n + m) | O(1) in place / O(n + m) new | Must drain leftovers |
| Sort + pointers | O(n log n) | O(1)–O(n) | Sorting dominates; loses original order |
| Fix elements + scan (k) | O(n^(k-1)) | O(1) | Versus O(n^k) brute force |
| Duplicate skipping | no added cost | O(1) | Needs adjacent equal values |
| State on the fly | O(n) | O(1) | Replaces auxiliary arrays |
| Prefix/suffix arrays | O(n) | O(n) | Simpler to reason about |
| Index-parameterized helper | O(n) per call | O(1) | Bounded branching keeps total linear |
| Reversal / modular shift | O(n) | O(1) / O(n) | Reversal in place; copy-based uses extra space |
| Sort + greedy | O(n log n) | O(1) | Needs an exchange argument |

**Key tradeoffs**

- **Two pointers vs. hashing:** hashing handles unsorted data in O(n) average time with O(n) memory. Pointers use O(1) space but typically need sorted order or monotonic structure, and sorting costs O(n log n).
- **In-place vs. copy:** in-place saves memory but needs more careful bounds reasoning. A copy is simpler but uses O(n) space.
- **On-the-fly state vs. precomputed arrays:** O(1) space but requires a valid move justification, versus O(n) space with simpler reasoning.
- **Average vs. worst case:** the pointer scans are always O(n). Variance comes only from the sorting step.

---

## 7. Important C++ Pitfalls

1. **Unsigned wraparound:** `v.size() - 1` on an empty vector is not −1. Cast to `int` first.
2. **Bounds check order:** write `while (l < r && a[l] == a[l-1])`, never the reverse.
3. **Off-by-one when fixing an element:** inner pointers start at `i + 1`, not `i`.
4. **Integer overflow:** sums of two or more `int` values can exceed `INT_MAX`. Use `long long`, and cast **before** the addition.
5. **Sorting destroys positions:** store original indices *before* sorting if needed.
6. **Infinite loops:** every branch must move at least one pointer. Verify this explicitly.
7. **Not re-checking the condition** after skip loops, particularly in duplicate and filtering logic.
8. **Draining leftovers:** forgetting the leftover loops in a two-sequence traversal silently drops data.
9. **Signed indices when counting down:** a back-to-front index must reach `-1`, so it must be a signed `int`. With `size_t`, `i >= 0` is always true and the loop never ends.
10. **`reverse` takes a half-open range:** `reverse(v.begin(), v.begin() + k)` touches indices `0..k-1`, not `k`.
11. **`k %= n` with `n == 0`** is undefined behavior. Check for an empty container first.
12. **`substr` copies:** inside a loop it can turn an O(n) scan into O(n²). Use index parameters.
13. **Duplicate skipping at every level** when k ≥ 3.

---

## 8. Recap

- Two pointers keeps a few positions and moves them monotonically, so total movement is O(n).
- Every move must be **justified**: skipped positions provably cannot affect the answer.
- The main shapes are **converging**, **read/write**, **fast/slow**, and **one pointer per sequence**.
- **Sorting** can create the structure that justifies moves, at O(n log n) cost.
- **Fixing elements** extends pair scans to larger combinations; **duplicate skipping** keeps results unique.
- **Accumulated state** and **prefix/suffix summaries** let a decision use information from regions already passed.
- **Index-parameterized helpers**, **reversal with modular arithmetic**, and **greedy choice after sorting** are supporting tools used with the pointer loop.
- State an **invariant** before writing the loop. It makes both the logic and the debugging easier.

---
