# NeetCode DSA Theory Chapter Generator

You will be given:

* **Topic:** the DSA topic/section
* **Problems:** a list of problems from that topic

Create a **standalone theory chapter** that teaches the concepts, data structures, algorithms, and general techniques needed to understand and solve the supplied problem set.

The goal is:

> **Learn the theory and established techniques first → then solve mixed problems and figure out which technique applies yourself.**

Do **not** map individual problems to techniques, reveal solution approaches, or give clues that make problem recognition automatic.

The chapter should read like a **beginner-friendly DSA textbook/handbook**, not like a competitive-programming reference manual.

---

## 1. Prerequisites

List only the knowledge genuinely required to understand this chapter.

Separate into:

### Must Know

Give **specific C++ knowledge**, not vague statements.

For example, instead of:

> Know vectors

write things such as:

* `vector<int>` declaration and initialization
* indexing with `[]`
* `size()`
* `push_back()`
* range-based `for`
* passing vectors to functions

For STL structures, mention the exact operations that will be used, such as:

* `unordered_map`: declaration, `[]`, `count()`, `find()`, `erase()`
* `unordered_set`: declaration, `insert()`, `count()`, `find()`, `erase()`

Also include any genuinely necessary prerequisite concepts such as:

* Big-O basics
* references/pointers
* recursion
* sorting basics

Only include prerequisites that are actually relevant.

### Useful but Not Required

Include knowledge that may make the chapter easier, but is not necessary to begin.

Do not turn this section into a tutorial. It is a prerequisite checklist.

---

# 2. Core Theory

Teach the **fundamental concepts behind the topic**.

Write this section like a DSA textbook:

> Definition → intuition → how it works → important properties → C++ representation/usage → complexity → small generic example

Focus on understanding rather than memorization.

Explain enough that a beginner who has the prerequisites can understand **why the technique/data structure works**, not merely what its name means.

Use diagrams, tables, and small examples when they improve understanding.

### Important depth rule

Teach the **minimum complete conceptual foundation** needed for the supplied problem set.

Do NOT unnecessarily include:

* implementation internals
* advanced variants
* proofs that are not useful at this level
* historical details
* advanced optimizations
* low-level STL implementation details
* competitive-programming tricks
* obscure edge cases

For example, when `unordered_map` is relevant, explain hashing and its practical complexity, but do not spend a large section teaching hash-table implementation techniques such as tombstones, open addressing, rehashing internals, etc., unless the supplied problems actually require them.

Likewise, do not explain the internal implementation of `std::sort` unless it is directly relevant.

The goal is **complete enough to solve the problems, not complete enough to implement the C++ standard library.**

---

# 3. Patterns & General Techniques

This section is important.

Teach the **general reusable techniques that belong to this topic**, but do NOT turn them into a problem-recognition guide.

A technique should explain:

1. **What the technique is**
2. **What it does**
3. **How it works**
4. **Its underlying idea**
5. **How it is implemented in C++**
6. **Time and space complexity**
7. **A small generic example**

For example, a frequency-counting technique should explain the idea:

```text
value → count
```

and then show how `unordered_map` can represent it in C++.

It should NOT include a long section such as:

> “Use this whenever the problem asks whether two things are equal, or when there are duplicates, or when...”

Keep recognition guidance minimal.

The student should discover **when** the technique applies by solving the supplied problems.

### Pattern philosophy

Patterns are **general methods**, not labels attached to specific questions.

Do not write:

> Problem X uses Pattern Y.

Do not write:

> When you see wording like ___, immediately use ___.

Do not provide problem-specific hints.

Instead, teach the technique well enough that the student can later recognize its applicability independently.

---

## 4. Pattern Scope

Include the major techniques that are genuinely relevant to the supplied topic/problem set.

Do not create a separate named pattern for every small trick.

Combine closely related ideas when they naturally belong together.

For example:

* Frequency counting
* Fast lookup with hash sets/maps
* Prefix/suffix accumulation

may be separate techniques because they represent different ideas.

But tiny variations of the same technique should generally be explained together.

Prioritize **high-signal, reusable techniques** over a large catalog of names.

---

# 5. Technique Combinations

After explaining the individual techniques, briefly explain important ways they can be combined.

For example:

```text
Technique A
     +
Technique B
     ↓
new/general strategy
```

Explain the underlying relationship and why combining them is useful.

Do not map these combinations to individual supplied problems.

Only include combinations that are genuinely relevant to the given topic.

---

# 6. C++ Implementation

Throughout the chapter, keep the implementation focus on **modern C++ and STL**.

For important structures/techniques, show concise generic code demonstrating:

* declaration
* essential operations
* common usage
* important syntax
* complexity where relevant

Do not provide complete solutions to the supplied problems.

Code should be small enough to illustrate the concept rather than solve an exercise.

Mention important C++ pitfalls only when they are likely to cause confusion for a beginner working with this topic.

---

# 7. Complexity

Teach the complexity of the main operations and techniques used in the chapter.

Explain:

* time complexity
* space complexity
* average vs worst-case where relevant
* meaningful tradeoffs between approaches

Do not include mathematical proofs of complexity unless they are genuinely needed for understanding.

---

# 8. Scope Control

Use the supplied **Topic + Problems** to determine the scope.

Include concepts that are:

* required to understand the problems
* required to solve them
* or important foundational knowledge for the topic

Do not expand into unrelated DSA material merely because it is connected.

For example, if a topic touches sorting, teach the sorting concepts actually relevant to the supplied problems instead of turning the chapter into a complete sorting textbook.

---

# 9. No Spoilers

The supplied problem list is provided only to determine the chapter's **scope and depth**.

Do NOT:

* solve any supplied problem
* explain the intended approach for a supplied problem
* mention which technique a particular problem uses
* reproduce problem-specific examples that reveal the solution
* order the techniques according to the supplied problems
* give hints that allow the student to immediately identify the intended method

The chapter must remain useful even if the problem list is hidden from the student.

---

# 10. Writing Style

Use a **clear textbook/handbook style**.

The reader is a beginner-to-intermediate DSA learner, not a competitive-programming expert.

Therefore:

* explain unfamiliar terminology before using it
* use precise technical language
* use concrete examples
* avoid unnecessary jargon
* avoid excessive analogies
* avoid overly casual language
* do not assume advanced algorithm knowledge
* do not oversimplify the actual theory

The explanation should feel like:

> “Here is how this concept works.”

not:

> “Here are some tricks to spot this question.”

---

# 11. Recommended Chapter Structure

Adapt the structure to the topic, but generally use:

1. **Prerequisites**
2. **Core Concepts**
3. **Data Structures / Algorithms** relevant to the topic
4. **Patterns & General Techniques**
5. **Combining Techniques**
6. **C++ Implementation Notes**
7. **Complexity & Tradeoffs**
8. **Important C++ Pitfalls**

Do not force sections that are irrelevant to the topic.

Do not add a “Readiness Check” section.

Do not add a long motivational conclusion.

A short recap of the major ideas is acceptable if useful.

---

# 12. Final Quality Requirement

Before producing the chapter, internally check:

> “Does this teach the theory sufficiently for the supplied problems without teaching the solutions?”

and:

> “Is this something a beginner can realistically study before starting the problem set?”

If a section is technically interesting but unnecessary for solving or understanding the supplied problems, remove it.

Prioritize:

**understanding > coverage**

**useful depth > advanced depth**

**general theory > problem-specific hints**

**clear implementation > library internals**

---

## Input

**Topic:**
[insert topic]

**Problems:**
[insert problem list]
