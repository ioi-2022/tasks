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


globalMinN = 1
quadraticMaxN = 2000
globalMaxN = 100000

smallQueryMaxQ = 1
globalMaxQ = 100000

globalMaxD = 10**9


@indexify_testcases
def gen_random(minN, maxN, minQ, maxQ,
               bitonic, maxD, full_interval, constantD, testcase_index):
  print(f"gen random {minN} {maxN} {minQ} {maxQ}"
        f" {bitonic} {maxD} {full_interval} {constantD} {testcase_index}")


@indexify_testcases
def gen_almost_sorted(
    minN, maxN, minQ, maxQ,
    maxD, full_interval, constantD, reverse, swaps, testcase_index):
  print(f"gen almost-sorted {minN} {maxN} {minQ} {maxQ}"
        f" {maxD} {full_interval} {constantD} {reverse} {swaps}"
        f" {testcase_index}")


@indexify_testcases
def even_odd(
    minN, maxN, minQ, maxQ,
    maxD, full_interval, constantD, swaps, testcase_index):
  print(f"gen even-odd {minN} {maxN} {minQ} {maxQ}"
        f" {maxD} {full_interval} {constantD} {swaps} {testcase_index}")


def common_testcases(minN, maxN, minQ, maxQ,
                     bitonic, maxD, full_interval, constantD, own_subtask=True):
  if own_subtask:
    gen_random(minN, maxN, minQ, maxQ, bitonic, maxD, full_interval, constantD)

  for _ in range(2 if own_subtask else 1):
    gen_random(maxN, maxN, maxQ, maxQ, bitonic, maxD, full_interval, constantD)
  if not bitonic:
    for _ in range(2 if own_subtask else 1):
      for swaps in range(0, 3, 2):
        even_odd(maxN, maxN, maxQ, maxQ, maxD, full_interval, constantD, swaps)

  if own_subtask:
    for swaps in range(0, 3, 2):
      for rev in [False, True]:
        if not bitonic or swaps == 0:
          gen_almost_sorted(
              maxN, maxN, maxQ, maxQ,
              maxD, full_interval, constantD, rev, swaps)
  else:
    for swaps in range(0, 3, 2):
      if not bitonic or swaps == 0:
        gen_almost_sorted(
            maxN, maxN, maxQ, maxQ,
            maxD, full_interval, constantD, swaps == 0, swaps)


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


class TestsetBitonicQuadraticDEqualsOne(Testset):
  def __init__(self):
    super().__init__()
    manual("smallest-mid")
    common_testcases(globalMinN, quadraticMaxN, 1, smallQueryMaxQ,
                     True, 1, False, True, own_subtask=False)


class TestsetBitonicDEqualsOne(Testset):
  def __init__(self):
    super().__init__()
    include(TestsetBitonicQuadraticDEqualsOne)
    common_testcases(globalMinN, globalMaxN, 1, globalMaxQ,
                     True, 1, False, True, own_subtask=False)


class TestsetQuadraticDEqualsOneFullInterval(Testset):
  def __init__(self):
    super().__init__()
    common_testcases(globalMinN, quadraticMaxN, 1, smallQueryMaxQ,
                     False, 1, True, True, own_subtask=False)


class TestsetQuadraticDEqualsOne(Testset):
  def __init__(self):
    super().__init__()
    include(TestsetQuadraticDEqualsOneFullInterval)
    common_testcases(globalMinN, quadraticMaxN, 1, smallQueryMaxQ,
                     False, 1, False, True, own_subtask=False)


class TestsetQuadraticFullInterval(Testset):
  def __init__(self):
    super().__init__()
    include(TestsetQuadraticDEqualsOneFullInterval)
    # Intentionally make own_subtask=True for this testset.
    common_testcases(globalMinN, quadraticMaxN, 1, smallQueryMaxQ,
                     False, globalMaxD, True, True, own_subtask=True)


class TestsetLinearDEqualsOneFullInterval(Testset):
  def __init__(self):
    super().__init__()
    include(TestsetQuadraticDEqualsOneFullInterval)
    common_testcases(globalMinN, globalMaxN, 1, smallQueryMaxQ,
                     False, 1, True, True, own_subtask=False)


class TestsetLinearDEqualsOne(Testset):
  def __init__(self):
    super().__init__()
    include(TestsetQuadraticDEqualsOne)
    include(TestsetLinearDEqualsOneFullInterval)
    common_testcases(globalMinN, globalMaxN, 1, smallQueryMaxQ,
                     False, 1, False, True, own_subtask=False)


class TestsetLinearFullInterval(Testset):
  def __init__(self):
    super().__init__()
    include(TestsetQuadraticFullInterval)
    include(TestsetLinearDEqualsOneFullInterval)
    # Intentionally make own_subtask=True for this testset.
    common_testcases(globalMinN, globalMaxN, 1, smallQueryMaxQ,
                     False, globalMaxD, True, True, own_subtask=True)


# Non-linear DEqualsOneFullInterval does not make sense.


class SubtaskSamples(Subtask):
  name = "samples"

  def __init__(self):
    super().__init__()
    manual("sample-1")


class SubtaskBitonic(Subtask):
  name = "bitonic"

  def __init__(self):
    super().__init__()
    include(TestsetBitonicDEqualsOne)
    common_testcases(globalMinN, globalMaxN, 1, globalMaxQ,
                     True, globalMaxD, False, False)


class SubtaskQuadratic(Subtask):
  name = "quadratic"

  def __init__(self):
    super().__init__()
    include(TestsetBitonicQuadraticDEqualsOne)
    include(TestsetQuadraticDEqualsOne)
    include(TestsetQuadraticFullInterval)
    common_testcases(globalMinN, quadraticMaxN, 1, smallQueryMaxQ,
                     False, globalMaxD, False, False)


class SubtaskLinear(Subtask):
  name = "linear"

  def __init__(self):
    super().__init__()
    include(TestsetLinearDEqualsOne)
    include(TestsetLinearFullInterval)
    include(SubtaskQuadratic)
    common_testcases(globalMinN, globalMaxN, 1, smallQueryMaxQ,
                     False, globalMaxD, False, False)


class SubtaskDEqualsOne(Subtask):
  name = "d-equals-one"

  def __init__(self):
    super().__init__()
    include(TestsetBitonicDEqualsOne)
    include(TestsetLinearDEqualsOne)
    common_testcases(globalMinN, globalMaxN, 1, globalMaxQ,
                     False, 1, False, False)


class SubtaskFullInterval(Subtask):
  name = "full-interval"

  def __init__(self):
    super().__init__()
    include(TestsetLinearFullInterval)
    common_testcases(globalMinN, globalMaxN, 1, globalMaxQ,
                     False, globalMaxD, True, False)


class SubtaskConstantD(Subtask):
  name = "constant-d"

  def __init__(self):
    super().__init__()
    include(SubtaskLinear)
    include(SubtaskDEqualsOne)
    common_testcases(globalMinN, globalMaxN, 1, globalMaxQ,
                     False, globalMaxD, False, True)


class SubtaskFull(Subtask):
  name = "full"

  def __init__(self):
    super().__init__()
    include(SubtaskSamples)
    include(SubtaskBitonic)
    include(SubtaskFullInterval)
    include(SubtaskConstantD)
    common_testcases(globalMinN, globalMaxN, 1, globalMaxQ,
                     False, globalMaxD, False, False)


def main():
  for testset in Testset.__subclasses__():
    testset()
  for subtask in Subtask.__subclasses__():
    subtask()

main()
