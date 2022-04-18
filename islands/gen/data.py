import random
random.seed(0)

TESTSET_PREFIX = "Testset"
MAXN = 100000
MAXM = 200000

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

def shuffle_tc(tc):
  random.shuffle(tc)
  for t in tc:
    t()


@indexify_testcases
def gen_random(N=MAXN, M=MAXM, modifier="", testcase_index=0):
  print(f"gen random{modifier} {N} {M} {testcase_index}")

@indexify_testcases
def gen_dag(N=MAXN, M=MAXM, modifier="", testcase_index=0):
  print(f"gen dag{modifier} {N} {M} {testcase_index}")


@indexify_testcases
def gen_circle(N=MAXN, M=MAXM, circle_size=100, modifier="", testcase_index=0):
  print(f"gen circle{modifier} {N} {M} {circle_size} {testcase_index}")

@indexify_testcases
def gen_line(N=MAXN, M=MAXM, line_size=100, modifier="", testcase_index=0):
  print(f"gen line{modifier} {N} {M} {line_size} {testcase_index}")

@indexify_testcases
def gen_solution(N=MAXN, M=MAXM, sol_size=100, modifier="", testcase_index=0):
  print(f"gen solution1{modifier} {N} {M} {sol_size} {testcase_index}")

@indexify_testcases
def gen_solution_dag(N=MAXN, M=MAXM, modifier="", testcase_index=0):
  print(f"gen solution2{modifier} {N} {M} {testcase_index}")

@indexify_testcases
def gen_tricky(type=1,N=MAXN, K=1, modifier="", testcase_index=0):
  print(f"gen tricky{type}{modifier} {N} {0} {K} {testcase_index}")


@indexify_testcases
def gen_complete(N=MAXN, testcase_index=0):
  print(f"gen complete {N} {N*(N-1)} {testcase_index}")


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


class TestsetCompleteSmall(Testset):
  def __init__(self):
    super().__init__()
    gen_complete(2)
    gen_complete(3)
    gen_complete(4)
    gen_complete(10)
    gen_complete(400)


#==========
class SubtaskSamples(Subtask):
  name = "samples"

  def __init__(self):
    super().__init__()
    manual("sample-1")
    manual("sample-2")


class SubtaskN2(Subtask):
  name = "n-equals-two"

  def __init__(self):
    super().__init__()
    manual("sample-2")
    manual("N2-minimum-case")
    manual("N2-no-outdeg")
    manual("N2-no-indeg")
    gen_circle(2, 2, 2)
    gen_random(2, 10)
    gen_random(2, 200000)


class SubtaskComplete(Subtask):
  name = "complete"

  def __init__(self):
    super().__init__()
    include(TestsetCompleteSmall)
    manual("shuffled-complete")

class SubtaskBidirectional(Subtask):
  name = "bidirectional"

  def __init__(self):
    super().__init__()
    include(TestsetCompleteSmall)

    testcases = [
    # bidirectional perfect line, no solution
    lambda:gen_line(2, 1, 2, "-bidirectional"),
    lambda:gen_line(100, 99, 100, "-bidirectional"),
    lambda:gen_line(1000, 999, 1000, "-bidirectional"),

    # a line, except 1 node with a branching path. Has solution
    lambda:gen_line(100, 99, 99, "-bidirectional"),
    lambda:gen_line(100, 99, 99, "-bidirectional"),
    lambda:gen_line(100, 99, 99, "-bidirectional"),
    lambda:gen_line(1000, 999, 999, "-bidirectional"),

    # randoms
    lambda:gen_random(100, 1, "-bidirectional"),
    lambda:gen_random(100, 50, "-bidirectional"),
    lambda:gen_random(100, 100, "-bidirectional"),
    lambda:gen_random(1000, 500, "-bidirectional"),
    lambda:gen_random(1000, 2000, "-bidirectional"),
    lambda:gen_random(1000, 40000, "-bidirectional"),
    lambda:gen_random(1000, 50000, "-bidirectional"),

    # tricky
    lambda:gen_tricky(1, 2, 0, "-bidirectional"),
    lambda:gen_tricky(1, 3, 0, "-bidirectional"),
    lambda:gen_tricky(1, 5, 0, "-bidirectional"),
    lambda:gen_tricky(1, 1000, 0, "-bidirectional"),
    ]

    shuffle_tc(testcases)

class SubtaskDouble(Subtask):
  name = "double"

  def __init__(self):
    super().__init__()

    testcases = [
    # a perfect line
    # no solution
    lambda:gen_line(1000, 999, 1000, "-double"),

    # DAG
    # no solution
    lambda:gen_dag(50, 100, "-double"),
    lambda:gen_dag(500, 1000, "-double"),
    lambda:gen_dag(500, 77000, "-double"),
    lambda:gen_dag(999, 99999, "-double"),
    lambda:gen_dag(1000, 100000, "-double"),
    lambda:gen_dag(80,  100000, "-double"),

    # a circle with a bunch of extra useless edges
    # has solution
    lambda:gen_circle(100, 1000, 50, "-double"),
    lambda:gen_circle(1000, 1000, 1000, "-double"),
    lambda:gen_circle(1000, 1005, 990, "-double"),
    lambda:gen_circle(1000, 80000, 700, "-double"),
    lambda:gen_circle(1000, 90000, 700, "-double"),
    lambda:gen_circle(1000, 90000, 10, "-double"),

    # has solution
    lambda:gen_solution(10, 10, 10, "-double"),
    lambda:gen_solution(1000, 1000, 50, "-double"),
    lambda:gen_solution(555, 555, 555, "-double"),
    lambda:gen_solution(1000, 1000, 950, "-double"),
    lambda:gen_solution(1000, 1000, 1000, "-double"),
    lambda:gen_solution(1000, 95000, 800, "-double"),
    lambda:gen_solution(1000, 95000, 800, "-double"),
    lambda:gen_solution(1000, 95000, 800, "-double"),
    
    lambda:gen_solution_dag(1000, 95000, "-double"),
    lambda:gen_solution_dag(1000, 2000, "-double"),
    lambda:gen_solution_dag(100, 95000, "-double"),
    

    # randoms
    lambda:gen_random(100, 1, "-double"),
    lambda:gen_random(100, 50, "-double"),
    lambda:gen_random(100, 100, "-double"),
    lambda:gen_random(1000, 500, "-double"),
    lambda:gen_random(1000, 2000, "-double"),
    lambda:gen_random(1000, 40000, "-double"),
    lambda:gen_random(1000, 50000, "-double"),
    ]
    
    # tricky
    gen_circle(100, 200, 40, "-double"),
    gen_circle(100, 2000, 50, "-double"),
    gen_circle(100, 90000, 50, "-double"),
    gen_circle(1000, 90000, 50, "-double"),
    gen_tricky(3, 900, 0, "-double"),
    gen_tricky(3, 900, 1, "-double"),

    gen_tricky(1, 2, 0, "-double"),
    gen_tricky(1, 3, 0, "-double"),
    gen_tricky(1, 5, 0, "-double"),
    gen_tricky(1, 1000, 0, "-double"),
    
    gen_tricky(2, 1000, 0, "-double"),
    gen_tricky(2, 1000, 1, "-double"),
    
    gen_tricky(3, 1000, 0, "-double"),
    gen_tricky(3, 1000, 1, "-double"),
    
    gen_tricky(4, 1000, 0, "-double"),
    

    shuffle_tc(testcases)


class SubtaskFull(Subtask):
  name = "full"

  def __init__(self):
    super().__init__()

    include(SubtaskSamples)
    include(SubtaskN2)
    include(SubtaskComplete)
    include(SubtaskBidirectional)
    include(SubtaskDouble)

    testcases = [
    # smaller solutions
    lambda:gen_solution(8, 8, 8),
    lambda:gen_solution(10, 10, 10),
    lambda:gen_solution(8, 15, 5),
    lambda:gen_solution(10, 20, 8),
    lambda:gen_solution(20, 8, 8),
    lambda:gen_solution(100, 1000, 50),
    lambda:gen_solution(1000, 1000, 70),
    lambda:gen_solution(1000, 1000, 900),
    lambda:gen_solution(1000, 2000, 900),

    # big randoms
    lambda:gen_random(45555, 31421),
    lambda:gen_random(78543, 2222),
    lambda:gen_random(99000, 190000),
    lambda:gen_random(100000, 200000),

    # big non-solution
    lambda:gen_line(100000, 100000, 30000),
    lambda:gen_circle(100000, 100000, 100000),
    lambda:gen_circle(100000, 200000, 5000),
    lambda:gen_circle(100000, 200000, 5000),
    lambda:gen_circle(100000, 200000, 5000),
    lambda:gen_dag(10, 200000),
    lambda:gen_dag(1000, 200000),
    lambda:gen_dag(100000, 200000),
    lambda:gen_dag(100000, 200000),


    # solutions
    lambda:gen_solution(2000, 2000, 200),
    lambda:gen_solution(2000, 5000, 200),
    lambda:gen_solution(2000, 10000, 500),
    lambda:gen_solution(2000, 100000, 500),
    lambda:gen_solution(50000, 50000, 40000),
    lambda:gen_solution(50000, 50000, 40000),
    lambda:gen_solution(100000, 100000, 80000),
    lambda:gen_solution(100000, 100000, 100000),
    lambda:gen_solution(100000, 200000, 50000),
    lambda:gen_solution(100000, 200000, 50000),


    lambda:gen_solution_dag(50000, 150000),
    lambda:gen_solution_dag(70000, 200000),
    lambda:gen_solution_dag(100000, 200000),
    ]
    
    # tricky, no shuffle 
    gen_circle(100, 200, 20),
    gen_circle(100, 1000, 50),
    gen_circle(100, 90000, 50),
    gen_circle(1000, 5000, 100),

    gen_tricky(3, 50000, 0),
    gen_tricky(3, 50000, 1),
    gen_tricky(3, 50000, 2),

    gen_tricky(1, 2),
    gen_tricky(1, 3),
    gen_tricky(1, 5),
    gen_tricky(1, 1000),
    gen_tricky(1, 100000),

    gen_tricky(2, 100000, 0),
    gen_tricky(2, 100000, 1),
    gen_tricky(2, 100000, 2),

    gen_tricky(3, 100000, 0),
    gen_tricky(3, 100000, 1),
    gen_tricky(3, 100000, 2),
    
    gen_tricky(4, 65000),

    shuffle_tc(testcases)

def main():
  for testset in Testset.__subclasses__():
    testset()
  for subtask in Subtask.__subclasses__():
    subtask()

main()
