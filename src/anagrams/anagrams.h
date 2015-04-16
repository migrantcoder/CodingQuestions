// vim: set ts=4 sw=4 tw=80 expandtab
// Copyright 2015 Migrant Coder

/// Find Anagrams
///
/// See README for problem statement and solution description.

#pragma once

#include <algorithm>
#include <list>
#include <map>
#include <string>

/// Canonicalize a word by lexicographically sorting its letters.
///
/// \param word The word to canonicalize.
/// \return the canonicalization.
std::string canonicalize(const std::string& word)
{
    auto c = word;
    std::sort(begin(c), end(c));
    return c;
}

/// Find anagrams.
///
/// \param words A list of words without duplicates.
/// \return all words that are anagrams of other words in the list, in their
/// original order.
std::list<std::string> find_anagrams(const std::list<std::string>& words)
{
    using namespace std;

    // Build a table of cononicalized word occurrence counts.
    map<string, size_t> counts;
    for (const auto& w : words) {
        const auto c = canonicalize(w);
        auto iter = counts.find(c);
        if (iter == end(counts))
            counts.insert({c, 1});
        else
            iter->second++;
    }

    // Anagrams are the words who's canonicalized form occurrs more than once.
    list<string> anagrams;
    copy_if(
            begin(words),
            end(words),
            back_inserter(anagrams),
            [&counts] (const string& word)
            {
                const auto& iter = counts.find(canonicalize(word));
                return iter != end(counts) && iter->second > 1;
            });
    return anagrams;
}
