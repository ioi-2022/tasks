from enum import Enum, auto

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


class LOC(Enum):
    LEFT = 0
    RIGHT = 1
    BOTH = 2


globalMaxN = 100000
globalmaxM = 300000
globalMaxW = 1000000000

cubicMaxN = 300
quadraticMaxN = 3000
maxfullNSquared = int(globalmaxM**0.5)
smallN1 = 1000
smallN2 = 3500
betweenCubicAndLinearN = 50000
maxPerColumn = 2
maxColumnLinearN = 100000
bigN = 90000

maxSmallY = 8

probEmpty = 0
probLow1 = 2
probLow2 = 3
probMedium = 5
probHigh1 = 8
probHigh2 = 9
probFull = 10

smallW = 1000
mediumW = 2000000

intersectNum = 3

tinySegment = 2
mediumSegment = 5

narrowBand = 10
mediumBand = 500
wideBand = 10000


@indexify_testcases
def gen_sum_all(N, maxM, prob, maxW=globalMaxW, testcase_index=0):
    print(f"gen sum-all {N} {maxM} {prob} {maxW} {testcase_index}")


@indexify_testcases
def gen_uniform_sum_all(N, maxM, minW, maxW=globalMaxW, testcase_index=0):
    print(f"gen sum-all-uniform {N} {maxM} {minW} {maxW} {testcase_index}")


@indexify_testcases
def gen_two_columns_full(N, maxM, prob, loc, maxW=globalMaxW, testcase_index=0):
    print(
        f"gen two-columns-full {N} {maxM} {prob} {loc.value} {maxW} {testcase_index}")


@indexify_testcases
def gen_two_columns_diag(N, maxM, prob, pos0, pos1, maxW=globalMaxW, testcase_index=0):
    print(
        f"gen two-columns-diag {N} {maxM} {prob} {pos0[0]} {pos0[1]} {pos1[0]} {pos1[1]} {maxW} {testcase_index}")


@indexify_testcases
def gen_monotone_onedim(N, start, deltaRange, testcase_index=0):
    print(f"gen monotone-onedim {N} {start} {deltaRange} {testcase_index}")


@indexify_testcases
def gen_zigzag_onedim(N, segmentSize, start, deltaRange, testcase_index=0):
    print(
        f"gen zigzag-onedim {N} {segmentSize} {start} {deltaRange} {testcase_index}")


@indexify_testcases
def gen_bands_onedim(N, bandWith, probs, testcase_index=0):
    assert(len(probs) == 2 or len(probs) == 3)
    if len(probs) == 2:
        print(
            f"gen bands-onedim {N} {bandWith} {probs[0]} {probs[1]} {testcase_index}")
    else:
        print(
            f"gen bands-onedim {N} {bandWith} {probs[0]} {probs[1]} {probs[2]} {testcase_index}")


@indexify_testcases
def gen_uniform2D(N, maxX, maxY, maxM=globalmaxM, maxW=globalMaxW, testcase_index=0):
    maxM = min(globalmaxM, maxM)
    print(f"gen uniform2d {N} {maxX} {maxY} {maxM} {maxW} {testcase_index}")


@indexify_testcases
def gen_full_grid(N, maxY, maxW=globalMaxW, testcase_index=0):
    print(f"gen full-grid {N} {maxY} {maxW} {testcase_index}")


@indexify_testcases
def gen_bands_twodims(N, bandwith, gapwidth, prob, testcase_index=0):
    print(f"gen bands-2d {N} {bandwith} {gapwidth} {prob} {testcase_index}")


@indexify_testcases
def gen_zigzag_twodims(N, segmentSize, prob, inverse, testcase_index=0):
    print(f"gen zigzag-2d {N} {segmentSize} {prob} {inverse} {testcase_index}")


@indexify_testcases
def gen_max_percolumn(N, perColumn, testcase_index=0):
    print(f"gen max-percolumn {N} {perColumn} {testcase_index}")


@indexify_testcases
def gen_zigzag_max2percolumn(N, segmentWidth, testcase_index=0):
    print(f"gen zigzag-max-2-per-column {N} {segmentWidth} {testcase_index}")


@indexify_testcases
def gen_checkered(N, nCol, M, testcase_index=0):
    print(f"gen checkered {N} {nCol} {M} {testcase_index}")


@indexify_testcases
def gen_diagonal(N, testcase_index=0):
    print(f"gen diagonal {N} {testcase_index}")



class TestsetSamples(Testset):
    def __init__(self):
        super().__init__()
        manual("sample-1")


class TestsetSumAll(Testset):
    def __init__(self):
        super().__init__()
        gen_sum_all(globalMaxN, globalmaxM, probHigh2)
        gen_uniform_sum_all(globalMaxN, globalmaxM, globalMaxW, globalMaxW)


class TestsetManualN2(Testset):
    def __init__(self):
        super().__init__()
        manual("N2-main-diag-1")
        manual("N2-main-diag-2")
        manual("N2-anti-diag-1")
        manual("N2-anti-diag-2")


class TestsetManualN3(Testset):
    def __init__(self):
        super().__init__()
        manual("N3-main-diag")
        manual("N3-anti-diag")


class TestsetFirstColumnOfTwoColumns(Testset):
    def __init__(self):
        super().__init__()
        gen_two_columns_full(bigN, bigN, probHigh2, LOC.LEFT)
        gen_two_columns_full(globalMaxN, globalMaxN, probFull, LOC.LEFT)


class TestsetSecondColumnOfTwoColumns(Testset):
    def __init__(self):
        super().__init__()
        gen_two_columns_full(bigN, bigN, probHigh2, LOC.RIGHT)
        gen_two_columns_full(globalMaxN, globalMaxN, probFull, LOC.RIGHT)


class TestsetBothColumnsOfTwoColumns(Testset):
    def __init__(self):
        super().__init__()
        gen_two_columns_full(bigN, 2*bigN, probHigh2, LOC.BOTH)
        gen_two_columns_full(globalMaxN, 2*globalMaxN, probFull, LOC.BOTH)


class TestsetTwoColumnsMainDiag(Testset):
    def __init__(self):
        super().__init__()
        gen_two_columns_diag(bigN, bigN, probHigh2, [
                             0, int(bigN/2) - intersectNum], [int(bigN/2) + intersectNum, bigN-1])
        gen_two_columns_diag(globalMaxN, globalMaxN + 2*intersectNum, probHigh2, [
                             0, int(globalMaxN/2) + intersectNum], [int(globalMaxN/2) - intersectNum, globalMaxN-1])


class TestsetTwoColumnsAntiDiag(Testset):
    def __init__(self):
        super().__init__()
        gen_two_columns_diag(bigN, bigN, probHigh2, [
                             int(bigN/2) + intersectNum, bigN-1], [0, int(bigN/2)-intersectNum])
        gen_two_columns_diag(globalMaxN, globalMaxN + 2*intersectNum, probHigh2, [
                             int(globalMaxN/2) - intersectNum, globalMaxN-1], [int(globalMaxN/2) + intersectNum, globalMaxN-1])


class TestsetMonotoneOneDim(Testset):
    def __init__(self):
        super().__init__()
        gen_monotone_onedim(globalMaxN, 5*smallW, smallW)
        gen_monotone_onedim(globalMaxN, 10*smallW, -smallW)


class TestsetZigZagOneDim(Testset):
    def __init__(self):
        super().__init__()
        gen_zigzag_onedim(globalMaxN, mediumSegment, mediumW, smallW)
        gen_zigzag_onedim(globalMaxN, int(globalMaxN/5), mediumW, smallW)


class TestsetBandsOneDim(Testset):
    def __init__(self):
        super().__init__()
        gen_bands_onedim(bigN, mediumBand, [probHigh1, probEmpty, probFull])
        gen_bands_onedim(globalMaxN, wideBand, [
                         probLow1, probEmpty, probFull])


class TestsetUniformCubicSmallY(Testset):
    def __init__(self):
        super().__init__()
        gen_uniform2D(int(cubicMaxN/2), int(cubicMaxN/2),
                      maxSmallY, maxSmallY*cubicMaxN/3)
        gen_uniform2D(cubicMaxN, cubicMaxN, maxSmallY, maxSmallY*cubicMaxN/2)


class TestsetZigZagCubicSmallY(Testset):
    def __init__(self):
        super().__init__()
        gen_zigzag_twodims(int(cubicMaxN/4), maxSmallY/2, probHigh1, 0)
        gen_zigzag_twodims(cubicMaxN, maxSmallY, probHigh1, 0)


class TestsetUniform2dCubic(Testset):
    def __init__(self):
        super().__init__()
        gen_uniform2D(cubicMaxN, cubicMaxN, cubicMaxN,
                      int((cubicMaxN * cubicMaxN) / 10))
        gen_uniform2D(cubicMaxN, cubicMaxN, cubicMaxN,
                      int((cubicMaxN * cubicMaxN) / 3))


class TestsetUniform2dQuadratic(Testset):
    def __init__(self):
        super().__init__()
        gen_uniform2D(quadraticMaxN, quadraticMaxN, quadraticMaxN,
                      int((quadraticMaxN * quadraticMaxN) / 10))
        gen_uniform2D(quadraticMaxN, quadraticMaxN, quadraticMaxN,
                      int((quadraticMaxN * quadraticMaxN) / 3))


class TestsetUniform2dFull(Testset):
    def __init__(self):
        super().__init__()
        gen_uniform2D(globalMaxN, globalMaxN, globalMaxN)


class TestsetMaxCubicSmallY(Testset):
    def __init__(self):
        super().__init__()
        gen_full_grid(int(cubicMaxN/2), int(maxSmallY/2))
        gen_full_grid(cubicMaxN, maxSmallY)


class TestsetMaxCubic(Testset):
    def __init__(self):
        super().__init__()
        gen_full_grid(cubicMaxN, int(cubicMaxN/2))
        gen_full_grid(cubicMaxN, cubicMaxN)


class TestsetMaxFull(Testset):
    def __init__(self):
        super().__init__()
        gen_full_grid(maxfullNSquared, maxfullNSquared)


class TestsetBandsTwoDimsCubic(Testset):
    def __init__(self):
        super().__init__()
        gen_bands_twodims(cubicMaxN/4, int(cubicMaxN/20),
                          int(cubicMaxN/25), probHigh1)
        gen_bands_twodims(cubicMaxN, int(cubicMaxN/15),
                          int(cubicMaxN/30), probHigh1)
        gen_bands_twodims(cubicMaxN, int(cubicMaxN/5),
                          int(cubicMaxN/8), probHigh2)


class TestsetBandsTwoDimsQuadratic(Testset):
    def __init__(self):
        super().__init__()
        gen_bands_twodims(
            int(quadraticMaxN/4), int(quadraticMaxN/20), int(quadraticMaxN/25), probHigh1)
        gen_bands_twodims(quadraticMaxN, 5, int(quadraticMaxN/30), probHigh1)


class TestsetBandsTwoDimsFull(Testset):
    def __init__(self):
        super().__init__()
        gen_bands_twodims(bigN, 5, int(bigN/20), probLow1)
        gen_bands_twodims(globalMaxN, 5,
                          int(globalMaxN/30000), probLow1)


class TestsetCheckeredCubic(Testset):
    def __init__(self):
        super().__init__()
        gen_checkered(cubicMaxN, int(cubicMaxN/10), globalmaxM)
        gen_checkered(cubicMaxN, 4, globalmaxM)


class TestsetCheckeredQuadratic(Testset):
    def __init__(self):
        super().__init__()
        gen_checkered(quadraticMaxN, int(quadraticMaxN/10), globalmaxM)
        gen_checkered(quadraticMaxN, 4, globalmaxM)


class TestsetCheckeredFull(Testset):
    def __init__(self):
        super().__init__()
        gen_checkered(bigN, int(bigN/10), globalmaxM)
        gen_checkered(globalMaxN, 8, globalmaxM)


class TestsetZigZagTwoDimsCubic(Testset):
    def __init__(self):
        super().__init__()
        gen_zigzag_twodims(cubicMaxN, cubicMaxN/15, probLow2, 0)
        gen_zigzag_twodims(cubicMaxN, cubicMaxN/10, probLow1, 1)


class TestsetZigZagTwoDimsQuadratic(Testset):
    def __init__(self):
        super().__init__()
        gen_zigzag_twodims(quadraticMaxN, quadraticMaxN/15, probLow2, 0)
        gen_zigzag_twodims(quadraticMaxN, quadraticMaxN/10, probLow1, 1)


class TestsetZigZagTwoDimsFull(Testset):
    def __init__(self):
        super().__init__()
        gen_zigzag_twodims(globalMaxN, globalMaxN/2000, probLow2, 0)
        gen_zigzag_twodims(globalMaxN, globalMaxN/4000, probHigh1, 0)


class TestsetZigZagMax2PerColumnLinear(Testset):
    def __init__(self):
        super().__init__()
        gen_zigzag_max2percolumn(maxColumnLinearN, mediumSegment)
        gen_zigzag_max2percolumn(maxColumnLinearN, int(maxColumnLinearN/20))


class TestsetRandomMaxColumnLinear(Testset):
    def __init__(self):
        super().__init__()
        gen_max_percolumn(maxColumnLinearN, maxPerColumn)


class TestsetFullMaxColumnLinear(Testset):
    def __init__(self):
        super().__init__()
        gen_full_grid(betweenCubicAndLinearN, maxPerColumn)
        gen_full_grid(maxColumnLinearN, maxPerColumn)


class TestsetMaxNLeftCorners(Testset):
    def __init__(self):
        super().__init__()
        manual("maxN-bottom-left")
        manual("maxN-top-left")


class TestsetMaxNRightCorners(Testset):
    def __init__(self):
        super().__init__()
        manual("maxN-bottom-right")
        manual("maxN-top-right")


class TestsetMaxNCorners(Testset):
    def __init__(self):
        super().__init__()
        include(TestsetMaxNLeftCorners)
        include(TestsetMaxNRightCorners)


class SubtaskSamples(Subtask):
    name = "samples"

    def __init__(self):
        super().__init__()
        include(TestsetSamples)


class SubtaskSumAll(Subtask):
    name = "sum-all"

    def __init__(self):
        super().__init__()
        include(TestsetMaxNLeftCorners)
        include(TestsetSumAll)
        include(TestsetFirstColumnOfTwoColumns)


class SubtaskTwoColumns(Subtask):
    name = "two-columns"

    def __init__(self):
        super().__init__()
        manual('N3-main-diag')
        include(TestsetMaxNLeftCorners)
        include(TestsetManualN2)
        include(TestsetFirstColumnOfTwoColumns)
        include(TestsetSecondColumnOfTwoColumns)
        include(TestsetBothColumnsOfTwoColumns)
        include(TestsetTwoColumnsMainDiag)
        include(TestsetTwoColumnsAntiDiag)


class SubtaskOneDim(Subtask):
    name = "one-dim"

    def __init__(self):
        super().__init__()
        manual('maxN-bottom-left')
        manual('maxN-bottom-right')
        include(TestsetMonotoneOneDim)
        include(TestsetZigZagOneDim)
        include(TestsetBandsOneDim)


class SubtaskCubicSmallY(Subtask):
    name = "cubic-small-y"

    def __init__(self):
        super().__init__()
        include(SubtaskSamples)
        include(TestsetManualN2)
        include(TestsetManualN3)
        include(TestsetUniformCubicSmallY)
        include(TestsetZigZagCubicSmallY)
        include(TestsetMaxCubicSmallY)
        gen_diagonal(4)
        gen_diagonal(maxSmallY)


class SubtaskCubic(Subtask):
    name = "cubic"

    def __init__(self):
        super().__init__()
        include(SubtaskCubicSmallY)
        include(TestsetUniform2dCubic)
        include(TestsetMaxCubic)
        include(TestsetBandsTwoDimsCubic)
        include(TestsetZigZagTwoDimsCubic)
        include(TestsetCheckeredCubic)
        gen_diagonal(cubicMaxN)


class SubtaskQuadratic(Subtask):
    name = "quadratic"

    def __init__(self):
        super().__init__()
        include(SubtaskCubic)
        include(TestsetUniform2dQuadratic)
        include(TestsetMaxFull)
        include(TestsetBandsTwoDimsQuadratic)
        include(TestsetZigZagTwoDimsQuadratic)
        include(TestsetCheckeredQuadratic)
        gen_diagonal(quadraticMaxN)


class SubtaskMaxPerColumn(Subtask):
    name = "max-per-column"

    def __init__(self):
        super().__init__()
        include(TestsetManualN2)
        include(TestsetManualN3)
        include(TestsetFullMaxColumnLinear)
        include(TestsetRandomMaxColumnLinear)
        include(TestsetZigZagMax2PerColumnLinear)
        include(TestsetMaxNCorners)
        include(SubtaskSamples)
        include(SubtaskOneDim)
        gen_diagonal(maxColumnLinearN)


class SubtaskFull(Subtask):
    name = "full"

    def __init__(self):
        super().__init__()
        include(SubtaskSumAll)
        include(SubtaskTwoColumns)
        include(SubtaskQuadratic)
        include(SubtaskMaxPerColumn)
        include(TestsetUniform2dFull)
        include(TestsetBandsTwoDimsFull)
        include(TestsetZigZagTwoDimsFull)
        include(TestsetCheckeredFull)


def main():
    for testset in Testset.__subclasses__():
        testset()
    for subtask in Subtask.__subclasses__():
        subtask()


main()
