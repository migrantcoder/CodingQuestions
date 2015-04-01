// vim: set ts=4 sw=4 tw=80 expandtab
// Copyright 2015 Migrant Coder

/// Generate and Explore a Maze
///
/// Definition
///
/// A maze is a grid of 4 sided rooms that may be connected to their neighbours
/// by doors.  One room is marked as the exit
///
/// Generation & Representation
///
/// The maze is represented by a 2-dimensional (row by columns) array of room
/// objects.  The first (zeroeth) row is the top of the maze and the first
/// (zeroeth) columns is the left of the maze.
///
/// Each room object indicates whether it has doors to any of to the up, right,
/// down and left neighbours.  Rooms track other metadata, such as whether they
/// have been visited during or are on a path.
///
/// To generate a maze, start at a specified exit room.  Perform a depth first
/// exploration of the grid marking nodes as visited as they are encountered and
/// creating doors between the rooms being traversed.  Backtrack when no move
/// can be made from the current room, the room is surrounded by maze edges and
/// visited rooms.
///
/// Find Exit
///
/// A recursive depth first search is used to find the exit room from the
/// specified start room.  Each room on the path is marked.

#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <iosfwd>
#include <list>
#include <random>
#include <vector>

namespace maze {

/// Directions.
enum direction: uint8_t { up = 1, right = 2, down = 4, left = 8 };

std::array<direction, 4> get_shuffled_directions()
{
    using namespace std;

    static random_device device;
    static mt19937_64 generator;

    array<direction, 4> directions = {{ up, right, down, left }};
    shuffle(begin(directions), end(directions), generator);
    return directions;
}

/// \return row and column deltas to move in the specified direction.
std::pair<int, int> delta(direction d)
{
    using namespace std;

    if (d == up)
        return make_pair(-1, 0);
    if (d == down)
        return make_pair(1, 0);
    if (d == right)
        return make_pair(0, 1);
    assert(d = left);
    return make_pair(0, -1);
}

std::string to_string(direction d)
{
    if (d == up)
        return "up";
    if (d == down)
        return "down";
    if (d == right)
        return "right";
    assert(d = left);
    return "left";

}

/// \return the reverse of the specified direction \c d.
direction reverse(direction d)
{
    if (d == up)
        return down;
    if (d == down)
        return up;
    if (d == right)
        return left;
    assert(d = left);
    return right;
}

/// Maze room containing door directions and visit and path metadata.
class room {
public:
    room() : doors_(0), exit_(0), visited_(0), exit_path_(0) {}

    void add_door(direction d) { doors_ |= d; }
    bool has_door(direction d) const { return doors_ & d; }
    void exit(bool exit) { exit_ = exit; }
    bool exit() const { return exit_; }
    void visited(bool visited) { visited_ = visited; }
    bool visited() const { return visited_; }
    void exit_path(bool exit_path) { exit_path_ = exit_path; }
    bool exit_path() const { return exit_path_; }

private:
    unsigned int doors_:4;      /// Bitfield for directional doors' existence.
    unsigned int exit_:1;       /// Node is the exit.
    unsigned int visited_:1;    /// Node visited during maze generation.
    unsigned int exit_path_:1;  /// Node on exit path.
};

/// A maze is an R(ows) by C(olumns) grid of rooms.
template <size_t R, size_t C> using maze = std::array<std::array<room,C>, R>;

// Forward declarations.
template <size_t R, size_t C> void generate_rec(maze<R, C>&, size_t, size_t);
template <size_t R, size_t C> void foreach_room(maze<R, C>&, const std::function<void (room&)>&);

/// Generate a maze in a R(ows) by C(olumns) grid.
///
/// \param exit_row the exit room's row.
/// \param exit_col the exit room's column.
/// \return a grid containing the generated maze.
template <size_t R, size_t C>
maze<R, C> generate(size_t exit_row, size_t exit_col)
{
    assert(exit_row < R && exit_col < C);

    maze<R, C> m;

    m[exit_row][exit_col].exit(true);
    generate_rec(m, exit_row, exit_col);

    // Clear visited rooms.
    foreach_room(m, [] (room& r) { r.visited(false); });

    return m;
}

/// Generate a maze recusrive exploration and backtracking.
///
/// \param m the maze grid.
/// \param row the row co-ordinate of the current room.
/// \param col the column co-ordinate of the current room.
template <size_t R, size_t C>
void generate_rec(maze<R, C>& m, const size_t row, const size_t col)
{
    using namespace std;

    const auto directions = get_shuffled_directions();
    room& current = m[row][col];
    current.visited(true);

    // For each neighbour ...
    for (const auto& direction : directions) {
        const auto d = delta(direction);
        const int next_row = row + d.first;
        const int next_col = col + d.second;

        // Only visit a valid, unvisited neighbour.
        if (!(next_row >= 0 && next_row < R && next_col >= 0 && next_col < C)) {
            continue;
        }

        // Only visit an unvisited neighbour.
        room& next = m[next_row][next_col];
        if (next.visited()) {
            continue;
        }

        // Create doors (bi-directional) to extend the maze.
        current.add_door(direction);
        next.add_door(reverse(direction));

        // Recurse ...
        generate_rec(m, next_row, next_col);
    }
}

/// For all rooms.
template <size_t R, size_t C>
void foreach_room(maze<R, C>& m, const std::function<void (room&)>& f)
{
    for (size_t r = 0; r < R; ++r) {
        for (size_t c = 0; c < C; ++c) {
            f(m[r][c]);
        }
    }
}

template <size_t R, size_t C>
std::ostream& operator<<(std::ostream& os, const maze<R, C>& m)
{
    using namespace std;

    // For each row's rooms  ...
    for (size_t r = 0; r < R; ++r) {
        // ... print up door/wall ...
        for (size_t c = 0; c < C; ++c) {
            os << "+";
            os << (m[r][c].has_door(up) ? " " : "-");
            if (c == C - 1)
                os << "+";
        }
        os << endl;

        // ... print left door/wall ...
        for (size_t c = 0; c < C; ++c) {
            const auto& room = m[r][c];

            os << (room.has_door(left) ? " " : "|");

            if (room.exit()) {
                os << "X";
            } else if (room.exit_path()) {
                os << "*";
            } else {
                os <<  " ";
            }

            if (c == C - 1)
                os << "|";
        }
        os << endl;
    }

    // Maze's bottom wall.
    for (size_t c = 0; c < C; ++c) {
        os << "+-";
    }
    os << "+" << endl;

    return os;
}

/// Find a path to the exit starting at the specified room.
///
/// \note Recursive implementation. Limited by call stack size.
///
/// \param maze
/// \param row current/start room row co-ordinate.
/// \param col current/start room column co-ordinate.
/// \return \c true iff the exit was found.
///
/// \post \c maze is modified by the marking of rooms on the discovered path to
/// the exit.
template <size_t R, size_t C>
bool find_exit(maze<R, C>& m, const size_t row, const size_t col)
{
    using namespace std;

    static const array<direction, 4> directions = {{ up, right, down, left }};

    room& r = m[row][col];

    // Successful base case.
    if (r.exit()) {
        r.exit_path(true);
        return true;
    }

    r.visited(true);

    for (auto direction : directions) {
        if (!r.has_door(direction))
            continue;

        const auto d = delta(direction);
        const int next_row = row + d.first;
        const int next_col = col + d.second;

        if (m[next_row][next_col].visited())
            continue;

        if (find_exit(m, next_row, next_col)) {
            r.exit_path(true);
            return true;
        }
    }

    return false;
}

} // namespace maze
