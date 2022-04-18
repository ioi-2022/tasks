#include "testlib.h"
#include <bits/stdc++.h>

using namespace std;

const int kMaxW = 1'000'000'000;
const int kMaxM = 300'000;

void print_data(int N, const vector<int> &X, const vector<int> &Y, const vector<int> &W)
{
    assert(X.size() == Y.size());
    assert(Y.size() == W.size());

    cout << N << " " << W.size() << endl;
    for (int i = 0; static_cast<size_t>(i) < W.size(); i++)
    {
        assert(0 <= X[i] && X[i] < N);
        assert(0 <= Y[i] && Y[i] < N);
        assert(0 < W[i] && W[i] <= kMaxW);

        cout << X[i] << " " << Y[i] << " " << W[i] << endl;
    }
}

template <typename T>
void apply_permutation(vector<T> &v, const vector<int> &perm)
{
    vector<int> indices(perm);
    for (size_t i = 0; i < indices.size(); i++)
    {
        auto current = i;
        while (i != static_cast<size_t>(indices[current]))
        {
            auto next = indices[current];
            swap(v[current], v[next]);
            indices[current] = current;
            current = next;
        }
        indices[current] = current;
    }
}

void shuffle_data(vector<int> &X, vector<int> &Y, vector<int> &W)
{
    vector<int> idx(W.size());
    iota(idx.begin(), idx.end(), 0);

    shuffle(idx.begin(), idx.end());

    apply_permutation(X, idx);
    apply_permutation(Y, idx);
    apply_permutation(W, idx);
}

vector<int> gen_monotone_fish_1d(int N, int start, int minDelta, int maxDelta)
{
    assert(static_cast<long long>(minDelta) * maxDelta >= 0);
    assert(minDelta < maxDelta);

    vector<int> result;

    int current = start;
    for (int i = 0; i < N; i++)
    {
        result.push_back(clamp(current, 1, kMaxW));
        int delta = rnd.next(minDelta, maxDelta);
        current += delta;
    }

    return result;
}

int gen_fish(int probability, int maxW)
{
    int r = rnd.next(10);
    if (r < probability)
    {
        int w = rnd.next(1, maxW);
        return w;
    }
    return 0;
}

vector<pair<int, int>> gen_rand_fish_1d(int N, int maxM, int probability /* between 0 - 10 */, int maxW = kMaxW)
{
    vector<pair<int, int>> result;
    int m = 0;
    for (int i = 0; i < N && m < maxM; i++)
    {
        auto w = gen_fish(probability, maxW);
        if (w > 0)
        {
            result.emplace_back(i, w);
            m++;
        }
    }

    assert(result.size() <= static_cast<size_t>(maxM));
    return result;
}

void sum_all(int N, int maxM, int prob, int maxW = kMaxW)
{
    vector<int> X, Y, W;
    maxM = min(maxM, kMaxM);

    vector<size_t> xpos(rnd.perm((size_t)N / 2));
    std::transform(xpos.begin(), xpos.end(), xpos.begin(), [](auto x)
                   { return 2 * x; });

    for (int i = 0; static_cast<size_t>(i) < xpos.size(); ++i)
    {
        auto fish_column = gen_rand_fish_1d(N, maxM - W.size(), prob, maxW);

        for (auto [y, w] : fish_column)
        {
            X.push_back(xpos[i]);
            Y.push_back(y);
            W.push_back(w);
        }
    }

    shuffle_data(X, Y, W);
    print_data(N, X, Y, W);
}

void uniform_sum_all(int N, int maxM, int minW = 1, int maxW = kMaxW)
{
    vector<int> X, Y, W;
    maxM = min(maxM, kMaxM);

    set<pair<int, int>> pos;
    while (W.size() < static_cast<size_t>(maxM))
    {
        int x = 2 * rnd.next(N / 2 - 1);
        int y = rnd.next(N);
        if (pos.find(make_pair(x, y)) != pos.end())
            continue;

        pos.insert(make_pair(x, y));
        X.push_back(x);
        Y.push_back(y);
        if (minW < maxW)
            W.push_back(rnd.next(minW, maxW));
        else
            W.push_back(maxW);
    }

    assert(W.size() <= static_cast<size_t>(maxM));

    shuffle_data(X, Y, W);
    print_data(N, X, Y, W);
}

void exactX(int N, int maxM, int prob, const vector<int> &Xpos, int maxW = kMaxW)
{
    vector<int> X, Y, W;
    maxM = min(maxM, kMaxM);
    int m = 0;
    for (auto x : Xpos)
    {
        auto fish_column = gen_rand_fish_1d(N, maxM - m, prob, maxW);
        m += fish_column.size();
        for (auto [y, w] : fish_column)
        {
            X.push_back(x);
            Y.push_back(y);
            W.push_back(w);
        }
    }

    assert(W.size() <= static_cast<size_t>(maxM));
    shuffle_data(X, Y, W);
    print_data(N, X, Y, W);
}

void exactXY(int N, int maxM, int prob, int start0, int end0, int start1, int end1, int maxW = kMaxW)
{
    assert(start0 >= 0);
    assert(start1 >= 0);

    assert(end0 < N);
    assert(end1 < N);

    assert(start0 < end0);
    assert(start1 < end1);

    vector<int> X, Y, W;
    maxM = min(maxM, kMaxM);
    auto fish0 = gen_rand_fish_1d(N, maxM, prob, maxW);
    auto fish1 = gen_rand_fish_1d(N, maxM, prob, maxW);

    auto placeFish = [&](const vector<pair<int, int>> &fish, int x, int start, int end) -> void
    {
        for (auto [y, w] : fish)
        {
            if (start + y > end)
                break;
            X.push_back(x);
            Y.push_back(start + y);
            W.push_back(w);
        }
    };

    placeFish(fish0, 0, start0, end0);
    placeFish(fish1, 1, start1, end1);

    assert(W.size() <= kMaxM);
    shuffle_data(X, Y, W);
    print_data(N, X, Y, W);
}

void monotoneOneDim(int N, int start, int deltaRange)
{
    assert(0 <= start && start <= kMaxW);
    assert(abs(deltaRange) > 1);

    vector<int> X, Y, W;

    int minDelta, maxDelta;
    if (deltaRange > 0)
    {
        minDelta = 1;
        maxDelta = deltaRange;
    }
    else
    {
        minDelta = deltaRange;
        maxDelta = -1;
    }

    auto fish = gen_monotone_fish_1d(N, start, minDelta, maxDelta);
    int x = 0;
    for (auto f : fish)
    {
        if (x >= N)
            break;
        X.push_back(x++);
        Y.push_back(0);
        W.push_back(f);
    }

    shuffle_data(X, Y, W);
    print_data(N, X, Y, W);
}

void zigZagOneDim(int N, int segmentSize, int start, int deltaRange)
{
    assert(0 < segmentSize && segmentSize < N);
    assert(0 <= start && start <= kMaxW);

    int numSegment = static_cast<int>(N / static_cast<double>(segmentSize) + 0.5);
    vector<int> X, Y, W;
    bool increase = true;
    int x = 0;
    for (int i = 0; i < numSegment && x < N; ++i)
    {
        int minDelta = 1;
        int maxDelta = deltaRange;
        if (!increase)
        {
            minDelta -= (deltaRange + 1);
            maxDelta -= (deltaRange + 1);
            increase = true;
        }
        else
            increase = false;

        auto fish = gen_monotone_fish_1d(segmentSize, start, minDelta, maxDelta);
        for (auto f : fish)
        {
            if (x >= N)
                break;
            X.push_back(x);
            Y.push_back(0);
            W.push_back(f);
            x++;
        }

        start = fish.back() + (increase ? (-1) : 1) * rnd.next(deltaRange);
    }

    shuffle_data(X, Y, W);
    print_data(N, X, Y, W);
}

void bandsOneDim(int N, int bandwith, const vector<int> &probs)
{
    assert(0 < bandwith && bandwith <= N);
    assert(!probs.empty());

    vector<int> X, Y, W;
    int probIndex = 0;
    int xPos = 0;
    while (xPos + bandwith < N)
    {
        auto fish_gen = gen_rand_fish_1d(bandwith, bandwith, probs[probIndex]);
        for (auto [x, w] : fish_gen)
        {
            X.push_back(xPos + x);
            Y.push_back(0);
            W.push_back(w);
        }

        probIndex++;
        probIndex %= probs.size();
        xPos += bandwith;
    }

    shuffle_data(X, Y, W);
    print_data(N, X, Y, W);
}

void uniform2D(int N, int maxX, int maxY, int maxM = kMaxM, int maxW = kMaxW)
{
    vector<int> X, Y, W;
    maxM = min(maxM, kMaxM);

    set<pair<int, int>> pos;
    while (W.size() < static_cast<size_t>(maxM))
    {
        int x = rnd.next(min(N, maxX));
        int y = rnd.next(min(N, maxY));
        if (pos.find(make_pair(x, y)) != pos.end())
            continue;

        pos.insert(make_pair(x, y));
        X.push_back(x);
        Y.push_back(y);
        W.push_back(rnd.next(1, maxW));
    }

    assert(W.size() <= static_cast<size_t>(maxM));
    shuffle_data(X, Y, W);
    print_data(N, X, Y, W);
}

void checkered(int N, int nCol, int M)
{
    vector<int> X, Y, W;
    int maxM = min(static_cast<long long>(N) * (N / 2), static_cast<long long>(kMaxM));
    M = min(M, maxM);

    for (int i = 0; i < nCol; i++)
    {
        set<pair<int, int>> pos;
        int m = 0;
        while (m < M / nCol)
        {
            int x = rnd.next(N / nCol);
            int y = rnd.next(N / 2);
            if (pos.find(make_pair(x, y)) != pos.end())
                continue;

            pos.insert(make_pair(x, y));
            m++;
            X.push_back(N / nCol * i + x);
            auto currentY = i % 2 == 0 ? y : N / 2 + y;
            Y.push_back(currentY);
            W.push_back(rnd.next(1, kMaxW));
        }
    }

    shuffle_data(X, Y, W);
    print_data(N, X, Y, W);
}

void full_grid(int N, int maxY, int maxW = kMaxW)
{
    assert(N * maxY <= kMaxM);

    vector<int> X, Y, W;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < min(maxY, N); j++)
        {
            int w = rnd.next(1, maxW);
            X.push_back(i);
            Y.push_back(j);
            W.push_back(w);
        }
    }

    assert(W.size() <= kMaxM);
    shuffle_data(X, Y, W);
    print_data(N, X, Y, W);
}

void bandsTwoDims(int N, int bandwith, int gapwidth, int prob)
{
    vector<int> X, Y, W;
    int x = bandwith;
    while (x < N)
    {
        for (int i = x; i < x + bandwith && i < N; i++)
        {
            auto fish = gen_rand_fish_1d(N, kMaxM - W.size(), prob);
            for (auto [y, w] : fish)
            {
                X.push_back(i);
                Y.push_back(y);
                W.push_back(w);
            }
        }
        x += bandwith + gapwidth;
    }

    assert(W.size() <= kMaxM);
    shuffle_data(X, Y, W);
    print_data(N, X, Y, W);
}

void zigZagTwoDims(int N, int segmentSize, int prob, int inverse = 0)
{
    assert(0 < segmentSize && segmentSize < N);

    vector<int> X, Y, W;

    bool increase = true;
    bool stay = false;

    int currentSegmentSize = rnd.next(segmentSize / 2, segmentSize);
    int currentMinH = rnd.next(segmentSize / 2);
    int currentH = currentMinH;
    for (int x = 0; x < N && W.size() < kMaxM; x++)
    {
        if (!stay)
        {
            if (increase)
                currentH++;
            else
                currentH--;
        }

        if (currentH == min(currentSegmentSize, N - 1))
        {
            increase = false;
            stay = rnd.next(10) <= 3;
        }
        if (currentH <= currentMinH)
        {
            increase = true;
            stay = rnd.next(10) <= 3;
        }

        vector<pair<int, int>> fish;
        if (inverse == 0)
            fish = gen_rand_fish_1d(currentH, kMaxM - W.size(), prob);
        else
            fish = gen_rand_fish_1d(N / 2 - currentH, kMaxM - W.size(), prob);

        for (auto [y, w] : fish)
        {
            X.push_back(x);
            if (inverse == 0)
                Y.push_back(y);
            else
                Y.push_back(N - y - 1);
            W.push_back(w);
        }
    }

    assert(W.size() <= kMaxM);

    shuffle_data(X, Y, W);
    print_data(N, X, Y, W);
}

void randomMaxPerColumn(int N, int maxPerColumn)
{
    vector<int> X, Y, W;
    for (int i = 0; i < N; i++)
    {
        int perColumn = rnd.next(maxPerColumn + 1);

        set<int> yPos;
        while (yPos.size() < static_cast<size_t>(perColumn))
        {
            int y = rnd.next(N);
            if (yPos.find(y) != yPos.end())
                continue;

            yPos.insert(y);
        }

        for (auto y : yPos)
        {
            X.push_back(i);
            Y.push_back(y);
            W.push_back(rnd.next(1, kMaxW - 1));
        }
    }

    assert(W.size() <= kMaxM);
    shuffle_data(X, Y, W);
    print_data(N, X, Y, W);
}

void zigZagMax2PerColumn(int N, int segmentWidth)
{
    assert(0 < segmentWidth && segmentWidth < N);

    vector<int> X, Y, W;
    int currentH = 1;
    bool increase = true;
    for (int x = 0; x < N && W.size() < kMaxM; x++)
    {
        if (increase)
            currentH++;
        else
            currentH--;

        if (currentH == segmentWidth)
            increase = false;
        if (currentH <= 1)
            increase = true;

        X.push_back(x);
        Y.push_back(currentH);
        W.push_back(rnd.next(1, kMaxW - 1));

        if (W.size() < kMaxM)
        {
            if (rnd.next(2) == 0)
            {
                X.push_back(x);
                Y.push_back(currentH + 1);
                W.push_back(rnd.next(1, kMaxW - 1));
            }
            else
            {
                X.push_back(x);
                Y.push_back(currentH - 1);
                W.push_back(rnd.next(1, kMaxW - 1));
            }
        }
    }

    shuffle_data(X, Y, W);
    print_data(N, X, Y, W);
}

void diagonal(int N)
{
    vector<int> X(N - 1), Y(N - 1), W(N - 1);
    for (int i = 0; i < N - 1; ++i)
    {
        X[i] = Y[i] = i;
        W[i] = 1;
    }

    shuffle_data(X, Y, W);
    print_data(N, X, Y, W);
}

int main(int argc, char *argv[])
{
    registerGen(argc, argv, 0);

    if (strcmp(argv[1], "sum-all") == 0)
    {
        int N = atoi(argv[2]);
        int maxM = atoi(argv[3]);
        int prob = atoi(argv[4]);
        if (argc >= 6)
        {
            int maxW = atoi(argv[5]);
            sum_all(N, maxM, prob, maxW);
        }
        else
            sum_all(N, kMaxW, prob);
    }
    else if (strcmp(argv[1], "sum-all-uniform") == 0)
    {
        int N = atoi(argv[2]);
        int maxM = atoi(argv[3]);
        int minW = atoi(argv[4]);
        if (argc >= 6)
        {
            int maxW = atoi(argv[5]);
            uniform_sum_all(N, maxM, minW, maxW);
        }
        else
            uniform_sum_all(N, maxM, minW);
    }
    else if (strcmp(argv[1], "two-columns-full") == 0)
    {
        int N = atoi(argv[2]);
        int maxM = atoi(argv[3]);
        int prob = atoi(argv[4]);
        int loc = atoi(argv[5]);
        int maxW = atoi(argv[6]);
        if (loc == 0 || loc == 1)
            exactX(N, maxM, prob, {loc}, maxW);
        else
            exactX(N, maxM, prob, {0, 1}, maxW);
    }
    else if (strcmp(argv[1], "two-columns-diag") == 0)
    {
        int N = atoi(argv[2]);
        int maxM = atoi(argv[3]);
        int prob = atoi(argv[4]);
        int start0 = atoi(argv[5]);
        int end0 = atoi(argv[6]);
        int start1 = atoi(argv[7]);
        int end1 = atoi(argv[8]);
        if (argc >= 10)
        {
            int maxW = atoi(argv[9]);
            exactXY(N, maxM, prob, start0, end0, start1, end1, maxW);
        }
        else
            exactXY(N, maxM, prob, start0, end0, start1, end1);
    }
    else if (strcmp(argv[1], "monotone-onedim") == 0)
    {
        int N = atoi(argv[2]);
        int start = atoi(argv[3]);
        int deltaRange = atoi(argv[4]);
        monotoneOneDim(N, start, deltaRange);
    }
    else if (strcmp(argv[1], "zigzag-onedim") == 0)
    {
        int N = atoi(argv[2]);
        int segmentSize = atoi(argv[3]);
        int start = atoi(argv[4]);
        int deltaRange = atoi(argv[5]);
        zigZagOneDim(N, segmentSize, start, deltaRange);
    }

    else if (strcmp(argv[1], "bands-onedim") == 0)
    {
        vector<int> probs;
        int N = atoi(argv[2]);
        int bandWith = atoi(argv[3]);
        probs.push_back(atoi(argv[4]));
        probs.push_back(atoi(argv[5]));
        if (argc >= 7)
        {
            probs.push_back(atoi(argv[6]));
        }
        bandsOneDim(N, bandWith, probs);
    }

    else if (strcmp(argv[1], "uniform2d") == 0)
    {
        int N = atoi(argv[2]);
        int maxX = atoi(argv[3]);
        int maxY = atoi(argv[4]);
        int maxM = atoi(argv[5]);
        int maxW = atoi(argv[6]);
        uniform2D(N, maxX, maxY, maxM, maxW);
    }

    else if (strcmp(argv[1], "full-grid") == 0)
    {
        int N = atoi(argv[2]);
        int maxY = atoi(argv[3]);
        int maxW = atoi(argv[4]);
        full_grid(N, maxY, maxW);
    }

    else if (strcmp(argv[1], "bands-2d") == 0)
    {
        int N = atoi(argv[2]);
        int bandwidth = atoi(argv[3]);
        int gapwidth = atoi(argv[4]);
        int prob = atoi(argv[5]);
        bandsTwoDims(N, bandwidth, gapwidth, prob);
    }

    else if (strcmp(argv[1], "zigzag-2d") == 0)
    {
        int N = atoi(argv[2]);
        int segmentSize = atoi(argv[3]);
        int prob = atoi(argv[4]);
        int inverse = atoi(argv[5]);
        zigZagTwoDims(N, segmentSize, prob, inverse);
    }

    else if (strcmp(argv[1], "max-percolumn") == 0)
    {
        int N = atoi(argv[2]);
        int maxPerColumn = atoi(argv[3]);
        randomMaxPerColumn(N, maxPerColumn);
    }
    else if (strcmp(argv[1], "zigzag-max-2-per-column") == 0)
    {
        int N = atoi(argv[2]);
        int segmentWidth = atoi(argv[3]);
        zigZagMax2PerColumn(N, segmentWidth);
    }
    else if (strcmp(argv[1], "checkered") == 0)
    {
        int N = atoi(argv[2]);
        int nCol = atoi(argv[3]);
        int M = atoi(argv[4]);
        checkered(N, nCol, M);
    }
    else if (strcmp(argv[1], "diagonal") == 0)
    {
        int N = atoi(argv[2]);
        diagonal(N);
    }
    else
        return 1;

    return 0;
}
