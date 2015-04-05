// vim: set ts=4 sw=4 tw=80 expandtab
// Copyright 2014 Migrant Coder 

#include <topk/topk.h>

#include <cstdio>

using namespace std;
using namespace topk;

/// Generate \c n elements.
std::unique_ptr<element[]> generate(const size_t n)
{
    using namespace std;

    srand(time(nullptr));
    auto es = make_unique<element[]>(n);
    for (size_t i = 0; i < n; ++i) {
        element& e = es[i];
        e._id = i;
        e._rating = rand() %  (MAX_RATING + 1);
    }
    return es;
}

/// Run example. 
///
/// N elements with random scores are generated. The top K rated elements are
/// printed in descending order.
///
/// \pre N > K
/// \param N The total number of elements.
/// \param K The number of top rated elements, K.
/// \param S The solution to run.
static void run_example(const size_t N, const size_t K, topk_t S)
{
    auto es = generate(N);
    auto ts = S(es.get(), N, K);
    for (const auto t : ts) {
        cout << t << endl;
    }
}

/// Run an example with the optionally specified parameters.
int main(const int argc, const char** const argv)
{
    constexpr static const char* USAGE =
            "Usage: %s [N [K [naive]]]\n" \
            "    N - the number of elements (generated)\n" \
            "    K - the number of top rated elements\n" \
            "    naive - use the naive O(N log N) solution over the O(N log K) solution\n";

    const int n = argc > 1 ? atoi(argv[1]) : 100;
    const int k = argc > 2 ? atoi(argv[2]) : 10;
    const bool naive = argc > 3 && !strcmp("naive", argv[3]);
    if (n <= 0 || k <= 0 || (argc > 3 && !naive)) {
        fprintf(stderr, USAGE, argv[0]);
        return 1;
    }

    const size_t N = static_cast<size_t>(n);
    const size_t K = static_cast<size_t>(k);
    topk_t* S = naive ? topk_by_sort : topk_by_pq;

    run_example(N, K, S);

    return 0;
}
