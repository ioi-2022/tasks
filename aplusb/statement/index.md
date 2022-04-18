# A Plus B

Given two non-negative integers $A$ and $B$, find the result of adding those two numbers.


## Implementation Details

You should implement the following procedure:

```
int add_two_numbers(int A, int B);
```

* $A$: The first non-negative integer that is given.
* $B$: The second non-negative integer that is given.
* The procedure should return the result of adding the given two numbers.


## Examples

### Example 1

Consider the following call:

```
add_two_numbers(1, 1)
```

This means that you are asked to find the result of adding $1$ and $1$.
Following the definition of arithmetical addition, the result of $1 + 1 = 2$.
Therefore, the procedure should return $2$.

### Example 2

Consider the following call:

```
add_two_numbers(2, 3)
```

This means that you are asked to find the result of adding $2$ and $3$.
It can be shown that adding those two numbers will result in $5$, therefore the procedure should return $5$.


## Constraints

* $0 \le A, B \le 100$


## Subtasks

1. (20 points) $0 \le A, B \le 10$
1. (30 points) $B = 0$
1. (50 points) No additional constraints.


## Sample Grader

The sample grader reads the input in the following format:

* line $1$: $A \; B$

The sample grader prints your answer in the following format:

* line $1$: the integer returned by `add_two_numbers`.
