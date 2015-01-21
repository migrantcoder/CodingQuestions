// vim: set ts=4 sw=4 tw=80 expandtab
// Copyright 2015 Migrant Coder

/// Explore a Maze
///
/// The problem:
///     A maze consists of rooms connected to their neighbours by doors. Each
///     room's doors are number 0 to N-1 where N is the number of doors leading
///     out of the room. Starting in an arbitrary room, find a path to the exit.
///
///     A path is a sequence of numbers, indicating the door to take at each
///     step of the path.
///
///     Find a path to the exit.
///
/// Solutions:
///     Recursive depth first search.

#pragma once

#include <list>
#include <vector>

namespace maze {

struct room;

/// A path is a sequence of door numbers.
using path=std::list<size_t>;

/// Doors are pointers to adjoining rooms.
using doors=std::vector<room*>;

/// The exit door has the special value "exit_door".
constexpr static room* const exit_door = nullptr;

/// A room has "doors" to adjoining rooms or "neighbours".
struct room {
    doors neighbours_;
};

/// Find a path to the exit starting at the specified room.
///
/// @note Recursive implementation. Limited by call stack size.
///
/// @param start The room to start in.
/// @return a sequence of door numbers defining the path.
path find_exit(const room* const  start)
{
    path p;
    for (size_t i = 0; i < start->neighbours_.size(); ++i) {
        // Base case.
        if (start->neighbours_[i] == exit_door) {
            p.push_front(i);
            break;
        }

        // Recursive case.
        p = find_exit(start->neighbours_[i]);
        if (!p.empty()) {
            // Exit has been found, add door to the path.
            p.push_front(i);
            break;
        }
    }
    return p;
}

inline std::ostream& operator<<(std::ostream& o, const path& p)
{
    for (const auto& n : p) {
        o << n << ' ';
    }
    return o;
}

} // namespace maze
