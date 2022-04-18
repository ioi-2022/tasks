TESTSET_PREFIX = "Testset"


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

@indexify_testcases
def gen_random(maxA=100, maxB=100, testcase_index=0):
  print(f"gen random {maxA} {maxB} {testcase_index}")

@indexify_testcases
def gen_b_zero(maxA=100, testcase_index=0):
  print(f"gen b-zero {maxA} {testcase_index}")


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


class TestsetRandomSmallAB(Testset):
  def __init__(self):
    super().__init__()
    gen_random(10, 10)
    gen_random(10, 10)
    gen_random(10, 10)


class TestsetRandomAB(Testset):
  def __init__(self):
    super().__init__()
    gen_random()
    gen_random()
    gen_random()


class TestsetRandomBZero(Testset):
  def __init__(self):
    super().__init__()
    gen_b_zero()
    gen_b_zero()


class SubtaskSamples(Subtask):
  name = "samples"

  def __init__(self):
    super().__init__()
    manual("sample-1")
    manual("sample-2")


class SubtaskSmallAB(Subtask):
  name = "smallAB"

  def __init__(self):
    super().__init__()
    manual("max-small-test")
    include(SubtaskSamples)
    include(TestsetRandomSmallAB)
    gen_random(1, 10)


class SubtaskBZero(Subtask):
  name = "bZero"

  def __init__(self):
    super().__init__()
    include(TestsetRandomBZero)


class SubtaskFull(Subtask):
  name = "full"

  def __init__(self):
    super().__init__()
    include(TestsetRandomSmallAB)
    include(TestsetRandomAB)
    manual("max-test")


def main():
  for testset in Testset.__subclasses__():
    testset()
  for subtask in Subtask.__subclasses__():
    subtask()

main()
