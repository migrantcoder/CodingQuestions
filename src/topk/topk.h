// vim: set ts=4 sw=4 tw=132 expandtab
// Copyright 2014 Migrant Coder 

/// Top K
///
/// See README for problem and solution description.

#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include <topk/minheap.h>

namespace topk {

typedef uint64_t id_t;
typedef uint8_t rating_t;

constexpr static const rating_t MAX_RATING = 100;

/// An uniquely identified element with a rating.
struct element {
    element() : _id(0), _rating(0) {}
    element(const id_t id, const rating_t rating) : _id(id), _rating(rating) {}
    element(const element&) = default;
    ~element() = default;

    id_t _id;
    rating_t _rating;
} __attribute__((packed));

bool operator<(const element& a, const element& b) { return a._rating < b._rating; }
bool operator>(const element& a, const element& b) { return a._rating > b._rating; }

std::ostream& operator<<(std::ostream& o, const element& e)
{
    return o << "{ id : " << e._id << ", rating : " << static_cast<int>(e._rating) << " }";
}

/// Return the top K rated elements rating order.
///
/// \pre n > k
/// \param es The elements.
/// \param n The number of elements.
/// \param k The top K elements to return.
/// \return the top \code min(k, n) \encode rated elements in descending order of rating.
std::vector<element> topk(const element* const es, const size_t n, const size_t k);

/// Signature for solution functions.
typedef decltype(topk) topk_t;

/// Implement \c topk using a priority queue to track top K.
///
/// Runtime O(n) = n log k.
std::vector<element> topk_by_pq(const element* const es, const size_t n, const size_t k)
{
    using namespace std;

    // Find the top K rated elements.
    minheap<element> ts;
    for (size_t i = 0; i < n; ++i) {
        const auto& e = es[i];
        if (ts.size() < k) {
            ts.push(e);
        } else if (ts.top() < e) {
            ts.pop();
            ts.push(e);
        }
    }

    // Return in descending order.
    vector<element> topk;
    topk.reserve(k);
    while (!ts.empty()) {
        topk.push_back(ts.top());
        ts.pop();
    }
    reverse(begin(topk), end(topk));
    return topk;
}

/// Implement \c topk by sorting all N elements.
///
/// Runtime O(n) = n log n.
std::vector<element> topk_by_sort(const element* const es, const size_t n, const size_t k)
{
    using namespace std;

    vector<element> copied;
    copied.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        copied.push_back(es[i]);
    }
    sort(begin(copied), end(copied));
    reverse(begin(copied), end(copied));
    vector<element> topk(begin(copied), begin(copied) + min(n, k));
    return topk;
}

}   // namespace topk
