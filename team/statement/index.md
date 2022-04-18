# Team Contest

<!-- NOTE TO TRANSLATORS: DUE TO TIME LIMITATIONS DURING MOCK TRANSLATION, PLEASE ONLY TRANSLATE THE TASK STORY, STARTING FROM THIS BLOCK -->

You are a staff member of a university.
Your university is registering several teams to participate in a programming contest.
The programming contest is participated by teams of three programmers.

In your university, there are $N$ eligible programmers, numbered from $0$ to $N - 1$.
For each $i$ such that $0 \le i \le N - 1$, programmer $i$ has a skill level of $L[i]$.
The skill level of a team consisting of programmer $i$, $j$, and $k$ is $\min(L[i], L[j], L[k]) + \max(L[i], L[j], L[k])$.

You want to only register teams with a skill level of strictly more than $K$.
Each programmer may only be assigned to at most one registered team.
You want to know the maximum number of teams you can register.

<!-- NOTE TO TRANSLATORS: END OF THE TRANSLATION BLOCK -->


## Implementation Details

You should implement the following procedure:

```
int maximum_teams(int N, int K, int[] L);
```

* $N$: the number of programmers.
* $K$: the skill level limit of the registered teams.
* $L$: an array of length $N$ describing the skill level of the programmers.
* This procedure should return the maximum number of teams you can register.
* This procedure is called exactly once.


## Examples

### Example 1

Consider the following call:

```
maximum_teams(8, 6, [5, 4, 6, 2, 3, 2, 1, 1])
```

You can register a team with programmer $0$, $3$, and $5$ (with skill levels $5$, $2$, $2$ respectively) and a team with programmer $1$, $2$, and $4$ (with skill levels $4$, $6$, $3$ respectively).
There is no way to register more than two teams. 
Therefore, the procedure `maximum_teams` should return $2$.


## Constraints

* $1 \le N \le 100\;000$
* $1 \le K \le 10^8$
* $1 \le L[i] \le 10^8$ (for each $i$ such that $0 \le i \le N - 1$)


## Subtasks

1. (6 points) $N \le 3$
1. (12 points) $N \le 8$
1. (37 points) $N \le 1000$
1. (45 points) No additional constraints.


## Sample Grader

The sample grader reads the input in the following format:

* line $1$: $N \; K$
* line $2$: $L[0] \; L[1] \; \ldots \; L[N - 1]$

The sample grader prints your answer in the following format:

* line $1$: the return value of `maximum_teams`
