// vim: set ts=4 sw=4 tw=80 expandtab
// Copyright 2015 Migrant Coder

/// Generate a maze in a grid.
///
/// See README description.

#pragma once

#include <maze/maze.h>

namespace maze {

// Forward declaration.
template <size_t R, size_t C> void generate_rec(maze<R, C>&, size_t, size_t);

/// Generate a maze in a R(ows) by C(olumns) grid.
///
/// \param exit the exit room's co-ordinates.
/// \return a grid containing the generated maze.
template <size_t R, size_t C>
maze<R, C> generate(const coord& exit)
{
    assert(exit.row < R && exit.col < C);

    maze<R, C> m;
    m[exit.row][exit.col].exit(true);
    generate_rec(m, exit);
    clear_path_and_visited(m);

    return m;
}

/// Generate a maze using recursive exploration and backtracking.
///
/// \param m the maze grid.
/// \param current the co-ordinates of the current room.
template <size_t R, size_t C>
void generate_rec(maze<R, C>& m, const coord& current)
{
    const auto directions = get_shuffled_directions();
    room& current_room = m[current.row][current.col];
    current_room.visited(true);

    // For each neighbour ...
    for (const auto& direction : directions) {
        const auto d = delta(direction);
        const coord next = 
                {
                    current.row + d.first,
                    current.col + d.second
                };

        // Only visit a valid, unvisited neighbour.
        if (!(next.row >= 0 && next.row < R && next.col >= 0 && next.col < C)) {
            continue;
        }

        // Only visit an unvisited neighbour.
        room& next_room = m[next.row][next.col];
        if (next_room.visited()) {
            continue;
        }

        // Create doors (bi-directional) to extend the maze.
        current_room.add_door(direction);
        next_room.add_door(reverse(direction));

        // Recurse ...
        generate_rec(m, next);
    }
}

} // namespace maze
