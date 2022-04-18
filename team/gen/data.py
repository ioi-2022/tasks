TESTSET_PREFIX = "Testset"

maxK = 10**8


def manual(testcase):
  print(f"manual {testcase}.in")


def include(include):
  print(f"@include {include.name or include.__name__[len(TESTSET_PREFIX):]}")


def indexify_testcases(gen_func):
  testcase_index = 0
  def _gen_func(*args):
    nonlocal testcase_index
    # We duplicate the number of testcases to test grader runtime variance and
    # add the number of testcases.
    for _ in range(4):
      gen_func(*args, testcase_index=testcase_index)
    testcase_index += 1
  return _gen_func


@indexify_testcases
def gen_random(
    minN, maxN, minK, maxK,
    minLRatNum, minLRatDen, minLDelta,
    maxLRatNum, maxLRatDen, maxLDelta,
    testcase_index=0):
  print(f"gen random {minN} {maxN} {minK} {maxK}"
        f" {minLRatNum} {minLRatDen} {minLDelta}"
        f" {maxLRatNum} {maxLRatDen} {maxLDelta}"
        f" {testcase_index}")


@indexify_testcases
def gen_ranges(K, ranges, testcase_index=0):
  print(f"gen ranges {K} {len(ranges)}"
        f" {' '.join(map(lambda range : ' '.join(map(str, range)), ranges))}"
        f" {testcase_index}")


def gen_random_simple(N, K, minL, maxL):
    gen_random(N, N, K, K, 0, 1, minL, 0, 1, maxL)


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


class SubtaskSamples(Subtask):
  name = "samples"

  def __init__(self):
    super().__init__()
    manual("sample-1")


class SubtaskOneTeam(Subtask):
  name = "one-team"

  def __init__(self):
    super().__init__()
    manual("one-person")
    manual("two-people")
    manual("three-people-team")
    manual("three-people-no-team")
    gen_random_simple(1, maxK, 1, maxK)
    gen_random_simple(2, 1055, 1, maxK)
    gen_ranges(100, [(1, 1, 1), (99, 99, 1), (99, 99, 1)])
    gen_ranges(100, [(1, 1, 1), (100, 100, 1), (99, 99, 1)])


class SubtaskExponential(Subtask):
  name = "exponential"

  def __init__(self):
    super().__init__()
    include(SubtaskSamples)
    include(SubtaskOneTeam)
    gen_random_simple(4, 1000, 1, 2000)
    gen_random_simple(5, 10000, 1, 20000)
    gen_random_simple(6, 100000, 1, 130000)
    gen_random_simple(7, 1000000, 1, 2000000)
    gen_random_simple(8, 10000000, 1, 20000000)

    for _ in range(6):
      gen_random(1, 8, 1, maxK // 2, 0, 1, 1, 2, 3, 0)

    gen_ranges(10, [(1, 4, 1), (5, 9, 2)])


class SubtaskQuadratic(Subtask):
  name = "quadratic"

  def __init__(self):
    super().__init__()
    include(SubtaskExponential)

    for _ in range(4):
      gen_random(9, 1000, 1, maxK - 10000, 1, 3, 0, 3, 4, 0)

    for _ in range(4):
      gen_random(9, 1000, 1, 100000, 1, 2, -20000, 1, 2, 10000)

    for _ in range(2):
      gen_random(1000, 1000, 1, 100000, 1, 2, -20000, 1, 2, 10000)

    gen_ranges(10**5, [(1, 666, 1), (9335, 9999, 2)])


class SubtaskFull(Subtask):
  name = "full"

  def __init__(self):
    super().__init__()
    include(SubtaskQuadratic)
    
    for _ in range(2):
      gen_random(1001, 10**5, 1, maxK - 10000, 1, 2, -1000000, 1, 2, 10000)
      gen_random(1001, 10**5, 1, maxK - 10000, 1, 2, -10000, 1, 2, 10000)
      gen_random(1001, 10**5, 1, maxK - 10000, 1, 4, 0, 4, 5, 0)

    for _ in range(2):
      gen_random(1001, 10**5, 1, 100000, 1, 4, 0, 4, 5, 0)

    for _ in range(2):
      gen_random(10**5, 10**5, 1, 100000, 1, 3, 0, 4, 5, 0)

    gen_ranges(10**6, [(1, 66666, 1), (933335, 999999, 2)])


def main():
  for subtask in Subtask.__subclasses__():
    subtask()

main()
