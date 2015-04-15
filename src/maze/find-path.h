// vim: set ts=4 sw=4 tw=80 expandtab
// Copyright 2015 Migrant Coder

#pragma once

#include <set>
#include <stack>

#include <maze/maze.h>

namespace maze {

// Forward declaration.
template <size_t R, size_t C>
path find_path_iterative(const maze<R, C>&, const coord&, const coord&);
template <size_t R, size_t C>
path find_path_recursive(const maze<R, C>&, const coord&, const coord&);

/// Find a path to the exit starting at the specified room using DFS.
///
/// \param m The maze to explore.
/// \param s The start room co-ordinates.
/// \param e The exit room co-ordinates.
/// \return the path to the exit from the specified start.
///
/// \note Alias either recursive or iteration implementation.
template <size_t R, size_t C>
path find_path(const maze<R, C>& m, const coord& s, const coord& e)
{
    return find_path_iterative(m, s, e);
}

/// Find a path to the exit starting at the specified room using DFS implemented
/// using iteration.
///
/// \see \c find_path
template <size_t R, size_t C>
path find_path_iterative(const maze<R, C>& maze, const coord& start, const coord& exit)
{
    using namespace std;

    // A stack entry consists of the co-ordinates of the rooms to explore and
    // the path to their predecessor.
    struct entry {
        ::maze::path path;
        list<coord> coordinates;
    };

    set<coord> visited;
    stack<entry> stack;
    stack.push({{}, {start}});
    while (!stack.empty()) {
        const struct entry entry = move(stack.top());
        stack.pop();
        for (const auto& coord : entry.coordinates) {
            if (visited.find(coord) != end(visited))
                continue;
            else
                visited.insert(coord);

            if (coord == exit) {
                // Found!
                path solution = move(entry.path);
                solution.push_back(coord);
                return solution;
            }

            // Create a stack entry to explore the current room's neigbours.
            // The entry must contain the path to the current room and a list of
            // coordinates to the current room's neighbours.
            struct entry next_entry = {entry.path, {}};
            next_entry.path.push_back(coord);
            const auto& room = maze[coord.row][coord.col];
            for (auto direction : {up, right, down, left}) {
                if (!room.has_door(direction))
                    continue;
                auto d = delta(direction);
                struct coord next_coord = {coord.row + d.first, coord.col + d.second};
                next_entry.coordinates.emplace_back(next_coord);
            }
            stack.emplace(move(next_entry));
        }
    }

    return {};
}

/// Find a path to the exit starting at the specified room using DFS implemented
/// using recursion.
///
/// \note Maze size limited by call stack size.
///
/// \see \c find_path
template <size_t R, size_t C>
path find_path_recursive(const maze<R, C>& m, const coord& s, const coord& e)
{
    std::set<coord> visited;
    return find_path_recursive_(m, s, e, visited);
}

template <size_t R, size_t C>
path find_path_recursive_(
        const maze<R, C>& maze,
        const coord& current,
        const coord& exit,
        std::set<coord>& visited)
{
    const room& room = maze[current.row][current.col];
    if (visited.find(current) != std::end(visited))
        return {};                                                  // Stop, pop and backtrack.
    else
        visited.insert(current);

    if (current == exit) {
        return {current};                                           // Found, stop and pop.
    }

    for (auto direction : {up, right, down, left}) {
        if (!room.has_door(direction))
            continue;

        const auto d = delta(direction);
        const coord next = {current.row + d.first, current.col + d.second};

        path p = find_path_recursive_(maze, next, exit, visited);   // Recurse.
        if (!p.empty()) {
            // Done. The recursive call found the exit and returned a path.
            p.push_front(current);
            return p;
        }
    }

    return {};
}

} // namespace maze
