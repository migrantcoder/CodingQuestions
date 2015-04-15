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

// Forward declarations.
struct coord;
std::ostream& operator<<(std::ostream&, const coord&);

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
};

bool operator==(const coord& lhs, const coord& rhs)
{
    return lhs.row == rhs.row && lhs.col == rhs.col;
}

bool operator<(const coord& lhs, const coord& rhs)
{
    return lhs.row != rhs.row ? lhs.row < rhs.row : lhs.col < rhs.col;
}

std::ostream& operator<<(std::ostream& o, const coord& c)
{
    return o << '(' << c.row << "," << c.col << ')';
}

/// Maze path.
using path = std::list<coord>;

std::ostream& operator<<(std::ostream& o, const path& p)
{
    for (const auto& coord : p)
        std::cout << coord << ' ';
    return std::cout << std::endl;
}

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

/// Maze room containing doors, exit and path data.
class room {
public:
    room() : doors_(0), exit_(0), start_(0),  path_(0) {}
    room(const room&) = default;
    room& operator=(const room&) = default;
    ~room() = default;

    void add_door(direction d) { doors_ |= d; }
    bool has_door(direction d) const { return doors_ & d; }
    void exit(bool exit) { exit_ = exit; }
    bool exit() const { return exit_; }
    void start(bool start) { start_ = start; }
    bool start() const { return start_; }
    void path(bool path) { path_ = path; }
    bool path() const { return path_; }

private:
    unsigned int doors_:4;      /// Bitfield for room's doors.
    unsigned int exit_:1;       /// The room is the exit.
    unsigned int start_:1;      /// The room is the start of the path.
    unsigned int path_:1;       /// The room is on the path.
};

/// A maze is an R(ows) by C(olumns) grid of rooms.
template <size_t R, size_t C> using maze = std::array<std::array<room,C>, R>;

// Forward declarations.
template <size_t R, size_t C> void foreach_room(maze<R, C>&, const std::function<void (room&)>&);
template <size_t R, size_t C> void clear_path(maze<R, C>&);

/// For all rooms.
template <size_t R, size_t C>
void foreach(maze<R, C>& m, const std::function<void (room&)>& f)
{
   for (size_t r = 0; r < R; ++r)
        for (size_t c = 0; c < C; ++c)
            f(m[r][c]);
}

/// Clear maze path.
template <size_t R, size_t C>
void clear_path(maze<R, C>& m)
{
    foreach(
            m,
            [] (room& r) {
                r.path(false);
                r.start(false);
                r.exit(false);
            });
}

template <size_t R, size_t C>
std::ostream& operator<<(std::ostream& os, const maze<R, C>& m)
{
    using namespace std;

    // For each row  ...
    for (size_t r = 0; r < R; ++r) {
        // ... for each room ...
        for (size_t c = 0; c < C; ++c) {
            // ... print up door/wall ...
            os << "+";
            os << (m[r][c].has_door(up) ? " " : "-");
            if (c == C - 1)
                os << "+";
        }
        os << endl;

        // ... for each room ...
        for (size_t c = 0; c < C; ++c) {
            // ... print left door/wall ...
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
                os << "|";      // ... print right wall.
        }
        os << endl;
    }

    // Bottom wall.
    for (size_t c = 0; c < C; ++c)
        os << "+-";
    return os << "+" << endl;
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
