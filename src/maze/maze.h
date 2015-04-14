// vim: set ts=4 sw=4 tw=80 expandtab
// Copyright 2015 Migrant Coder

/// Maze
///
/// See README for problem and solution description.

#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <deque>
#include <iosfwd>
#include <list>
#include <random>

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

/// Grid coordinates.
struct coord {
    size_t row;
    size_t col;

    friend std::ostream& operator<<(std::ostream&, const coord&);
};

std::ostream& operator<<(std::ostream& o, const coord& c)
{
    return o << '(' << c.row << "," << c.col << ')';
}

/// Maze path.
using path = std::list<coord>;

/// \return row and column deltas to move in the specified direction.
std::pair<int, int> delta(direction d)
{
    if (d == up)
        return {-1, 0};
    if (d == down)
        return {1, 0};
    if (d == right)
        return {0, 1};
    return {0, -1};
}

std::string to_string(direction d)
{
    if (d == up)
        return "u";
    if (d == down)
        return "d";
    if (d == right)
        return "r";
    assert(d = left);
    return "l";
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
    return right;
}

/// Maze room containing door directions and visit and path metadata.
class room {
public:
    room() : doors_(0), exit_(0),  path_(0), visited_(0) {}
    room(const room&) = default;
    room& operator=(const room&) = default;
    ~room() = default;

    void add_door(direction d) { doors_ |= d; }
    bool has_door(direction d) const { return doors_ & d; }
    void exit(bool exit) { exit_ = exit; }
    bool exit() const { return exit_; }
    void start(bool start) { start_ = start; }
    bool start() const { return start_; }
    void visited(bool visited) { visited_ = visited; }
    bool visited() const { return visited_; }
    void path(bool path) { path_ = path; }
    bool path() const { return path_; }

private:
    unsigned int doors_:4;      /// Bitfield for doors.
    unsigned int exit_:1;       /// Node is the exit.
    unsigned int start_:1;      /// Node is the path start.
    unsigned int path_:1;       /// Node on path.
    unsigned int visited_:1;    /// Node visited.
};

/// A maze is an R(ows) by C(olumns) grid of rooms.
template <size_t R, size_t C> using maze = std::array<std::array<room,C>, R>;

// Forward declarations.
template <size_t R, size_t C> path find_path_rec(maze<R, C>& m, const coord&);
template <size_t R, size_t C> void foreach_room(maze<R, C>&, const std::function<void (room&)>&);
template <size_t R, size_t C> void clear_path_and_visited(maze<R, C>&);

/// For all rooms.
template <size_t R, size_t C>
void foreach_room(maze<R, C>& m, const std::function<void (room&)>& f)
{
   for (size_t r = 0; r < R; ++r)
        for (size_t c = 0; c < C; ++c)
            f(m[r][c]);
}

/// Clear path membership and visited flags in every room in the maze.
template <size_t R, size_t C>
void clear_path_and_visited(maze<R, C>& m)
{
    foreach_room(
            m,
            [] (room& r) {
                r.path(false);
                r.start(false);
                r.visited(false);
            });
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
            } else if (room.start()) {
                os << "S";
            } else if (room.path()) {
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
/// \note Recursive implementation.  Limited by call stack size.
///
/// \param maze
/// \param current current/start room co-ordinates.
/// \return the path to the exit from the specified start.
template <size_t R, size_t C>
path find_path(const maze<R, C>& m, const coord& current)
{
    auto copy = m;
    auto path = find_path_rec(copy, current);
    return path;
}

template <size_t R, size_t C>
path find_path_rec(maze<R, C>& m, const coord& current)
{
    room& r = m[current.row][current.col];
    r.visited(true);

    if (r.exit())
        return {current};                                   // Stop.

    for (auto direction : {up, right, down, left}) {
        if (!r.has_door(direction))
            continue;

        const auto d = delta(direction);
        const coord next = {current.row + d.first, current.col + d.second};
        const auto& next_room = m[next.row][next.col];

        if (next_room.visited())
            continue;                                       // Skip.

        path p = find_path_rec(m, next);                    // Recurse.
        if (!p.empty()) {
            p.push_front(current);
            return p;
        }
    }

    return {};
}

/// Mark the path.
///
/// \param maze A maze with an exit.
/// \param path A valid path to the exit.
///
/// \post \c maze is modified by marking rooms on the path exit.
template <size_t R, size_t C>
void mark_path(maze<R, C>& m, const path& path)
{
    if (!path.empty())
        m[path.front().row][path.front().col].start(true);
    for (const auto& room : path)
        m[room.row][room.col].path(true);
}

} // namespace maze
