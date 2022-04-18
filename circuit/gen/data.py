TESTSET_PREFIX = 'Testset'


def manual(testcase):
  print(f"manual {testcase}.in")

def include(include):
  print(f"@include {include.name or include.__name__[len(TESTSET_PREFIX):]}")

def indexify_testcases(gen_func):
  testcase_index = 0
  def _gen_func(*args):
    nonlocal testcase_index
    gen_func(*args, testcase_index=testcase_index)
    testcase_index += 1
  return _gen_func


class Testset():
  name = None

  def __init__(self):
    assert self.__class__.__name__.startswith(TESTSET_PREFIX)
    print("")
    print(f"@testset {self.__class__.__name__[len(TESTSET_PREFIX):]}")


class Subtask():
  name = None

  def __init__(self):
    print("")
    print(f"@subtask {self.name}")


## Constraints

globalMinN, globalMaxN = 1, 100000
globalMinM, globalMaxM = 1, 100000
globalMinQ, globalMaxQ = 1, 100000

quadraticN = 5000
quadraticM = 5000

smallN = 1000
smallM = 1000
smallQ = 5


## Tree generators

def random_tree(minN, maxN, minM, maxM):
  return ['random-tree', minN, maxN, minM, maxM]

def max_degree_tree(minSize, maxSize, degree):
  return ['max-degree-tree', minSize, maxSize, degree]

def exact_degree_tree(minN, maxN, degree):
  return ['exact-degree-tree', minN, maxN, degree]

def complete_binary_tree(minSize, maxSize):
  return ['complete-binary-tree', minSize, maxSize]

def caterpillar_tree(minSize, maxSize):
  return ['caterpillar-tree', minSize, maxSize]

def broom_tree(minSize, maxSize):
  return ['broom-tree', minSize, maxSize]

def tall_tree(minSize, maxSize):
  return ['tall-tree', minSize, maxSize]

def min_degree_tree(minSize, maxSize, degree):
  return ['min-degree-tree', minSize, maxSize, degree]

def distinct_degrees_tree(maxDegree):
  return ['distinct-degrees-tree', maxDegree]


## Array generators

def random_array(ratio_one=1, ratio_zero=1):
  return ['random-array', ratio_one, ratio_zero]

def alternate_array():
  return ['alternate-array']


## Queries generators

def random_queries(minQ, maxQ):
  return ['random-queries', minQ, maxQ]

def large_queries(minQ, maxQ):
  return ['large-queries', minQ, maxQ]

def point_queries(minQ, maxQ):
  return ['point-queries', minQ, maxQ]


## Generators

@indexify_testcases
def gen(gen_tree, gen_array, gen_queries, testcase_index):
  print(*(["gen"] + gen_tree + gen_array + gen_queries + [testcase_index]))


def common_cases(maxN, maxM, maxQ):
  maxSize = maxN + maxM
  # For some cases, the value of N and M are hard to estimate from the `size`,
  #   so the `size` generated are not a tight bound
  #   The tree generators with this issue is:
  #   `random-tree`, `max-degree-tree`, `tall-tree`, `min-degree-tree`

  gen(
    random_tree(maxN // 2, maxN, maxM // 2, maxM),
    random_array(),
    random_queries(maxQ // 2, maxQ),
  )
  gen(
    max_degree_tree(maxSize // 2, maxSize // 4 * 3, 2),
    random_array(),
    random_queries(maxQ, maxQ),
  )
  gen(
    max_degree_tree(maxSize // 2, maxSize // 4 * 3, 3),
    random_array(),
    random_queries(maxQ, maxQ),
  )
  gen(
    random_tree(maxN, maxN, maxM, maxM),
    random_array(),
    point_queries(maxQ, maxQ)
  )
  gen(
    random_tree(maxN, maxN, maxM, maxM),
    random_array(1, 0),
    random_queries(maxQ, maxQ),
  )
  gen(
    random_tree(maxN, maxN, maxM, maxM),
    random_array(0),
    random_queries(maxQ, maxQ),
  )
  gen(
    random_tree(maxN, maxN, maxM, maxM),
    random_array(10),
    random_queries(maxQ, maxQ),
  )
  gen(
    random_tree(maxN, maxN, maxM, maxM),
    alternate_array(),
    large_queries(maxQ, maxQ),
  )

  # Star graph
  gen(
    random_tree(1, 1, maxM, maxM),
    random_array(),
    large_queries(maxQ, maxQ),
  )
  gen(
    random_tree(1, 1, maxM, maxM),
    random_array(),
    random_queries(maxQ, maxQ),
  )

  # Path graph
  gen(
    random_tree(maxN, maxN, 1, 1),
    random_array(),
    random_queries(maxQ, maxQ)
  )

  # Exact degrees
  # 1000002022 = 2 * 223 * 2242157
  for deg in [2, 3, 5, 223]:
    N = min(maxN, (maxM - 1) // (deg - 1))
    gen(
      exact_degree_tree(N, N, deg),
      random_array(),
      random_queries(maxQ, maxQ)
    )

  even_size = maxSize - maxSize % 2
  gen(
    caterpillar_tree(even_size, even_size),
    random_array(),
    random_queries(maxQ, maxQ),
  )

  gen(
    broom_tree(maxSize, maxSize),
    random_array(),
    random_queries(maxQ, maxQ),
  )
  gen(
    broom_tree(maxSize, maxSize),
    random_array(),
    large_queries(maxQ, maxQ),
  )

  gen(
    tall_tree(maxSize // 2, maxSize // 4 * 3),
    random_array(),
    random_queries(maxQ, maxQ),
  )
  gen(
    min_degree_tree(maxSize // 2, maxSize // 4 * 3, 2),
    random_array(),
    random_queries(maxQ, maxQ),
  )
  gen(
    min_degree_tree(maxSize // 2, maxSize // 4 * 3, 3),
    random_array(),
    random_queries(maxQ, maxQ),
  )
  gen(
    distinct_degrees_tree(int(maxSize**0.5)),
    random_array(),
    random_queries(maxQ, maxQ)
  )


def common_binary(maxN, maxM, maxQ):
  maxSize = maxN + maxM
  gen(
    exact_degree_tree(maxN // 2, maxN - 1, 2),
    random_array(),
    random_queries(maxQ // 2, maxQ)
  )
  gen(
    exact_degree_tree(maxN - 1, maxN - 1, 2),
    random_array(),
    random_queries(maxQ, maxQ),
  )
  gen(
    exact_degree_tree(maxN - 1, maxN - 1, 2),
    alternate_array(),
    random_queries(maxQ, maxQ),
  )
  gen(
    exact_degree_tree(maxN- 1, maxN - 1, 2),
    random_array(),
    large_queries(maxQ // 2, maxQ),
  )

  odd_size = maxSize - (1 - maxSize % 2)
  gen(
    caterpillar_tree(odd_size, odd_size),
    random_array(),
    random_queries(maxQ, maxQ)
  )
  gen(
    caterpillar_tree(odd_size, odd_size),
    random_array(),
    large_queries(maxQ, maxQ)
  )

def common_perfect_binary(maxN, maxM, maxQ):
  # Nearest 2^k - 1 <= maxN + maxM
  maxSize = (1 << len(bin(maxN + maxM).lstrip('-0b')) - 1) - 1
  gen(
    complete_binary_tree(maxSize // 2, maxSize // 2),
    random_array(),
    random_queries(maxQ // 2, maxQ)
  )
  gen(
    complete_binary_tree(maxSize, maxSize),
    random_array(),
    random_queries(maxQ, maxQ)
  )
  gen(
    complete_binary_tree(maxSize, maxSize),
    alternate_array(),
    random_queries(maxQ, maxQ)
  )
  gen(
    complete_binary_tree(maxSize, maxSize),
    random_array(),
    large_queries(maxQ, maxQ)
  )

## Testsets

# This is the only meaningful testset I can think of
class TestsetQuadraticPerfect(Testset):
  def __init__(self):
    super().__init__()
    common_perfect_binary(quadraticN, quadraticM, globalMaxQ)


## Subtasks

class SubtaskSamples(Subtask):
  name = "samples"

  def __init__(self):
    super().__init__()
    manual("sample-1")


class SubtaskNEqualsOne(Subtask):
  name = "n-equals-one"

  def __init__(self):
    super().__init__()
    manual("smallest-binary")
    manual("smallest")
    gen(
      random_tree(1, 1, smallM // 2, smallM),
      random_array(),
      random_queries(smallQ, smallQ)
    )
    gen(
      random_tree(1, 1, smallM, smallM),
      random_array(0),
      random_queries(smallQ, smallQ)
    )
    gen(
      random_tree(1, 1, smallM, smallM),
      random_array(1, 0),
      random_queries(smallQ, smallQ)
    )
    gen(
      random_tree(1, 1, smallM, smallM),
      random_array(),
      random_queries(smallQ, smallQ)
    )
    gen(
      random_tree(1, 1, smallM, smallM),
      random_array(),
      large_queries(smallQ, smallQ)
    )
    gen(
      random_tree(1, 1, smallM, smallM),
      random_array(),
      point_queries(smallQ, smallQ)
    )


class SubtaskBruteBinary(Subtask):
  name = "brute-binary"

  def __init__(self):
    super().__init__()

    maxSize = smallN + smallM
    manual("smallest-binary")
    common_perfect_binary(smallN, smallM, smallQ)
    common_binary(smallN, smallM, smallQ)
    gen(
      complete_binary_tree(maxSize // 2, maxSize),
      random_array(),
      random_queries(smallQ, smallQ)
    )


class SubtaskBrute(Subtask):
  name = "brute"

  def __init__(self):
    super().__init__()

    include(SubtaskNEqualsOne)
    include(SubtaskBruteBinary)
    common_cases(smallN, smallM, smallQ)


class SubtaskPerfectPointUpdate(Subtask):
  name = "perfect-point-update"

  def __init__(self):
    super().__init__()
    maxSize = (1 << len(bin(globalMaxN + globalMaxM).lstrip('-0b')) - 1) - 1
    gen(
      complete_binary_tree(maxSize // 2, maxSize // 2),
      random_array(),
      point_queries(globalMaxQ // 2, globalMaxQ)
    )
    gen(
      complete_binary_tree(maxSize, maxSize),
      random_array(),
      point_queries(globalMaxQ, globalMaxQ)
    )
    gen(
      complete_binary_tree(maxSize, maxSize),
      random_array(1, 0),
      point_queries(globalMaxQ, globalMaxQ)
    )
    gen(
      complete_binary_tree(maxSize, maxSize),
      random_array(0, 1),
      point_queries(globalMaxQ, globalMaxQ)
    )


class SubtaskPerfect(Subtask):
  name = "perfect"

  def __init__(self):
    super().__init__()

    include(SubtaskPerfectPointUpdate)
    include(TestsetQuadraticPerfect)
    common_perfect_binary(globalMaxN, globalMaxM, globalMaxQ)


class SubtaskBinary(Subtask):
  name = "binary"

  def __init__(self):
    super().__init__()

    include(SubtaskBruteBinary)
    include(SubtaskPerfect)
    common_binary(globalMaxN, globalMaxM, globalMaxQ)


class SubtaskQuadratic(Subtask):
  name = "quadratic"

  def __init__(self):
    super().__init__()

    include(SubtaskBrute)
    include(TestsetQuadraticPerfect)
    common_cases(quadraticN, quadraticM, globalMaxQ)


class SubtaskFull(Subtask):
  name = "full"

  def __init__(self):
    super().__init__()

    include(SubtaskQuadratic)
    include(SubtaskBinary)
    common_cases(globalMaxN, globalMaxM, globalMaxQ)


def main():
  for testset in Testset.__subclasses__():
    testset()
  for subtask in Subtask.__subclasses__():
    subtask()

main()
