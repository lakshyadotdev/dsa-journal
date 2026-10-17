# Linked Lists: Theory Chapter

A standalone handbook chapter covering the concepts, data structures, and general techniques needed to work confidently with linked lists in C++.

## 1. Prerequisites

### Must Know

**C++ basics**

- `struct` definition with data members and a constructor
- Pointers: declaring `Node*`, `nullptr`, dereferencing with `->` and `*`, pointer assignment (copying an address, not the object)
- Dynamic allocation with `new` and release with `delete`
- Pass by value vs. pass by reference (`int&`, `Node*&`)
- Functions, loops (`while`, `for`), and recursion (base case, recursive case, the call stack)

**STL used in this chapter**

- `vector<T>`: declaration, `[]`, `size()`, `push_back()`, `pop_back()`, `back()`, `empty()`
- `unordered_map<K,V>`: declaration, `[]`, `count()`, `find()`, `erase()`
- `unordered_set<T>`: declaration, `insert()`, `count()`, `erase()`
- `list<T>` (doubly linked list): `push_front()`, `push_back()`, `pop_back()`, `front()`, `back()`, `begin()`, `end()`, `erase(iterator)`, `empty()`, `size()`
- `priority_queue<T>`: `push()`, `top()`, `pop()`, `empty()`, and the idea that the default is a max-heap
- `pair<A,B>`, `.first`, `.second`

**Concepts**

- Big-O notation for time and space
- Integer division and modulo (`/` and `%`)
- Sorting basics: what “sorted order” means and why sorted data can be combined efficiently

### Useful but Not Required

- Arrays and how they are laid out in contiguous memory
- What a binary heap is conceptually (the chapter only uses it through `priority_queue`)
- Lambda expressions in C++ (for custom comparators)
- Basic graph vocabulary (node, edge, directed)

---

## 2. Core Theory

### 2.1 What a linked list is

A **linked list** is a sequence of elements stored in separate **nodes**. Each node holds a value and a **pointer** to the next node. The list is accessed through a pointer to its first node, called the **head**. The last node’s `next` pointer is `nullptr`, which marks the end.

```text
head
 |
 v
[ 3 | * ]--->[ 7 | * ]--->[ 1 | * ]--->[ 9 | null ]
```

The nodes can live anywhere in memory. The only thing that connects them is the pointers. This is the key difference from an array, where elements sit side by side and position `i` is found by arithmetic.

```cpp
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};
```

An empty list is represented by `head == nullptr`. Every operation on a list must handle this case.

### 2.2 Traversal

To visit every node, start at the head and follow `next` until reaching `nullptr`.

```cpp
for (ListNode* cur = head; cur != nullptr; cur = cur->next) {
    // use cur->val
}
```

There is no way to jump to the i-th node directly. Reaching position `i` costs `i` steps.

### 2.3 Insertion and deletion

These operations are **pointer rewiring**. Given a pointer `p` to a node, inserting a new node after it:

```text
Before:  p ---> A ---> ...
After:   p ---> N ---> A ---> ...
```

```cpp
ListNode* n = new ListNode(5);
n->next = p->next;   // 1. new node points at the old successor
p->next = n;         // 2. predecessor points at the new node
```

**Order matters.** If step 2 were done first, the old successor would be lost.

Deleting the node after `p`:

```cpp
ListNode* victim = p->next;
p->next = victim->next;  // bypass it
delete victim;           // free memory (if you own it)
```

Both operations are O(1) **once you hold a pointer to the predecessor**. Finding that predecessor is the O(n) part. Notice that deleting or inserting at the head is a special case, because the head has no predecessor. Section 3.1 addresses this.

### 2.4 Linked list vs. array

| Operation | Array / `vector` | Singly linked list |
| --- | --- | --- |
| Access element at index `i` | O(1) | O(i) |
| Insert/delete at front | O(n) | O(1) |
| Insert/delete at known position | O(n) (shifting) | O(1) (rewiring) |
| Search for a value | O(n) | O(n) |
| Extra memory per element | none | one pointer |
| Memory locality | excellent | poor |

Linked lists trade random access for cheap structural changes.

### 2.5 Variants

**Doubly linked list.** Each node stores both `next` and `prev`.

```text
null <--[ 3 ]<-->[ 7 ]<-->[ 1 ]--> null
```

```cpp
struct DNode {
    int key, val;
    DNode* prev;
    DNode* next;
    DNode(int k, int v) : key(k), val(v), prev(nullptr), next(nullptr) {}
};
```

Because a node knows its predecessor, a node can be **removed in O(1) given only a pointer to that node**. The cost is one extra pointer per node and more pointers to keep consistent on every change.

**Circular linked list.** The last node’s `next` points back to the first node instead of `nullptr`. Traversal never reaches `nullptr`, so loops must use a different stopping condition (for example, returning to the start node).

**Cycles in general.** A list whose `next` pointers eventually revisit a node is said to contain a **cycle**. A naive `while (cur != nullptr)` loop never terminates on such a list.

**Extra pointers.** A node can hold additional pointers beyond `next` (for example, a pointer to an arbitrary other node in the same list). The structure is then no longer a simple chain, and copying or traversing it requires care about what each pointer refers to.

### 2.6 Lists as a sequence of digits or values

A list can represent any ordered sequence, including a number written one digit per node. The order of nodes (most significant first, or least significant first) is a representation choice that affects how arithmetic on it is done.

### 2.7 Memory and ownership in C++

- `new` allocates a node on the heap; it stays alive until `delete`.
- Copying a `ListNode*` copies only the address. Two pointers can refer to the same node.
- Local variables like `ListNode dummy(0);` live on the stack and are destroyed at function exit. Never return a pointer into one.
- In many exercise settings, nodes are provided and reused; rearranging them does not require `new`.

---

## 3. Patterns and General Techniques

### 3.1 Dummy (sentinel) node

**What it is.** A temporary extra node placed in front of the real head.

**What it does.** It guarantees that every real node, including the first, has a predecessor. This removes the special case for “modify the head”.

**How it works.** Create `dummy` with `dummy.next = head`. Operate on the list as usual using a pointer that starts at `&dummy`. At the end, the answer is `dummy.next`.

```text
dummy ---> [ 4 ] ---> [ 6 ] ---> [ 8 ] ---> null
   ^
   tail (starts here)
```

```cpp
ListNode dummy(0);
dummy.next = head;
ListNode* prev = &dummy;
// ... rewire using prev->next ...
return dummy.next;
```

A dummy is also useful when **building** a new list: keep a `tail` pointer starting at the dummy and append with `tail->next = node; tail = tail->next;`. No “is the list empty yet?” branch is needed.

**Complexity.** O(1) extra space, no change to time.

### 3.2 Pointer reversal

**What it is.** Flipping each `next` pointer so the list runs in the opposite direction.

**Underlying idea.** Walk forward with three pointers, redirecting one link at a time. Before overwriting `cur->next`, save the old successor, otherwise the rest of the list is lost.

```text
Before:  null   [1] -> [2] -> [3] -> null
          ^      ^
         prev   cur

Step:    save nxt = cur->next
         cur->next = prev
         prev = cur; cur = nxt

After:   null <- [1] <- [2] <- [3]
                               ^
                              prev   (cur == nullptr)
```

```cpp
ListNode* reverse(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* cur = head;
    while (cur != nullptr) {
        ListNode* nxt = cur->next;  // save
        cur->next = prev;           // flip
        prev = cur;                 // advance
        cur = nxt;
    }
    return prev;                    // new head
}
```

**Reversing only part of a list.** Reversal can be applied to a sub-range: locate the node just before the range, reverse the requested number of nodes, then **reconnect** the reversed block to the node before it and the node after it. The reconnection step is the main source of errors, so it is worth drawing the four involved pointers (node before, first of block, last of block, node after).

**Recursive form.** A list is either empty or a node followed by a smaller list. Reversal can be defined as: reverse the rest, then attach the current node at the end of the reversed rest. This uses O(n) call-stack space.

**Complexity.** Iterative: O(n) time, O(1) space. Recursive: O(n) time, O(n) space.

### 3.3 Fast and slow pointers

**What it is.** Two pointers traverse the same list at different speeds, typically `slow` moving one node per step and `fast` moving two.

**What it does.** Because the speeds differ, the pointers’ relative position changes in a predictable way, which yields information about the list without storing anything.

**Properties.**

1. When `fast` reaches the end, `slow` is at the middle (the second middle for even lengths if you stop when `fast == nullptr`; the first middle if you stop when `fast->next == nullptr`).
2. If the list contains a cycle, `fast` cannot reach `nullptr`, and once both pointers are inside the cycle, `fast` closes the gap on `slow` by one node per step, so they eventually meet.
3. After the meeting, the position of the cycle’s entry can be located by a second phase. Let `a` be the distance from the head to the cycle entry, and `c` the cycle length. At the meeting point, `slow` has travelled `d` steps and `fast` `2d`, so `d` is a multiple of `c`. Starting one pointer at the head and another at the meeting point, both moving one node per step, they meet at the cycle entry. This is known as **Floyd’s tortoise-and-hare algorithm**.

```cpp
ListNode* slow = head;
ListNode* fast = head;
while (fast != nullptr && fast->next != nullptr) {
    slow = slow->next;
    fast = fast->next->next;
    // compare slow and fast here when checking for cycles
}
```

**The idea beyond lists.** Any rule “from position `x`, go to position `f(x)`” defines a path that either ends or eventually repeats. For example, an array `a` of integers can be treated as a rule that sends index `i` to index `a[i]`. Following such a rule from a starting index produces the same kind of path as following `next` pointers, and cycle detection applies unchanged. In this setting there is no `nullptr`; the path is guaranteed to repeat because the values stay inside a finite set.

**Complexity.** O(n) time, O(1) space.

### 3.4 Gap (two-pointer offset) technique

**What it is.** Two pointers moving at the **same speed** but starting a fixed number of nodes apart.

**What it does.** It measures a distance from the end of a list in a single pass, without knowing the length in advance.

**How it works.** Advance the leading pointer `k` steps. Then move both together. When the leading pointer reaches the end, the trailing pointer is exactly `k` nodes behind it.

```text
k = 2
[1] -> [2] -> [3] -> [4] -> [5] -> null
 trail         lead                       (after advancing lead by 2)
                      trail         lead   (move together)
                              trail        lead = null
```

With a dummy node (3.1), the trailing pointer can be positioned at the **predecessor** of the target, which is what deletion requires.

```cpp
ListNode* lead = &dummy;
ListNode* trail = &dummy;
for (int i = 0; i < k; ++i) lead = lead->next;
while (lead->next != nullptr) {
    lead = lead->next;
    trail = trail->next;
}
```

**Complexity.** O(n) time, O(1) space. An alternative is two passes: one to count the length, one to walk to the target position. Both are O(n); the gap approach uses a single traversal.

### 3.5 Merging sorted sequences

**What it is.** Combining two sorted lists into one sorted list.

**Underlying idea.** The smallest remaining element of the combined result is always the smaller of the two current front elements. Repeatedly take the smaller front, append it to the output, and advance in that list. When one list is exhausted, attach the remainder of the other directly, since it is already sorted.

```text
A: 1 -> 4 -> 6          B: 2 -> 3 -> 9
out: 1 -> 2 -> 3 -> 4 -> 6 -> 9
```

```cpp
ListNode dummy(0);
ListNode* tail = &dummy;
while (a != nullptr && b != nullptr) {
    if (a->val <= b->val) { tail->next = a; a = a->next; }
    else                  { tail->next = b; b = b->next; }
    tail = tail->next;
}
tail->next = (a != nullptr) ? a : b;
```

Using `<=` keeps equal elements in their original relative order (a **stable** merge).

The same merge idea applies to **arrays** and to any two sorted streams. Merging relinks existing nodes, so no new nodes are required.

**Complexity.** O(n + m) time for lists of sizes n and m; O(1) extra space when relinking nodes.

### 3.6 Merging many sorted sequences

With `k` sorted lists, the smallest remaining element is the smallest of `k` current fronts. Two standard strategies:

**(a) Min-heap of fronts.** Store the current front node of each non-empty list in a min-heap. Repeatedly pop the smallest, append it to the output, and push its successor if it exists.

```cpp
auto cmp = [](ListNode* x, ListNode* y) { return x->val > y->val; };
priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);
```

The comparator is inverted (`>`) because `priority_queue` is a max-heap by default; `>` makes the smallest element come out on top.

With `N` total nodes, each node is pushed and popped once, each operation costing O(log k): **O(N log k)** time, O(k) extra space.

**(b) Repeated pairwise merging (divide and conquer).** Merge lists in pairs, then merge the results in pairs, and so on, until one list remains. There are about log k rounds and each round touches every node once: **O(N log k)** time, O(1) extra space for relinking (plus O(log k) if implemented recursively).

Merging lists one after another into a growing accumulator is correct but can cost O(N·k) in the worst case, because early elements are re-traversed many times.

### 3.7 Digit-by-digit construction with carry

**What it is.** Producing a result list one node at a time while carrying information forward from the previous step.

**How it works.** Process both inputs position by position. At each position compute `sum = a_digit + b_digit + carry`, create a node with `sum % 10`, and set `carry = sum / 10`. Continue while either input has nodes left **or** `carry != 0`. Treat a missing node as digit 0.

```cpp
int carry = 0;
while (a != nullptr || b != nullptr || carry != 0) {
    int sum = carry;
    if (a != nullptr) { sum += a->val; a = a->next; }
    if (b != nullptr) { sum += b->val; b = b->next; }
    tail->next = new ListNode(sum % 10);
    tail = tail->next;
    carry = sum / 10;
}
```

This is the same column-by-column addition done by hand. It requires that digits be processed from least significant to most significant. If a list stores the most significant digit first, it must be reversed or otherwise read in the opposite order first.

The general idea of a **running value carried between steps** applies to any left-to-right pass that produces output incrementally.

**Complexity.** O(max(n, m)) time and O(max(n, m)) space for the new list.

### 3.8 Hash-based node lookup

**What it is.** Using a hash table whose keys or values are **node pointers**.

Two general uses:

1. **Visited set.** `unordered_set<ListNode*>` records which nodes have been seen. A pointer equal to one already seen means the traversal has returned to an earlier node. Cost: O(n) extra space, but no pointer-speed reasoning is needed.
2. **Node-to-node map.** `unordered_map<Node*, Node*>` associates each original node with a counterpart, such as a copy. Once the mapping exists for every node, any pointer in the original structure can be translated to its counterpart with a single lookup.

```cpp
unordered_map<Node*, Node*> mp;
mp[orig] = new Node(orig->val);   // build the association
// later: translate any original pointer p
Node* counterpart = mp[p];
```

**Why it matters.** Structures whose pointers can refer to arbitrary nodes cannot be duplicated in a single left-to-right pass without a way to identify “the counterpart of node X”. A pointer-keyed map supplies that identity. A multi-pass approach is typical: first create all counterparts, then wire up their pointers.

An alternative that avoids the map is to interleave counterpart nodes into the original list, wire the extra pointers using the interleaving, and then separate the two lists. It reduces extra space to O(1) at the price of temporarily modifying the input and more delicate pointer handling.

Note that `unordered_map::operator[]` inserts a default value if the key is missing; use `find()` or `count()` when you must not insert.

**Complexity.** Average O(1) per hash operation; O(n) total time and O(n) space.

### 3.9 Array-backed circular buffer (ring buffer)

**What it is.** A fixed-capacity queue implemented on an array where positions wrap around.

**Underlying idea.** Keep a `head` index and a `size` (or a `tail` index). Indexes advance with modular arithmetic, so the end of the array is logically connected to the start, like a circular linked list but with contiguous storage.

```text
capacity = 5, indices 0..4
[ . . A B C ]   head = 2, size = 3
tail position (next free) = (head + size) % capacity = 0
```

```cpp
vector<int> buf(capacity);
int head = 0, size = 0;

// enqueue (if size < capacity)
buf[(head + size) % capacity] = x;
++size;

// dequeue (if size > 0)
head = (head + 1) % capacity;
--size;
```

Tracking `size` explicitly avoids the ambiguity of “full” vs. “empty” that arises when only `head` and `tail` indices are used.

The same structure can also be built with a linked list that keeps pointers to both the first and last nodes. Appending at the tail and removing at the head are then both O(1).

**Complexity.** All operations O(1); O(capacity) space.

### 3.10 Hash map combined with a doubly linked list (ordered storage with O(1) access)

**The problem it solves.** A hash map gives O(1) lookup by key but has no meaningful order. A doubly linked list maintains an order and allows O(1) removal and reinsertion **if you already hold a pointer to the node**. Combining them gives both.

```text
unordered_map<key, pointer-to-node>
        |
        v
front <-> node <-> node <-> node <-> back   (order maintained)
```

- Lookup: find the node pointer through the map in O(1).
- Reorder: unlink that node from its place and relink it at an end, in O(1).
- Remove from an end: read the end node, unlink it, and also erase its key from the map. This is why each node must store its **key**, not only its value: the map entry needs to be found from the node.

Using sentinel head and tail nodes (the dummy idea from 3.1, applied at both ends) removes null checks from every link and unlink.

```cpp
void unlink(DNode* x) {
    x->prev->next = x->next;
    x->next->prev = x->prev;
}
void pushFront(DNode* x) {          // after the front sentinel
    x->next = front->next;
    x->prev = front;
    front->next->prev = x;
    front->next = x;
}
```

The STL offers this combination ready-made: `std::list<T>` is a doubly linked list whose iterators remain valid when other elements are inserted or erased. Store `list<T>::iterator` values in a map, and move an element to the front without invalidating its iterator using `splice`:

```cpp
list<pair<int,int>> items;                       // (key, value)
unordered_map<int, list<pair<int,int>>::iterator> where;

items.splice(items.begin(), items, where[key]);  // move element to front, O(1)
```

**Several orderings at once.** The idea generalizes: one hash map can map a key to its data, while a **second** structure groups keys by some attribute (for example, a map from an integer attribute to its own list) so that each group is itself an ordered list. Moving a key between groups is then an unlink from one list and a relink into another, each O(1). Tracking the smallest non-empty attribute value lets you reach the right group directly.

**Complexity.** O(1) average per operation; O(n) space.

---

## 4. Combining Techniques

```text
Fast/slow pointers  +  Reversal
        ↓
Find a midpoint, reverse one half in place,
then walk both halves together in O(1) extra space
```

Splitting a list at its midpoint, transforming one half, and then traversing the two halves simultaneously is a general in-place strategy for problems that relate the front of a sequence to its back.

```text
Dummy node  +  Gap pointers
        ↓
A trailing pointer positioned at the predecessor of a
node counted from the end, safe even if that node is the head
```

```text
Merge of two sorted lists  +  Divide and conquer
        ↓
Merge many sorted lists in O(N log k)
```

```text
Reversal  +  Segment boundaries (dummy, count, reconnect)
        ↓
Reverse every block of a fixed size in a single left-to-right pass
```

Count ahead to confirm a full block exists, reverse exactly that block, reconnect it to the previous block’s tail and the next block’s head, and move on. A block that is too short is left as-is.

```text
Hash map  +  Doubly linked list
        ↓
O(1) lookup plus O(1) maintenance of an order
```

```text
Pointer-keyed hash map  +  Two-pass construction
        ↓
Duplicate a structure whose pointers can refer to any node
```

```text
Modular indexing  +  Array
        ↓
Fixed-size queue with O(1) enqueue and dequeue and no shifting
```

```text
Cycle detection on  "index → value at index"  +  pigeonhole reasoning
        ↓
Cycle-finding on sequences that are not stored as linked nodes
```

---

## 5. C++ Implementation Notes

**Building a list by hand for experiments**

```cpp
ListNode* head = new ListNode(1);
head->next = new ListNode(2);
head->next->next = new ListNode(3);
```

**Printing a list (useful for debugging)**

```cpp
void print(ListNode* h) {
    while (h != nullptr) { cout << h->val << " -> "; h = h->next; }
    cout << "null\n";
}
```

**Pointer to pointer / reference to pointer.** If a function must change which node the caller’s `head` refers to, either return the new head or take `ListNode*&`. Passing `ListNode*` by value changes only a local copy.

**Custom comparator for `priority_queue`.** Use a lambda or a struct with `operator()` and remember that the comparator describes “lower priority”, so a “greater-than” comparison yields a min-heap.

**`std::list` vs. hand-written nodes.** Use `std::list` when you only need a working doubly linked list with iterators. Write your own nodes when the task is about pointer manipulation itself or when nodes are supplied to you.

**Null checks before dereferencing.** Expressions like `fast->next->next` are safe only after both `fast` and `fast->next` have been checked.

---

## 6. Complexity and Tradeoffs

| Technique | Time | Extra space |
| --- | --- | --- |
| Traversal | O(n) | O(1) |
| Insert/delete after a known node | O(1) | O(1) |
| Reversal (iterative) | O(n) | O(1) |
| Reversal (recursive) | O(n) | O(n) stack |
| Fast/slow pointers | O(n) | O(1) |
| Gap pointers | O(n) | O(1) |
| Merge two sorted lists | O(n + m) | O(1) |
| Merge k lists with heap | O(N log k) | O(k) |
| Merge k lists pairwise | O(N log k) | O(1)–O(log k) |
| Digit-wise construction | O(max(n, m)) | O(max(n, m)) |
| Visited set of nodes | O(n) average | O(n) |
| Pointer-keyed map | O(n) average | O(n) |
| Ring buffer operations | O(1) | O(capacity) |
| Hash map + doubly linked list | O(1) average per operation | O(n) |

**Key tradeoffs**

- **Space vs. simplicity.** Hash-based approaches are often easier to reason about; pointer-based approaches (fast/slow, interleaving) use O(1) extra space but need careful handling of links.
- **Average vs. worst case.** Hash table operations are O(1) on average; adversarial inputs can degrade them, which is rarely a concern in practice for these structures.
- **Mutating the input.** In-place reversal and interleaving modify the original list. Decide whether the original must be preserved, and restore it if so.
- **Recursion vs. iteration.** Recursive list code is compact but uses stack space proportional to list length, which can overflow on very long lists.

---

## 7. Important C++ Pitfalls

1. **Losing the rest of the list.** Always save `cur->next` before overwriting it.
2. **Forgetting the empty list or single-node list.** Check `head == nullptr` and `head->next == nullptr` where relevant.
3. **Dereferencing `nullptr`.** Guard each `->` with a check, especially chained ones.
4. **Leaving a stale `next` at the end.** After rearranging nodes, the final node must have `next == nullptr`, otherwise an accidental cycle or old tail link survives.
5. **Returning the wrong head.** If the head can change, return `dummy.next` (or the new head), not the original `head` variable.
6. **Pointers to stack objects.** A `ListNode dummy(0);` is fine as a helper inside a function; never return `&dummy`.
7. **Comparing values instead of nodes.** Two different nodes can hold equal values. When the question is “same node”, compare **pointers**, not `val`.
8. **`unordered_map::operator[]` side effect.** It silently inserts a default entry for missing keys.
9. **Iterator invalidation.** `erase()` on a `std::list` invalidates only the iterator to the erased element; keep other iterators but never use the erased one afterwards.
10. **Off-by-one in counted loops.** For the gap and sub-range techniques, draw a three- or four-node example and check where each pointer lands before coding.
11. **Memory leaks.** Every `new` should have a matching `delete` when the nodes are owned by your code and no longer needed.
12. **Modulo on negative or wrapped indices.** In ring buffers, keep indices non-negative before applying `%`.

---

## 8. Short Recap

- A linked list stores elements in nodes joined by pointers; access is sequential, but structural changes are cheap.
- The central skill is **safe pointer rewiring**: save before overwrite, draw the before/after picture, and handle the head with a dummy node.
- Reusable techniques: dummy nodes, reversal, fast/slow pointers, gap pointers, merging sorted sequences (two or many), carry-based construction, hash-based node identification, ring buffers, and the hash map plus doubly linked list combination.
- Many of these combine naturally, and most run in O(n) time with O(1) extra space, with hash-based variants trading memory for simplicity.
