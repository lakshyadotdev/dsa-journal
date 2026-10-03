I will provide you with:

1. A **DSA topic/section name**
2. A list of **problem names** from that section (for example, a NeetCode 250 section)

Your task is to create a **textbook-style theory/notes chapter** that I can study BEFORE solving those problems.

The purpose is NOT to give me solutions to the problems.

The purpose is to teach me the **general theory, concepts, techniques, and algorithmic patterns that are important for the entire section**, so I don't waste time rediscovering well-known patterns from scratch.

Think of this as:

**Prerequisites → Theory → Patterns/Techniques → Problems**

I will then solve the given problems myself and figure out which theory/pattern applies to each problem.

---

## 1. Analyze the section first

Before writing the theory, infer from the provided topic and problem list:

* What knowledge is actually required to solve the problems?
* What are the important prerequisite concepts?
* What C++ knowledge is required?
* What fundamental algorithms/data structures are involved?
* What recurring patterns or techniques are likely relevant?
* Are there concepts from earlier DSA topics that are prerequisites?
* Are there concepts that are technically useful but unnecessary for this particular section?

Do NOT blindly teach everything related to the topic.

Teach what is **actually relevant to the supplied problem set**, while also including closely related foundational concepts when they are important for understanding the section.

The problem list is a guide for determining the scope, not something you should solve one-by-one.

---

# 2. Prerequisites

Start with a concise but specific prerequisite section.

Do NOT write vague prerequisites such as:

> "Know arrays."

Instead, specify the actual level of knowledge required.

For example, if vectors are required, say exactly what I should know:

* `vector` declaration and initialization
* indexing with `[]`
* `size()`
* `push_back()`
* `pop_back()`
* iteration
* `begin()` / `end()`
* basic sorting with `sort()`

Do the same for every prerequisite.

For each prerequisite, distinguish between:

### Must know

Knowledge I should already have before starting.

### Useful but not required

Knowledge that may help but can be learned during the section.

Do not unnecessarily teach the prerequisites in depth. The purpose of this section is to tell me **what I need to know beforehand and at what level**.

Also include relevant C++ STL/data-structure prerequisites when necessary.

For example, if the section requires hashing, don't simply say:

> "Know unordered_map."

Instead specify the operations/concepts I need to know, such as insertion, lookup, checking existence, accessing/updating values, iteration, etc.

Do not list every method of a C++ class. Only include what is relevant to the section.

---

# 3. Core Theory

After prerequisites, teach the **general theory required for the section**.

This should feel like a concise but properly written DSA textbook chapter, not a collection of LeetCode tips.

Explain concepts from first principles where necessary.

For each important concept:

* Explain what it is.
* Explain why it exists / what problem it solves.
* Explain the intuition behind it.
* Explain how it works conceptually.
* Explain important properties.
* Explain relevant time and space complexity.
* Explain important trade-offs when applicable.
* Give small generic examples when they improve understanding.

Use simple examples that are **not copies of the supplied LeetCode problems** whenever possible.

Do not over-fragment the material.

For example, if several ideas naturally belong together, teach them together instead of creating 10 tiny headings.

The theory should be detailed enough that I can genuinely understand the idea before encountering it in a problem, but should not become a huge academic textbook.

---

# 4. Patterns and Techniques

After the core theory, introduce the **important problem-solving patterns/techniques** relevant to the supplied section.

This is different from teaching individual problems.

A pattern should be explained as a reusable idea that can appear across many problems.

For each important pattern, explain:

* What the pattern is.
* The intuition behind it.
* What kind of structure in a problem suggests it.
* What information/state is usually maintained.
* How the technique works at a conceptual level.
* Typical time/space complexity.
* Important variations or combinations with other patterns.
* Common mistakes or misunderstandings.

For example, depending on the supplied topic, patterns might include things such as:

* frequency counting
* membership checking
* prefix/suffix processing
* two pointers
* sliding window
* binary search
* monotonic stack
* BFS/DFS
* prefix sum + hashing
* in-place modification
* divide and conquer
* dynamic programming state
* etc.

Only include patterns that are actually relevant to the supplied topic/problem set.

Do not force every problem into one named pattern.

---

# 5. Do NOT map patterns to individual questions

This is extremely important.

Do NOT write things like:

> Two Sum → Hash Map
> Group Anagrams → Frequency Map
> Product of Array Except Self → Prefix/Suffix

Do not reveal the intended technique for each supplied problem.

I want to develop the ability to recognize the appropriate technique myself.

The theory should instead teach the general concept well enough that, when I encounter a problem, I can ask myself:

> "What information do I need?"

> "What is being repeated?"

> "Can I maintain some state?"

> "Can I trade memory for time?"

> "Does the structure of this problem resemble one of the techniques I learned?"

The problems should function like **exercises after learning the theory**, similar to mathematics.

For example:

**Math:**
Learn Taylor series theory → encounter mixed questions → determine yourself when Taylor series applies.

Likewise:

**DSA:**
Learn the relevant patterns → encounter mixed problems → determine yourself which pattern/combination applies.

Do not remove the challenge of pattern recognition by giving away the answer.

---

# 6. Patterns can combine

Do not assume that every problem uses exactly one pattern.

Explain when important techniques can be combined.

For example:

> Prefix information + hashing

or

> Sorting + two pointers

or

> Hashing + prefix information

or whatever combinations are genuinely relevant to the supplied section.

Explain the underlying relationship rather than attaching combinations to individual problems.

---

# 7. Complexity and trade-offs

Teach complexity as part of the theory where relevant.

Don't merely state:

> O(n)

Explain briefly why.

When multiple approaches exist, explain the conceptual trade-off:

> Approach A: less memory but more time
> Approach B: more memory but faster lookup

This is important because I want to learn **algorithmic thinking**, not just memorize implementations.

---

# 8. C++ focus

The implementation language is **C++**.

Use C++ terminology and STL where appropriate.

However, the main goal is understanding the **algorithm and pattern**, not memorizing library syntax.

Only teach C++ syntax/STL that is actually relevant to the supplied section.

Do not provide complete solutions to the supplied problems.

Small generic code snippets are allowed when they help explain a concept or C++ operation, but avoid turning the chapter into a collection of solution templates.

---

# 9. Avoid problem-specific spoilers

The supplied problem names are primarily used to determine the scope of the chapter.

Do NOT:

* solve them
* give their approaches
* reveal which pattern each uses
* give hints specific enough to reveal their solutions
* reproduce their solutions
* create a "problem → technique" table

You may use the problem list internally to decide what theory should be taught.

The final notes should remain useful even if the problem names were removed.

---

# 10. Scope control

Be comprehensive about the **important concepts**, but do not teach irrelevant DSA.

If a concept is not necessary or meaningfully useful for the supplied section, leave it out.

If a concept is a prerequisite but belongs to an earlier topic, mention it under prerequisites rather than unnecessarily teaching an entire chapter about it.

If a concept is important enough that I cannot reasonably understand the section without it, teach it.

Use judgment.

The goal is:

> **Maximum useful theory, minimum unnecessary theory.**

---

# 11. Structure

Use a natural textbook-like structure.

A typical chapter may look like:

# [Topic]

## Prerequisites

* Required knowledge
* Required C++/STL knowledge
* Exact operations/concepts I should know

## Core Theory

### Concept A

Explanation...

### Concept B

Explanation...

### Concept C

Explanation...

## Patterns & Techniques

### Pattern A

Explanation...

### Pattern B

Explanation...

### Pattern C

Explanation...

## Important Relationships / Combinations

Explain important ways the concepts interact, only when relevant.

## What I should now understand

A concise summary of the concepts and patterns I should have learned before starting the problem set.

Do NOT force this exact structure if the supplied topic naturally requires a different organization. The structure should remain dynamic.

---

# 12. Writing style

Write like a **good DSA textbook written for a student who wants to learn efficiently**.

I prefer:

* clear explanations
* intuition before formalism
* concrete small examples
* concise but sufficient detail
* precise terminology
* practical algorithmic reasoning
* C++-relevant explanations
* minimal unnecessary prose

Avoid:

* motivational filler
* generic study advice
* excessive repetition
* huge walls of text
* unnecessary historical background
* overly academic proofs unless genuinely useful
* explaining every problem individually
* solution walkthroughs
* pattern spoilers for the supplied questions

Do not make every concept a separate tiny section just for the sake of organization.

Group related ideas naturally.

---

# 13. Important philosophy

I am intentionally learning established DSA patterns **before** solving the problems.

I do NOT need to independently rediscover every common algorithmic technique.

The value of the problem-solving stage should instead come from:

* recognizing when a known idea applies
* deciding between multiple possible approaches
* implementing the algorithm
* analyzing complexity
* debugging
* combining known ideas
* encountering genuinely new variations
* eventually recognizing patterns in unfamiliar problems

Therefore, teach established theory/patterns clearly and efficiently first.

Then let the problems test whether I can **recognize and apply** what I learned.

Do not artificially hide basic DSA knowledge just so I am forced to "discover" it through frustration.

At the same time, do not spoon-feed the mapping between theory and individual problems.

---

## Input

I will provide the input in this form:

**Topic:**
[topic/section name]

**Problems:**
[list of problem names]

Generate the theory chapter dynamically based on that input.

Do not assume the topic is Arrays & Hashing. The same prompt must work for any DSA section I provide.
