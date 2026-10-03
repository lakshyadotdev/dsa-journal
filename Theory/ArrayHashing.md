# Arrays & Hashing: Theory Chapter

**Scope note.** The problem set mixes five kinds of knowledge: array/string manipulation in C++, hash tables (use and implementation), sorting (including non-comparison sorting and selection), prefix-based accumulation, and in-place/pointer-based techniques. The problem names are deliberately not mentioned below, and nothing here maps a technique to a problem.

---

## 1. Prerequisites

### Must know

**C++ basics**

- Loops, conditionals, functions, pass by value vs. by reference (`const vector<int>&`)
- Integer types: `int` range (~±2.1×10⁹), `long long`; integer division and `%`
- Lambdas (just the syntax `[](int a, int b){ return a < b; }`), used for custom sorting

**`vector<T>`**

- Declare and initialize: `vector<int> a;`, `vector<int> a(n);`, `vector<int> a(n, 0);`, `vector<int> a = {1,2,3};`
- 2D: `vector<vector<int>> g(r, vector<int>(c, 0));`
- `[]`, `size()`, `empty()`, `push_back()`, `pop_back()`, `back()`, `front()`, `clear()`
- Range-based `for` and index-based `for`
- `begin()` / `end()`, `sort(a.begin(), a.end())`, `swap(a[i], a[j])`

**`string`**

- `s[i]`, `size()`, `+`, `+=`, `push_back`, `substr(pos, len)`
- Comparison with `==`, `<` (lexicographic)
- Character arithmetic (`c - 'a'` gives 0–25), `to_string`, `stoi`

**Hash containers (usage level)**

- `unordered_set<T>`: `insert`, `count`/`find`, `erase`, `size`
- `unordered_map<K,V>`: `m[key]`, `insert`, `find`, `count`, `erase`, iterating with `for (auto& [k, v] : m)`

**Complexity**

- Big-O for loops, nested loops, and "do O(log n) work n times"
- Amortized vs. worst-case (you should know the words; section 2.2 explains them)

### Useful but not required (learned in this chapter)

- `priority_queue`, `nth_element`, `stable_sort`, `accumulate`, `iota`
- `std::map` / `std::set` (ordered containers)
- How a hash table works internally
- Merge sort / quicksort internals

---

## 2. Core Theory

### 2.1 Arrays and `vector` as a memory model

An array is a **contiguous block of memory**. Element `i` lives at `base + i·sizeof(T)`, so index access is O(1) (one address computation). Contiguity also makes arrays **cache-friendly**, which is why a plain array scan often beats "fancier" structures with the same Big-O.

`vector` is a dynamic array. It keeps a *capacity* ≥ *size*. When `push_back` exceeds capacity, it allocates a larger block (typically ×2), copies everything over, and frees the old block. That copy is O(n), but it happens rarely, so `push_back` is **amortized O(1)**: n pushes cost O(n) total.

| Operation | Cost |
| --- | --- |
| `a[i]`, `back()`, `push_back`, `pop_back` | O(1) (push amortized) |
| `insert`/`erase` in the middle or front | O(n) (shifts elements) |
| Copy a vector | O(n) |
| Search unsorted | O(n) |

Consequences worth internalizing:

- **Building a new array from an old one** is O(n) time and O(n) space, a trade you will make constantly.
- **Index arithmetic** is a tool. `i % n` wraps around, and `i / k` and `i % k` split an index into (group, offset). A 2D position `(r, c)` in an `R×C` grid flattens to `r*C + c`.
- **Iterator invalidation**: growing a vector may reallocate and invalidate pointers/iterators into it. In particular, `v.insert(v.end(), v.begin(), v.end())` (inserting a range of itself) is **undefined behavior**. Copy first or use index loops.
- `vector<int> b = a;` is a deep copy. `reserve(n)` pre-allocates capacity to avoid regrowth.

**Strings** are essentially `vector<char>` with extras. Two points that matter for complexity:

- `substr(pos, len)` costs O(len), and `+` or `==` between strings costs O(length).
- So "n strings of length L" means most whole-string operations cost O(L), not O(1). Keep this in mind in 2.2 and 2.3.

**Integer pitfalls**: sums, prefix sums, and products can overflow `int`. Use `long long` when values or counts can be large. `accumulate(v.begin(), v.end(), 0)` accumulates in `int`; use `0LL` for `long long`. In C++, `-7 % 3 == -1`, so wrapping a possibly negative index needs `((x % n) + n) % n`.

---

### 2.2 Hashing

**The problem it solves.** Searching an unsorted array is O(n). Searching a sorted array is O(log n). Hashing aims for **O(1) average** lookup, insertion, and deletion for arbitrary keys, by trading memory for time.

#### Direct-address tables

If keys are small integers in a known range `[0, K)`, just use an array of size K: `cnt[key]`. This is O(1) *worst case*, with no hashing or collisions. A fixed alphabet (26 letters, 256 byte values) is the classic case. Always prefer this when the key space is small and bounded. It's simpler and faster than `unordered_map`.

#### General hash tables

When the key space is huge (all ints, all strings), we can't have a slot per possible key. Instead:

1. A **hash function** `h(key)` maps a key to an integer.
2. Index = `h(key) mod m`, where `m` is the number of buckets.
3. Store the entry at that bucket.

Two keys mapping to the same bucket is a **collision**. Collisions are *inevitable* when keys outnumber buckets (pigeonhole), so every hash table needs a collision strategy.

**Separate chaining.** Each bucket holds a small list (or vector) of entries.

- *Insert*: compute the bucket, scan its chain for the key (update if found), otherwise append.
- *Lookup/delete*: compute the bucket, scan the chain.
- Cost per operation is O(1 + chain length). The **load factor** α = n/m is the expected chain length under a good hash function.

**Open addressing.** All entries live in the table array itself. On collision, *probe* for another slot using a deterministic sequence (linear: `i+1, i+2, …`; quadratic; double hashing).

- Lookup follows the same probe sequence until it finds the key or an empty slot.
- **Deletion is subtle**: simply emptying a slot would break probe sequences of other keys that probed past it. The standard fix is a *tombstone* marker ("deleted, but keep probing past me").
- It's more cache-friendly than chaining, but performance degrades sharply as α → 1.

**Load factor and rehashing.** To keep α bounded (e.g., ≤ 1 for chaining, ≤ ~0.5–0.7 for open addressing), when α crosses a threshold the table allocates a larger bucket array (typically ×2) and **re-inserts every entry** (the bucket index depends on `m`, so everything moves). Same amortization argument as `vector`: O(n) occasionally, O(1) amortized per insert.

**Complexity summary.** *Expected/average* O(1) for insert, lookup, and delete. *Worst case* O(n) if everything collides, which a decent hash function makes astronomically unlikely on non-adversarial input. When you design a table yourself, the main design decisions are: bucket count, hash function (e.g., `key % prime`), collision strategy, and the resize policy.

**Key cost caveat.** Computing `h(key)` for a string costs O(L). So hash operations on strings are O(L), not O(1). A set of n strings of length L costs O(n·L) total to build.

#### C++ containers

| Container | Structure | Ops | Order | Key requirement |
| --- | --- | --- | --- | --- |
| `unordered_set/map` | hash table | avg O(1), worst O(n) | none | hash + `==` |
| `set/map` | balanced BST | O(log n) always | sorted | `<` |

- `unordered_map` has built-in hashes for `int`, `char`, `string`, etc. but **not** for `pair`, `vector`, or `tuple`. For those, either use ordered `map`/`set` (which only needs `<`, and `pair`/`vector` have it), or *encode the key into a string/integer*, or write a custom hash.
- **`m[key]` inserts a default value if the key is missing.** This is convenient for counting (`m[x]++`) but a classic bug source when you only wanted to *check*. To check without inserting, use `m.find(k) != m.end()` or `m.count(k)`.
- Iteration order of an unordered container is unspecified. Don't rely on it.
- Don't insert or erase while iterating unless you know the rules (`erase` returns the next iterator).
- Ordered containers give you things hashes can't: sorted iteration, `lower_bound`, min/max. If you need order, you pay O(log n).

---

### 2.3 Sorting

**What sorting buys you.** Sorting costs O(n log n) but turns global questions into *local* ones:

- Equal elements become adjacent, so duplicates can be found by comparing neighbors.
- Extremes are at the ends, and binary search becomes possible.
- Linear scans can then replace nested loops.

Sorting is therefore the main alternative to hashing: **O(n log n) time with little extra memory vs. O(n) expected time with O(n) extra memory.** It also destroys original positions. If you need them, sort `(value, index)` pairs.

**Lower bound.** Any comparison-based sort needs Ω(n log n) comparisons. A decision tree distinguishing n! permutations needs height ≥ log₂(n!) ≈ n log n. Beating it requires *not* comparing, which means exploiting key structure (2.3.3).

#### 2.3.1 The comparison sorts you should be able to write

**Merge sort** (divide and conquer): split in half, sort each half recursively, **merge** two sorted halves with two pointers.

- Time O(n log n) in *all* cases (log n levels, O(n) merge work per level).
- Space O(n) for the merge buffer. **Stable** (equal elements keep their relative order, as long as the merge takes from the left half on ties).

**Quicksort**: choose a *pivot*, **partition** so that smaller elements go left and larger go right, then recurse on both sides.

- Average O(n log n), in place (O(log n) stack). *Worst case* O(n²) when pivots are consistently extreme (e.g., already-sorted input with a first-element pivot). Mitigation: **random pivot** or median-of-three.
- Many duplicates degrade naive partitioning. **Three-way partitioning** (`< pivot | == pivot | > pivot`) fixes this.
- Not stable.

**Heap sort**: build a max-heap (O(n)), then repeatedly extract the max (n × O(log n)). O(n log n) worst case, O(1) extra space, not stable. It is rarely the fastest in practice, but its worst-case guarantee and O(1) space are useful.

**Stability** matters when sorting by one key after another, or sorting records.

**In practice**, `std::sort` is introsort (quicksort + heapsort fallback + insertion sort for small ranges): O(n log n) worst case. `std::stable_sort` is merge-based.

**Comparators**: must define a *strict weak ordering* (irreflexive: `comp(a,a)` is false). Using `<=` in a comparator can crash or cause undefined behavior. Examples: `sort(v.begin(), v.end(), greater<int>())`, or a lambda comparing a field.

#### 2.3.2 Partitioning as a primitive

Partition is the engine inside quicksort, and it's useful alone. Rearranging an array so elements fall into a few *classes* can be done in O(n) with O(1) space, without sorting, by maintaining **regions with invariants**. For example, with three classes (A, B, C) keep pointers `lo`, `mid`, `hi` such that:

- `[0, lo)` are all A
- `[lo, mid)` are all B
- `(hi, end)` are all C
- `[mid, hi]` is *unexamined*

Each step examines `a[mid]`, puts it in the right region by swapping, and shrinks the unexamined region. **Subtle point:** an element swapped in from the unexamined region has not been classified yet, whereas one swapped in from a classified region has. Track what you know about each region at all times; this "state the invariant" habit prevents off-by-one bugs.

#### 2.3.3 Non-comparison sorting: counting and bucketing

If keys are integers in a bounded range `[0, K)`:

- **Counting sort**: count occurrences (`cnt[k]++`), then emit each key `cnt[k]` times, or use prefix sums of counts to place records stably. Time O(n + K), space O(K).
- **Bucket sort**: distribute items into buckets by key, then process buckets in order.

This beats O(n log n) *when K is small relative to n*, and it applies whenever the thing you want to order by is an integer with a known bound. (Observe: a count of occurrences can never exceed n.)

---

### 2.4 Selection and heaps (the "top k" family)

If you only need the k best items, a full sort (O(n log n)) is more than necessary.

**Heap / `priority_queue`.** A binary heap stores a complete binary tree in an array with the invariant "parent ≥ children" (max-heap). `push`/`pop` are O(log n), `top` is O(1), and building a heap from n items is O(n). In C++, `priority_queue<T>` is a **max-heap**. For a min-heap, use `priority_queue<T, vector<T>, greater<T>>`. Elements are often `pair<count, item>`; pairs compare by first, then second.

**Top-k via a bounded heap.** Keep a *min-heap of size k* holding the best k seen so far. For each new item, push it, and if size > k, pop the smallest. The root is always the weakest member of the current top k. Time O(n log k), space O(k). It's good for streams and when k ≪ n.

**Quickselect.** Partition like quicksort but recurse into only the side containing the k-th position. Average O(n), worst O(n²) (random pivots make it unlikely). `nth_element` does this.

**Bucketing by key** (from 2.3.3) gives O(n) when ranking by a bounded integer.

| Method | Time | Extra space |
| --- | --- | --- |
| Full sort | O(n log n) | O(1)–O(n) |
| Size-k heap | O(n log k) | O(k) |
| Quickselect | O(n) avg | O(1) |
| Bucket by bounded key | O(n) | O(n) |

---

### 2.5 Prefix sums

Given `a[0..n-1]`, define `P[0] = 0` and `P[i] = a[0] + … + a[i-1]` (array length n+1). Then:

> sum of `a[l..r]` = `P[r+1] − P[l]`

**Why it works:** `P[r+1]` is "everything up to r"; subtracting "everything before l" leaves exactly the range. Building costs O(n), and each query then costs O(1) instead of O(r−l). The leading `P[0] = 0` makes ranges starting at index 0 work without special-casing.

**Applicability:** the operation must be **invertible** (so you can "subtract off" the prefix). Sum, XOR, and count work. Min, max, gcd do not (no inverse), and product only does if no zeros and you accept division. It also assumes a **static** array; if values change, you need other structures (e.g., Fenwick trees, which come later) or O(n) rebuilds.

**2D prefix sums.** Let `P[i][j]` = sum of the sub-rectangle with corners `(0,0)` and `(i−1, j−1)`, using an `(R+1)×(C+1)` table with a zero first row and column.

- Build: `P[i][j] = a[i-1][j-1] + P[i-1][j] + P[i][j-1] − P[i-1][j-1]`. The overlap `P[i-1][j-1]` is counted twice by the other two terms, so we subtract it. This is **inclusion–exclusion**.
- Query rectangle `(r1,c1)` to `(r2,c2)` inclusive:
  `P[r2+1][c2+1] − P[r1][c2+1] − P[r2+1][c1] + P[r1][c1]`

Build costs O(R·C), and each query O(1).

---

## 3. Patterns & Techniques

### 3.1 Frequency counting

**Idea:** replace a sequence with a *multiset summary*, "how many times does each value occur", using a map or a direct-address array.
**State:** `count[value]`.
**Signals:** the order of elements doesn't matter, only *which* and *how many*. Or you need to compare whether two collections have the same contents, or find the most/least common element, or check duplicates.
**Variations:**

- *Increment/decrement* to test equality of two multisets with one table (or compare two tables directly).
- *Fixed alphabet* → array of size 26/256; *arbitrary keys* → hash map.
- A frequency table can itself be the input to the next step (sorting, bucketing, or a heap by count).

**Cost:** O(n) time, O(k) space for k distinct keys. **Mistake:** forgetting that `m[x]` inserts, or using a map for a tiny alphabet where an array suffices.

### 3.2 Remembering the past: membership and lookup-by-complement

**Idea:** while scanning left to right, store what you have already seen so that for each new element you can ask a question *about the past* in O(1).
**State:** a set (does it exist?) or a map (what is associated with it: index, count, first position?).
**Signal:** a nested loop whose inner loop is "does some earlier element satisfy a relation with this one?" If the relation can be *solved for the missing partner* (e.g., "what value would have to exist?"), the inner loop becomes a hash lookup.
**Cost:** O(n) time, O(n) space, vs. O(n²) time, O(1) space. This is the canonical space-for-time trade.
**Mistakes:**

- Order of operations: query *before* inserting the current element if an element must not pair with itself.
- Duplicates: decide what the map stores when keys repeat (first index? last? count?).
- Alternative without hashing: sort, then use two pointers from both ends (O(n log n), O(1) space; but destroys indices).

### 3.3 Canonical form (signature) as a key

**Idea:** to group or compare objects that are "equivalent" under some transformation, compute a **canonical representation** that is identical for all equivalent objects, then use it as a hash key.
**State:** `map<signature, group>`.
**Signal:** you need to bucket things by an equivalence (same contents regardless of order, same shape, etc.).
**Ways to build a signature:** sort the object; use a count vector (flatten into a string or tuple); use a normalized form.
**Cost:** signature cost × n. For n strings of length L, sorting each costs O(L log L), while counting over a fixed alphabet is O(L). Total O(n·L log L) vs. O(n·(L + alphabet)).
**Mistakes:** signatures that are *ambiguous* (e.g., concatenating counts without separators, so `1,11` and `11,1` collide), and keys that are not hashable in C++ (convert to a string).

### 3.4 Prefix and suffix accumulation

**Idea:** for each position, you need information about *everything to its left* and/or *everything to its right*. Precompute running aggregates once, then combine in O(1).
**State:** `pre[i]` (aggregate of elements before i), `suf[i]` (aggregate of elements after i), or one running variable updated during a pass.
**Signal:** a quantity at index i depends on "all the others" or on "everything before/after"; the naive solution recomputes an O(n) aggregate per index.
**Key insight:** if the operation is invertible, you can "total minus part"; if not (or if inverting is unsafe, such as dividing by zero), keep prefix and suffix **separately** and combine them.
**Space optimization:** you can often store one direction in the output array and carry the other as a single running variable during a second pass, giving O(1) extra space beyond the output.
**Cost:** O(n) time. **Mistakes:** off-by-one at the boundaries (what is the aggregate of an *empty* prefix? the identity: 0 for sum, 1 for product), and overflow.

### 3.5 Prefix sums + hashing (counting ranges with a target property)

**Idea:** a subarray sum is `P[j] − P[i]`. To count ranges whose sum equals `T`, for each `j` ask "how many earlier prefixes equal `P[j] − T`?" That is the "remember the past" pattern (3.2) applied to *prefix values*.
**State:** a running prefix value and a `map<prefix value, number of times seen>`, initialized with `{0: 1}` (the empty prefix).
**Why not a sliding window?** Windows that grow/shrink monotonically need a *monotone* property; with negative numbers, extending a range may decrease its sum. Prefix + hash has no such requirement.
**Variations:** store counts (how many?) or earliest index (longest?). Reduce prefixes modulo something for divisibility questions. Map a ±1 encoding of elements to "balance" questions.
**Cost:** O(n) time, O(n) space.

### 3.6 Candidate elimination (majority-type problems)

**Idea:** if an element occurs more than half the time, then *cancelling pairs of distinct elements* can never eliminate it, because each cancellation removes at most one copy of it along with one non-copy. So after cancelling everything cancellable, the survivor is the only possible majority.
**State:** `(candidate, counter)`. If the counter is 0, adopt the current element; if equal to the candidate, increment; otherwise decrement.
**Important:** the survivor is only a *candidate*. If a majority is not guaranteed to exist, **verify** with a second counting pass.
**Generalization:** at most `k` distinct values can have frequency > n/(k+1) (pigeonhole). Maintain up to `k` candidates with counters; a "cancellation" removes `k+1` distinct elements at once. Space O(k), time O(n), then verify.
**Alternatives and trade-offs:** hash counts (O(n) space, simple, no verification trick), sort and inspect the middle (O(n log n); for "more than n/2" the median is the majority).

### 3.7 In-place modification: read/write pointers

**Idea:** rewrite an array into its result *inside itself* using two indices: `r` scans every element, `w` marks where the next kept element should be written. The write index never overtakes the read index, so unprocessed data is never overwritten.
**State:** `w` plus the invariant "`a[0..w)` is the finished output".
**Signal:** filtering, deduplicating, compacting, or removing elements while minimizing extra space.
**Variations:**

- *Order-preserving compaction* (copy kept elements forward).
- *Swap-based* when order doesn't matter: swap with the end and shrink.
- *Partition* (2.3.2), where invariants describe several regions.
**Cost:** O(n) time, O(1) space. **Mistakes:** forgetting the new logical length (return `w`); using `erase` in a loop (O(n²)); failing to state the invariant.

### 3.8 Bucketing by bounded keys

**Idea:** when a value you would sort by is a bounded integer, index an array by it. Use `bucket[key]` as a list. Iterate buckets in order to get sorted output (or top-k) without comparisons.
**Signal:** the sorting key is a count, a small value range, or a position. The bound is known and ≈ O(n).
**Cost:** O(n + K) time and space. See 2.3.3 and 2.4.

### 3.9 Index-as-hash (using the array as its own table)

**Idea:** if values lie in `[1..n]` and the array has n slots, then value `v` has a natural home at index `v−1`. The array can serve as a direct-address table for itself, giving O(1) extra space.
**Techniques:**

- **Cyclic placement:** repeatedly swap `a[i]` into its home slot until the element is out of range or already home. Each swap permanently fixes one element, so total work is O(n).
- **Sign marking:** negate `a[v−1]` to record "v seen", which requires values to be non-negative first (so pre-process out-of-range values).
**Key reasoning:** among `n` slots, if every value in `[1..n]` is present the answer is "n+1"; otherwise some slot is wrong, which tells you what's missing. Values outside `[1..n]` are irrelevant and can be ignored or neutralized.
**Mistakes:** infinite loops with duplicates (guard by checking the destination already holds the right value); reading a slot after you have overwritten or negated it.
**Trade-off:** O(1) space but mutates the input and is trickier to get right than a hash set.

### 3.10 Entry-point detection (avoiding redundant expansion)

**Idea:** if elements form *chains* (each may link to a successor) and you expand a chain from every element, you redo work: a chain of length m gets walked m times, giving O(n²). Fix: only start expanding from elements that are **true starts** (no predecessor in the data). Then each chain is walked exactly once, so total work is O(n) with a hash set for O(1) "does x exist?".
**Alternative:** sort + one linear scan (O(n log n), O(1) extra space; must handle duplicates).
**General principle:** when the cost lies in repeated overlapping work, identify a canonical starting point or an ownership rule so each piece of work is done once.

### 3.11 Constraint validation with "seen" sets and group indexing

**Idea:** to verify "no duplicates within each group", keep a seen-set per group and check/insert in a single pass.
**Index arithmetic for groups on a grid:** rows are `r`, columns are `c`, and an `B×B` block has id `(r / B) * (C / B) + (c / B)` (integer division). Diagonals are identified by `r − c` and `r + c`. Choose the group-id formula first, then the checking loop becomes trivial.
**Implementation choices:** `unordered_set` per group, boolean arrays of size (groups × values), or bitmasks (one integer per group, bit v = "seen v").
**Cost:** O(cells) time, O(groups × distinct values) space.

### 3.12 One-pass running state, and decomposing totals

**Idea:** maintain a few variables updated each step (running min, running max, best so far, running count) instead of storing history.
**Decomposition:** a net change over a range equals the sum of its step-by-step changes (telescoping: `(a₂−a₁)+(a₃−a₂) = a₃−a₁`). When you can freely choose *which steps to include* and steps don't interfere, the best total is obtained by independently taking each beneficial step. In general, check whether the problem splits into independent local choices; this is the core test for a **greedy** approach.
**Mistake:** assuming locally independent choices where they actually interact. Argue for independence (an exchange or decomposition argument), don't just assume it.

### 3.13 Unambiguous encoding (serialization)

**Idea:** to flatten a structure (e.g., a list of strings) into one string and recover it exactly, the format must be **self-delimiting**.

- **Delimiter approach:** fails if the payload itself can contain the delimiter, unless you *escape* it (and then escape the escape character).
- **Length-prefix approach:** write `length` + a marker + the raw data. When decoding, read the length (digits up to the marker), then take **exactly that many characters** without ever inspecting their contents. Arbitrary payloads (including digits, markers, and empty strings) are safe because you only trust the length.
**Principle:** an encoding is correct iff decode(encode(x)) = x for *all* valid x, including empty items and items containing your special characters. Test edge cases deliberately.
**Cost:** O(total characters) for both directions.

### 3.14 Designing a data structure from an interface

**Idea:** implement `insert / contains / remove` (and `get / put` for maps) yourself. The relevant theory is 2.2: pick a bucket count, a hash (`key % m`; use a prime or a power-of-two-aware variant), a collision strategy (chaining via `vector<list<pair<K,V>>>` or vectors of pairs), correct *update-on-existing-key* semantics, and (optionally) a rehash when load factor exceeds a threshold. When the key range is small and known, a direct-address array is a legitimate and simplest design. Always define behavior for missing keys and duplicate inserts up front.

---

## 4. Important Relationships and Combinations

- **Hashing + prefix information:** prefixes turn range properties into *pairs of prefix values* satisfying an equation; hashing finds matching partners in O(1). (3.2 + 2.5/3.5)
- **Sorting vs. hashing:** the same problem often has both a sort-based solution (O(n log n), little memory, order destroyed) and a hash-based one (O(n), more memory, order irrelevant). Know both and the trade-off.
- **Sorting + two pointers:** sorted order gives the pointers a monotone structure, so moving one pointer predictably increases or decreases a quantity.
- **Frequency counting + selection:** count first (a hash map), then rank counts (heap or buckets). Count values are bounded by n, which enables bucketing.
- **Canonical form + hashing:** the signature is the key; building signatures is usually the dominant cost.
- **Frequency counting + candidate elimination:** the elimination idea is a *space-optimized* frequency count that only needs to know "who could possibly be frequent".
- **In-place + partitioning + counting:** the same classification task can be done by counting then rewriting, or by one-pass partitioning. Compare passes, simplicity, and invariants.
- **Index-as-hash + in-place:** both require careful invariants because you are overwriting the data you read.
- **Hashing + sets for structure detection:** O(1) existence queries make "neighbor exists?" checks cheap, enabling entry-point techniques.

---

## 5. Cross-Cutting Trade-offs

| You want… | Typical cost |
| --- | --- |
| Fast lookup of arbitrary keys | O(n) extra memory (hash table) |
| Minimal memory | Sort (O(n log n)) or in-place tricks (more complex logic) |
| Ordered results / range queries | `map`/sorting, O(log n) per op |
| Fixed small key space | Direct-address array, fastest of all |
| Many repeated range queries | Precompute prefix sums once (O(n)), then O(1) per query |
| Only top-k of n | Heap O(n log k) or quickselect O(n) avg, rather than a full sort |
| Preserve input | Copy (O(n) space) vs. mutate in place |

---

## 6. C++-Specific Pitfalls

- Overflow in sums/products/prefix sums: use `long long`; `accumulate` with `0LL`.
- `unordered_map::operator[]` inserts. Use `find`/`count` to merely check.
- No default hash for `pair`/`vector`: encode into a `string`, or use ordered containers.
- Comparators must be strict (`<`, never `<=`).
- Self-range `insert` into a vector is undefined behavior; `erase` in a loop is O(n²).
- Negative `%` results; integer division truncates toward zero.
- Passing large vectors by value copies them; pass `const&`.
- `size()` returns an unsigned type: `a.size() - 1` underflows on empty vectors. Cast to `int` when subtracting.
- Don't depend on unordered container iteration order.

---

## 7. What You Should Now Understand

- How `vector` and strings behave in memory, and the real cost of each operation (including string-length factors).
- How a hash table works (hash function, collisions, chaining vs. open addressing, load factor, rehashing, tombstones), when to use a plain array instead, and `unordered_*` vs. ordered containers.
- Comparison sorting (merge, quick, heap), stability, the Ω(n log n) bound, and when counting/bucket sort beats it.
- Selection: bounded heaps, quickselect, bucketing by bounded keys.
- 1D and 2D prefix sums, when they apply (invertibility, static data), and how to derive range queries.
- The patterns: frequency counting, remembering the past, canonical-form keys, prefix/suffix accumulation, prefix + hash, candidate elimination (and its k-candidate generalization), read/write pointers and partitioning, bucketing, index-as-hash, entry-point detection, seen-set validation with group indexing, running state/decomposition, self-delimiting encodings, and data-structure design.
- For any problem, the questions to ask are: *What information do I need about the past or future? What repeats? Does order matter? Is the key space bounded? Can I trade memory for time (or the reverse)? What invariant am I maintaining?*
