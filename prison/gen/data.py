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
def gen(minN, maxN, subtask, testcase_index=0):
  print(f"gen {minN} {maxN} {subtask} {testcase_index}")

def common_cases(subtask, fixed, random_splits):
  for N in fixed:
    gen(N, N, subtask)
  for i in range(len(random_splits)-1):
    gen(random_splits[i], random_splits[i+1]-1, subtask)


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
    gen(3, 3, 0)


class SubtaskLinear(Subtask):
  name = "linear"

  def __init__(self):
    super().__init__()
    maxN = 500
    common_cases(subtask=1,
                 fixed=[2, 3, 3 ** 5, 2 ** 8, maxN-1, maxN],
                 random_splits=[4, 50, 400, maxN-1])

class SubtaskSquareRoot(Subtask):
  name = "square-root"

  def __init__(self):
    super().__init__()
    maxN = 500
    common_cases(subtask=2,
                 fixed=[2, 3, 3 ** 5, 2 ** 8, maxN-1, maxN],
                 random_splits=[4, 50, 400, maxN-1])

class SubtaskFull(Subtask):
  name = "full"

  def __init__(self):
    super().__init__()
    maxN = 5000
    common_cases(subtask=3,
                 fixed=[2, 3, 3 ** 7, 2 ** 12, maxN-1, maxN],
                 random_splits = [4, 50, 500, 2000, 4000, maxN-1])

def main():
  for testset in Testset.__subclasses__():
    testset()
  for subtask in Subtask.__subclasses__():
    subtask()

main()
