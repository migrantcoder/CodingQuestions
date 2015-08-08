#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <array>
#include <iostream>

#pragma once

namespace nqueens {

using RowType = uintmax_t;

/// N (default 8) queens placement solver board.
///
/// The N queens must be placed on the board such that no queen can take
/// another. A queen may move horizontally, vertically or diagonally.
///
/// The solution is implemented using recursive backtracking to place a queen on
/// each row by recursively trying to place a queen on the next row for each
/// possible column of the current row.
///
/// @tparam N The width and height of the board. Defaults to 8. Must be less
///           than half the width in bits of the widest supported unsigned int
///           type.
/// @tparam RowType An unsigned integer type of at least 2N bits size. The 2N
///                 requirement is necessary to ensure 1 bits aren't lost on
///                 left shift when validating diagonals.
template <size_t N = 8>
class board {
public:
    static_assert(
            N <= (sizeof(RowType) * 8)/2,
            "N is not <= half of uintmax_t width");

    board() : rows_() { rows_.fill(0); }

    /// @return a board configured with a solution to the N queens problem.
    static board solve();

    /// @return the rows representing the board.
    const std::array<RowType, N>& rows() const { return rows_; }

    /// @param r The square's row.
    /// @param c The square's column.
    /// @return true iff the specified square is occupied.
    bool get(size_t r, size_t c) const { return rows_[r] & (1 << (N - c - 1)); }

    /// Print the board configuration.
    template <size_t N_>
    friend std::ostream& operator<<(std::ostream&, const board<N_>&);

private:
    void clear(size_t r, size_t c) { rows_[r] &= ~(1 << (N - c - 1)); }
    bool solve(size_t row);
    void set(size_t r, size_t c) { rows_[r] |= (1 << (N - c - 1)); }
    bool valid() const;

    std::array<RowType, N> rows_;
};

template<typename UIntType>
bool is_pow2(UIntType n)
{
    return (n != 0) && ((n & (n - 1)) == 0);
}

template <size_t N>
std::ostream& operator<<(std::ostream& os, const board<N>& b)
{
    for (size_t r = 0; r < N; ++r) {
        for (size_t c = 0; c < N; ++c) {
            os << static_cast<bool>(b.get(r, c)) << ' ';
        }
        os << std::endl;
    }
    return os;
}

template <size_t N>
bool board<N>::solve(size_t row)
{
    for (size_t col = 0; col < N; ++col) {
        set(row, col);
        if (valid()) {              // Valid configuration.
            if (row == N - 1) {
                return true;        // DONE! Placed queen on last row.
            }
            if (solve(row + 1)) {   // Successful so far, try place on next row.
                return true;        // DONE! Notify caller.
            }
        }
        clear(row, col);            // Backtrack by trying the next column.
    }
    return false;                   // Invalid configuration.
}

template <size_t N>
board<N> board<N>::solve()
{
    board<N> b;
    bool result = b.solve(0);
    assert(result);
    return b;
}


template <size_t N>
bool valid_colums(const std::array<RowType, N>& rows)
{
    RowType acc = 0;                        // OR accumulator
    for (size_t i = 0; i < N; ++i) {
        if (rows[i] & acc) {
            return false;
        } else {
            acc |= rows[i];
        }
    }
    return true;
}

template <size_t N>
bool valid_diagonals(const std::array<RowType, N>& rows)
{
    // RowType must be twice the number of bits as N so that 1 bits aren't lost
    // on left shift.

    auto copy = rows;

    // Validate up/right.
    for (size_t i = 0; i < (N - 1); ++i)
        copy[i] <<= (N - 1 - i);
    if (!valid_colums<N>(copy))
        return false;

    // Reset copy.
    copy = rows;

    // Validate down/right.
    for (size_t i = 1; i < N; ++i)
        copy[i] <<= i;
    return valid_colums<N>(copy);
}

template <size_t N>
bool board<N>::valid() const
{
    // Validate rows.
    for (size_t i = 0; i < N; ++i)
        if (rows_[i] != 0 && !is_pow2(rows_[i]))
            return false;

    return valid_colums<N>(rows_) && valid_diagonals<N>(rows_);
}

} // end namespace nqueens

