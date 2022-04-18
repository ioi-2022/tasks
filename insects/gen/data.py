import math


def manual(testcase):
  print(f"manual {testcase}.in")

def include(include):
  print(f"@include {include.name}")


def indexify_testcases(gen_func):
  testcase_index = 0
  def _gen_func(*args):
    nonlocal testcase_index
    gen_func(*args, testcase_index=testcase_index)
    testcase_index += 1
  return _gen_func


@indexify_testcases
def gen_random(minN, maxN, partial, D, testcase_index=0):
  print(
      f"gen {minN} {maxN} {partial} random {D}"
      f" {testcase_index}")

@indexify_testcases
def gen_uniform(minN, maxN, partial, D, testcase_index=0):
  print(
      f"gen {minN} {maxN} {partial} uniform {D}"
      f" {testcase_index}")

@indexify_testcases
def gen_dominant(minN, maxN, partial, pos, D, testcase_index=0):
  print(
      f"gen {minN} {maxN} {partial} dominant {pos} {D}"
      f" {testcase_index}")

@indexify_testcases
def gen_linear(minN, maxN, partial, D, testcase_index=0):
  print(
      f"gen {minN} {maxN} {partial} linear {D}"
      f" {testcase_index}")

@indexify_testcases
def gen_optimum(minN, maxN, partial, pos, D, ans, testcase_index=0):
  print(
      f"gen {minN} {maxN} {partial} optimum {pos} {D} {ans}"
      f" {testcase_index}")

@indexify_testcases
def gen_special(minN, maxN, partial, ver, testcase_index=0):
  print(
      f"gen {minN} {maxN} {partial} special {ver}"
      f" {testcase_index}")


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


def gen_small_cases(partial):
  gen_uniform(2, 2, partial, 1)
  gen_uniform(2, 2, partial, 2)
  gen_uniform(3, 3, partial, 2)
  gen_random(4, 7, partial, 2)
  gen_random(8, 10, partial, 3)

def gen_common_cases(minN, maxN, partial):
  sqrtN = math.floor(math.sqrt(maxN))
  gen_uniform(minN, maxN, partial, 1)
  gen_uniform(minN, maxN, partial, maxN)
  gen_uniform(minN, maxN, partial, sqrtN)
  gen_linear(minN, maxN, partial, sqrtN)
  gen_dominant(minN, maxN, partial, "shuffle", 2)
  gen_dominant(minN, maxN, partial, "shuffle", maxN//2)
  gen_optimum(minN, maxN, partial, "shuffle", 5, 2)
  gen_optimum(minN, maxN, partial, "shuffle", sqrtN, 1)

def gen_random_cases(minN, maxN, partial):
  ds = [5, 8, 10, 12, 15, 20, 50, 100, 200, 500, 1000, 2000]
  for d in ds:
    if d > maxN:
      break
    gen_random(minN, maxN, partial, d)

def gen_counter_cases(minN, maxN, partial):
  gen_optimum(minN - 1, minN - 1, partial, "both", 60, 32)
  gen_optimum(minN - 1, minN - 1, partial, "left", 60, 32)
  gen_optimum(minN - 1, minN - 1, partial, "both", 30, 64)
  gen_optimum(minN - 1, minN - 1, partial, "right", 30, 64)
  gen_optimum(minN + 1, minN + 1, partial, "both", 60, 17)
  gen_optimum(minN + 1, minN + 1, partial, "shuffle", 60, 17)
  gen_optimum(minN + 9, minN + 9, partial, "both", 117, 2)
  gen_optimum(minN + 9, minN + 9, partial, "left", 117, 2)
  gen_optimum(minN - 10, minN - 10, partial, "both", 58, 32)
  gen_optimum(minN - 10, minN - 10, partial, "shuffle", 58, 32)
  
  gen_optimum(minN, minN, partial, "both", 60, 18)
  gen_optimum(minN, minN, partial, "right", 60, 18)
  gen_optimum(maxN, maxN, partial, "both", 118, 16)
  gen_optimum(maxN, maxN, partial, "left", 118, 16)
  gen_optimum(maxN, maxN, partial, "shuffle", 59, 33)
  gen_optimum(maxN, maxN, partial, "right", 59, 33)

  gen_special(maxN, maxN, partial, 1)
  gen_special(maxN, maxN, partial, 2)
  gen_uniform(maxN, maxN, partial, 2)
  gen_dominant(maxN, maxN, partial, "left", maxN//2)
  gen_dominant(maxN, maxN, partial, "right", maxN//2)
  gen_dominant(maxN, maxN, partial, "both", maxN//2)


class SubtaskQuadratic(Subtask):
  name = "quadratic"
  partial = 0
  maxN = 200

  def __init__(self):
    super().__init__()
    gen_small_cases(self.partial)
    gen_common_cases(self.maxN - 5, self.maxN, self.partial)
    gen_random_cases(self.maxN - 5, self.maxN, self.partial)


class SubtaskSubquadratic(Subtask):
  name = "subquadratic"
  partial = 0
  maxN = 1000

  def __init__(self):
    super().__init__()
    include(SubtaskQuadratic)
    gen_common_cases(self.maxN - 5, self.maxN, self.partial)
    gen_random_cases(self.maxN - 5, self.maxN, self.partial)


class SubtaskFull(Subtask):
  name = "full"
  partial = 1
  maxN = 2000

  def __init__(self):
    super().__init__()
    include(SubtaskSubquadratic)
    gen_small_cases(self.partial)
    gen_common_cases(self.maxN, self.maxN, self.partial)
    gen_random_cases(self.maxN - 5, self.maxN, self.partial)
    gen_counter_cases(self.maxN - 20, self.maxN, self.partial)


def main():
  for subtask in Subtask.__subclasses__():
    subtask()

main()
