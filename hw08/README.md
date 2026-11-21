# HW8 – Binary Search Tree

## Build and Run

Compile the programs:

```bash
make
```

Run the tests:

```bash
make test
```

Run memory checks:

```bash
make memcheck
```

Remove compiled executables:

```bash
make clean
```

---

## Part 2: Finding the Original Search Bug

### Test That Triggers the Crash

The test `Search in empty tree` calls `bst_contains()` on an empty tree.

### AddressSanitizer Report

```text
bst.c:118:19: runtime error: member access within null pointer of type 'const struct node'
```

### Explanation

The original search function checks `current->data` in the while condition before checking whether `current` is NULL. When the tree is empty, `current` is NULL, causing an invalid memory access.

The original program did not detect this bug because its main function inserted seven keys before performing any searches. Therefore, the root was never NULL when the search function was called.

---

## Part 3: Tree Statistics

### Increasing Insertion Order

Command:

```bash
seq 1 1023 | ./treestats
```

Output:

```text
n 1023, height 1023
```

### Random Insertion Order

Command:

```bash
seq 1 1023 | sort -R | ./treestats
```

Output:

```text
n 1023, height 21
```

### Analysis

When values are inserted in increasing order, each new node becomes the right child of the previous node. The BST becomes completely unbalanced, with a height of 1023.

When values are inserted in random order, the tree is much more balanced, with a height of 21. Therefore, insertion order strongly affects tree height and can also affect the number of visits required during searches.
