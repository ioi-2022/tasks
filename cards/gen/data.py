def manual(testcase):
  print(f"manual {testcase}.in")

def indexify_testcases(gen_func):
  testcase_index = 0
  def _gen_func(*args):
    nonlocal testcase_index
    gen_func(*args, testcase_index=testcase_index)
    testcase_index += 1
  return _gen_func

@indexify_testcases
def gen(n, k, q, testcase_index=0):
  print(f"gen {n} {k} {q} {testcase_index}")


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


# K = 2; N <= 3
class SubtaskK2(Subtask):
  name = "k-equals-2"

  def __init__(self):
    super().__init__()
    gen(2, 2, 1)
    gen(3, 2, 5)
    gen(3, 2, 50000)
    gen(3, 2, 50000)


# K = 3; N <= 5
class SubtaskK3(Subtask):
  name = "k-equals-3"

  def __init__(self):
    super().__init__()
    gen(3, 3, 1)
    gen(4, 3, 5)
    gen(5, 3, 5)
    gen(4, 3, 50000)
    gen(5, 3, 50000)
    gen(5, 3, 50000)


# K = 6; N <= 12
class SubtaskK6(Subtask):
  name = "k-equals-6"

  def __init__(self):
    super().__init__()
    gen(6, 6, 1)
    gen(7, 6, 100)
    gen(8, 6, 100)
    gen(9, 6, 100)
    gen(10, 6, 100)
    gen(11, 6, 100)
    gen(12, 6, 100)
    gen(10, 6, 50000)
    gen(11, 6, 50000)
    gen(12, 6, 50000)
    gen(12, 6, 50000)


# K = 8; N <= 10000
class SubtaskK6(Subtask):
  name = "full"

  def __init__(self):
    super().__init__()
    gen(8, 8, 1)
    gen(9, 8, 100)
    gen(10, 8, 100)
    gen(100, 8, 50000)
    gen(1000, 8, 50000)
    gen(10000, 8, 50000)
    gen(10000, 8, 50000)
    gen(10000, 8, 50000)


def main():
  for subtask in Subtask.__subclasses__():
    subtask()

main()
