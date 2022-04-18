import random
random.seed(0)

def manual(testcase):
  print(f"manual {testcase}.in")

def include(include):
  print(f"@include {include.name}")

def indexify_testcases(gen_func):
  testcase_index = 0
  def _gen_func(*args):
    nonlocal testcase_index
    gen_func(testcase_index, *args)
    testcase_index += 1
  return _gen_func


k1e9 = 10**9

kMaxN = 2 * 10**5
kMaxQ = 1 * 10**5
kMaxS = k1e9
kMaxSumO = 2 * 10**5
kMaxNSub = [0, 0, 50, 50, 300, 2000]
kMaxQSub = [0, 0, 50, 50, 300, 2000]
kMaxSSub = [0, 0, 50, k1e9, k1e9, k1e9]
kMaxSumOSub = [0, 0, 50, 50, 300, 2000]

N = kMaxN
Q = kMaxQ
S = kMaxS
O = []
AB = []
P = []


@indexify_testcases
def gen_random(minN, maxN, minQ, maxQ, minElem, maxElem,
               xEqualsOne, yEqualsOne, testcase_index):
  print(f"gen random {minN} {maxN} {minQ} {maxQ} {minElem} {maxElem} "
        f"{xEqualsOne} {yEqualsOne} {testcase_index}")


@indexify_testcases
def CASE(testcase_index, *args):
  all_args = []
  for arg in args:
    all_args += arg
  print(f"gen {testcase_index} {' '.join(all_args)}")


class rnd:
  def nextInt(l, r):
    return random.randint(l, r)


def n(arg):
  global N
  N = arg
  return ["N", str(N)]


def s(arg):
  global S
  S = arg
  return ["S", str(S)]


def q(arg):
  global Q
  Q = arg
  return ["Q", str(Q)]


def generateAllPeopleActive(N, S, maxO, O, AB, split):
  return ["generateAllPeopleActive", str(maxO), str(split)]

def generateChainingIntervals(N, S, O, AB):
  return ["generateChainingIntervals"]

def generateRandomIntervalsForAll(N, S, maxSumO, O, AB):
  return ["generateRandomIntervalsForAll", str(maxSumO)]

def generateIntervalsFromPartition(N, S, partitionSize, O, AB):
  return ["generateIntervalsFromPartition", str(partitionSize)]

def generateRandomQuery(N, Q, P):
  return ["generateRandomQuery"]

def generateRandomMultipleIntervalCase(N, S, Q, O, AB, P, connectingUser=10):
  return ["generateRandomMultipleIntervalCase", str(connectingUser)]

def generateRandomSingleIntervalCase(N, S, Q, O, AB, P):
  return ["generateRandomSingleIntervalCase"]


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


class SubtaskSmallS(Subtask):
  name = "small-S"

  def __init__(self):
    super().__init__()
    include(SubtaskSamples)
    manual("open-1")
    manual("open-2")
    manual("simplest-case")
    manual("all-connected")
    manual("disjoint")

    CASE(
      n(10), q(5),
      generateChainingIntervals(N, S, O, AB),
      generateRandomQuery(N, Q, P)
    )
    CASE(
      n(kMaxNSub[2]-5), q(N),
      generateChainingIntervals(N, S, O, AB),
      generateRandomQuery(N, Q, P)
    )
    CASE(
      n(kMaxNSub[2]-1), q(N),
      generateChainingIntervals(N, S, O, AB),
      generateRandomQuery(N, Q, P)
    )
    manual("three-connected-groups")
    CASE(
      n(32), s(48), q(30),
      generateRandomIntervalsForAll(N, S, kMaxSumOSub[2], O, AB),
      generateRandomQuery(N, Q, P)
    )
    CASE(
      n(kMaxNSub[2]), s(kMaxSSub[2]), q(30),
      generateRandomIntervalsForAll(N, S, kMaxSumOSub[2], O, AB),
      generateRandomQuery(N, Q, P)
    )
    manual("nesting-intervals")
    manual("abacba")

    CASE(
      n(kMaxNSub[2]), s(kMaxSSub[2]), q(min(N, kMaxQSub[2])),
      generateAllPeopleActive(N, S, kMaxSumOSub[2], O, AB, False),
      generateRandomQuery(N, Q, P)
    )
    CASE(
      n(7), s(kMaxSSub[2]), q(min(N, kMaxQSub[2])),
      generateAllPeopleActive(N, S, kMaxSumOSub[2], O, AB, True),
      generateRandomQuery(N, Q, P)
    )
    CASE(
      n(12), s(kMaxSSub[2]), q(min(N, kMaxQSub[2])),
      generateIntervalsFromPartition(N, S, 4, O, AB),
      generateRandomQuery(N, Q, P)
    )
    CASE(
      n(25), s(kMaxSSub[2]), q(min(N, kMaxQSub[2])),
      generateIntervalsFromPartition(N, S, 5, O, AB),
      generateRandomQuery(N, Q, P)
    )
    CASE(
      n(32), s(kMaxSSub[2]), q(min(N, kMaxQSub[2])),
      generateIntervalsFromPartition(N, S, 7, O, AB),
      generateRandomQuery(N, Q, P)
    )
    manual("two-online-times")
    manual("two-online-times-complex")

    for _ in range(3):
      CASE(
        n(kMaxNSub[2]-3), s(kMaxSSub[2]), q(min(N, kMaxQSub[2])),
        generateRandomMultipleIntervalCase(N, S, Q, O, AB, P, 3)
      )


class SubtaskFourth(Subtask):
  name = "fourth"

  def __init__(self):
    super().__init__()
    include(SubtaskSmallS)

    CASE(
      n(kMaxNSub[3]), s(kMaxSSub[3]), q(min(N, kMaxQSub[3])),
      generateAllPeopleActive(N, S, kMaxSumOSub[3], O, AB, False),
      generateRandomQuery(N, Q, P)
    )
    CASE(
      n(7), s(kMaxSSub[3]), q(min(N, kMaxQSub[3])),
      generateAllPeopleActive(N, S, kMaxSumOSub[3], O, AB, True),
      generateRandomQuery(N, Q, P)
    )
    for _ in range(3):
      CASE(
        n(rnd.nextInt(1, kMaxNSub[3])),
        s(rnd.nextInt(kMaxNSub[3], kMaxSSub[3])),
        q(rnd.nextInt(1, min(N, kMaxQSub[3]))),
        generateRandomSingleIntervalCase(N, S, Q, O, AB, P)
      )
    for _ in range(3):
      CASE(
        n(kMaxNSub[3]-3), s(kMaxSSub[3]), q(min(N, kMaxQSub[3])),
        generateRandomMultipleIntervalCase(N, S, Q, O, AB, P, 3)
      )
    for _ in range(3):
      CASE(
        n(rnd.nextInt(1, kMaxNSub[3])),
        s(rnd.nextInt(kMaxNSub[3], kMaxSSub[3])),
        q(rnd.nextInt(1, min(N, kMaxQSub[3]))),
        generateRandomIntervalsForAll(N, S, kMaxSumOSub[3], O, AB),
        generateRandomQuery(N, Q, P)
      )
    for _ in range(5):
      CASE(
        n(kMaxNSub[3]), s(kMaxSSub[3]), q(min(N, kMaxQSub[3])),
        generateRandomIntervalsForAll(N, S, kMaxSumOSub[3], O, AB),
        generateRandomQuery(N, Q, P)
      )


class SubtaskCubic(Subtask):
  name = "cubic"

  def __init__(self):
    super().__init__()
    include(SubtaskFourth)

    CASE(
      n(kMaxNSub[4]), s(kMaxSSub[4]), q(min(N, kMaxQSub[4])),
      generateAllPeopleActive(N, S, kMaxSumOSub[4], O, AB, False),
      generateRandomQuery(N, Q, P)
    )
    CASE(
      n(14), s(kMaxSSub[4]), q(min(N, kMaxQSub[4])),
      generateAllPeopleActive(N, S, kMaxSumOSub[4], O, AB, True),
      generateRandomQuery(N, Q, P)
    )
    for _ in range(5):
      CASE(
        n(rnd.nextInt(kMaxNSub[3]+1, kMaxNSub[4])),
        s(rnd.nextInt(kMaxNSub[4], kMaxSSub[4])),
        q(rnd.nextInt(1, min(N, kMaxQSub[4]))),
        generateRandomSingleIntervalCase(N, S, Q, O, AB, P)
      )
    for _ in range(3):
      CASE(
        n(kMaxNSub[4]-10), s(kMaxSSub[4]), q(min(N, kMaxQSub[4])),
        generateRandomMultipleIntervalCase(N, S, Q, O, AB, P)
      )
    for _ in range(5):
      CASE(
        n(rnd.nextInt(kMaxNSub[3]+1, kMaxNSub[4])),
        s(rnd.nextInt(kMaxNSub[4], kMaxSSub[4])),
        q(rnd.nextInt(1, min(N, kMaxQSub[4]))),
        generateRandomIntervalsForAll(N, S, kMaxSumOSub[4], O, AB),
        generateRandomQuery(N, Q, P)
      )
    for _ in range(10):
      CASE(
        n(kMaxNSub[4]), s(kMaxSSub[4]), q(min(N, kMaxQSub[4])),
        generateRandomIntervalsForAll(N, S, kMaxSumOSub[4], O, AB),
        generateRandomQuery(N, Q, P)
      )

class SubtaskQuadratic(Subtask):
  name = "quadratic"

  def __init__(self):
    super().__init__()
    include(SubtaskCubic)

    CASE(
      n(kMaxNSub[5]), s(kMaxSSub[5]), q(min(N, kMaxQSub[5])),
      generateAllPeopleActive(N, S, kMaxSumOSub[5], O, AB, False),
      generateRandomQuery(N, Q, P)
    )
    CASE(
      n(44), s(kMaxSSub[5]), q(min(N, kMaxQSub[5])),
      generateAllPeopleActive(N, S, kMaxSumOSub[5], O, AB, True),
      generateRandomQuery(N, Q, P)
    )
    for _ in range(2):
      CASE(
        n(1337), s(123456789), q(min(N, kMaxQSub[5])),
        generateRandomSingleIntervalCase(N, S, Q, O, AB, P)
      )
    for _ in range(3):
      CASE(
        n(kMaxNSub[5]-10), s(kMaxSSub[5]), q(min(N, kMaxQSub[5])),
        generateRandomMultipleIntervalCase(N, S, Q, O, AB, P)
      )
    for _ in range(5):
      CASE(
        n(rnd.nextInt(kMaxNSub[4]+1, kMaxNSub[5])),
        s(rnd.nextInt(kMaxNSub[5], kMaxSSub[5])),
        q(rnd.nextInt(1, min(N, kMaxQSub[5]))),
        generateRandomIntervalsForAll(N, S, kMaxSumOSub[5], O, AB),
        generateRandomQuery(N, Q, P)
      )
    for _ in range(10):
      CASE(
        n(kMaxNSub[5]), s(kMaxSSub[5]), q(min(N, kMaxQSub[5])),
        generateRandomIntervalsForAll(N, S, kMaxSumOSub[5], O, AB),
        generateRandomQuery(N, Q, P)
      )


class SubtaskFull(Subtask):
  name = "full"

  def __init__(self):
    super().__init__()
    include(SubtaskQuadratic)

    CASE(
      n(kMaxN), s(kMaxS), q(min(N, kMaxQ)),
      generateAllPeopleActive(N, S, kMaxSumO, O, AB, False),
      generateRandomQuery(N, Q, P)
    )
    CASE(
      n(444), s(kMaxS), q(min(N, kMaxQ)),
      generateAllPeopleActive(N, S, kMaxSumO, O, AB, True),
      generateRandomQuery(N, Q, P)
    )
    for _ in range(3):
      CASE(
        n(kMaxN), s(kMaxS), q(min(N, kMaxQ)),
        generateRandomSingleIntervalCase(N, S, Q, O, AB, P)
      )
    for _ in range(3):
      CASE(
        n(kMaxN-10), s(kMaxS), q(min(N, kMaxQ)),
        generateRandomMultipleIntervalCase(N, S, Q, O, AB, P)
      )
    for _ in range(5):
      CASE(
        n(rnd.nextInt(kMaxNSub[5]+1, kMaxN)),
        s(rnd.nextInt(kMaxN, kMaxS)),
        q(rnd.nextInt(1, min(N, kMaxQ))),
        generateRandomIntervalsForAll(N, S, kMaxSumO, O, AB),
        generateRandomQuery(N, Q, P)
      )
    for _ in range(5):
      CASE(
        n(kMaxN), s(kMaxS), q(min(N, kMaxQ)),
        generateRandomIntervalsForAll(N, S, kMaxSumO, O, AB),
        generateRandomQuery(N, Q, P)
      )


def main():
  for subtask in Subtask.__subclasses__():
    subtask()

main()
