def manual(testcase):
  print(f"manual {testcase}.in")


def include(include):
  print(f"@include {include.name}")


def indexify_testcases(gen_func):
  testcase_index = 0
  def _gen_func(*args):
    nonlocal testcase_index
    # We duplicate the number of testcases to test grader runtime variance and
    # add the number of testcases.
    for _ in range(2):
      gen_func(*args, testcase_index=testcase_index)
    testcase_index += 1
  return _gen_func


@indexify_testcases
def gen_must_sure(
    minN, maxN, subtask_index, graph_type, *args, testcase_index=0):
  print(
      f"gen {minN} {maxN} {subtask_index} must-sure"
      f" {graph_type} {' '.join(map(str, list(args) + ['']))}{testcase_index}")


@indexify_testcases
def gen_greedy_outdeg_min(minN, maxN, subtask_index, testcase_index=0):
  print(
      f"gen {minN} {maxN} {subtask_index} greedy-outdeg-min"
      f" {testcase_index}")


@indexify_testcases
def gen_greedy_outdeg_max(minN, maxN, subtask_index, testcase_index=0):
  print(
      f"gen {minN} {maxN} {subtask_index} greedy-outdeg-max"
      f" {testcase_index}")


@indexify_testcases
def gen_near_found(minN, maxN, subtask_index, testcase_index=0):
  print(
      f"gen {minN} {maxN} {subtask_index} near-found"
      f" {testcase_index}")


@indexify_testcases
def gen_maintain_cycle(minN, maxN, subtask_index, testcase_index=0):
  print(
      f"gen {minN} {maxN} {subtask_index} maintain-cycle"
      f" {testcase_index}")


@indexify_testcases
def gen_maintain_cycle_deterministic(
    minN, maxN, subtask_index, testcase_index=0):
  print(
      f"gen {minN} {maxN} {subtask_index} maintain-cycle-deterministic"
      f" {testcase_index}")


@indexify_testcases
def gen_delay_elimination(minN, maxN, subtask_index, testcase_index=0):
  print(
      f"gen {minN} {maxN} {subtask_index} delay-elimination"
      f" {testcase_index}")


@indexify_testcases
def gen_maintain_adjacent_nodes(minN, maxN, subtask_index, testcase_index=0):
  print(
      f"gen {minN} {maxN} {subtask_index} maintain-adjacent-nodes"
      f" {testcase_index}")


@indexify_testcases
def gen_maintain_three_nodes(minN, maxN, subtask_index, testcase_index=0):
  print(
      f"gen {minN} {maxN} {subtask_index} maintain-three-nodes"
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
    manual("sample-2")


def gen_common_cases(minN, maxN, subtask_index):
  for _ in range(2):
    gen_must_sure(maxN - 5, maxN, subtask_index, 'random')
    gen_must_sure(maxN - 5, maxN, subtask_index, 'cycle', 1)
    gen_must_sure(maxN - 5, maxN, subtask_index, 'cycle', 2)
    gen_must_sure(maxN - 5, maxN, subtask_index, 'has-k-outgoing', 0)
    gen_must_sure(maxN - 5, maxN, subtask_index, 'has-k-outgoing', 1)
    gen_must_sure(maxN - 5, maxN, subtask_index, 'has-k-outgoing', 2)
    gen_must_sure(maxN - 5, maxN, subtask_index, 'dag')
    gen_maintain_cycle(maxN, maxN, subtask_index)
    gen_delay_elimination(maxN, maxN, subtask_index)
    gen_maintain_adjacent_nodes(maxN, maxN, subtask_index)
    gen_maintain_three_nodes(maxN, maxN, subtask_index)

  gen_greedy_outdeg_max(maxN, maxN, subtask_index)
  gen_greedy_outdeg_min(maxN, maxN, subtask_index)
  gen_near_found(maxN, maxN, subtask_index)
  gen_maintain_cycle_deterministic(maxN, maxN, subtask_index)


class SubtaskSmallN(Subtask):
  name = "smallN"
  subtask_index = 1
  minN = 3
  maxN = 250

  def __init__(self):
    super().__init__()
    include(SubtaskSamples)
    gen_must_sure(self.minN, self.minN, self.subtask_index, 'cycle', 1)
    gen_must_sure(self.minN, self.minN, self.subtask_index, 'dag')
    if self.minN <= 5:
      gen_must_sure(5, 5, self.subtask_index, 'cycle', 2)
    gen_common_cases(self.minN, self.maxN, self.subtask_index)


class SubtaskFull(Subtask):
  name = "full"
  subtask_index = 2
  minN = 251
  maxN = 2000

  def __init__(self):
    super().__init__()
    gen_common_cases(self.minN, self.maxN, self.subtask_index)


def main():
  for subtask in Subtask.__subclasses__():
    subtask()

main()
