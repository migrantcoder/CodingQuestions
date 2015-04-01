#include <cassert>
#include <iostream>

#include <maze/maze.h>

using namespace std;
using namespace maze;

template <size_t R, size_t C>
void test_generate_maze()
{
}

/// Test generate and find exit.
template <size_t R, size_t C>
void test(const size_t exit_row, const size_t exit_col)
{
    auto maze = generate<R, C>(exit_row, exit_col);

    const size_t start_row = R - 1 - exit_row;
    const size_t start_col = C - 1 - exit_col;
    const bool found_exit = find_exit(maze, start_row, start_col);
    
    assert(found_exit);

    cout << R << "X" << C << " exit (" << exit_row << ", " << exit_col
            << "), start from (" << start_row << ", " << start_col << ")"
            << endl;
    cout << maze;
    cout << endl;
}

int main(int argc, char** argv)
{
    test<1, 1>(0, 0);
    
    test<3, 4>(0, 0);
    test<3, 4>(0, 3);
    test<3, 4>(2, 0);
    test<3, 4>(2, 3);
    test<3, 4>(1, 2);

    test<10, 10>(0, 0);
    test<10, 10>(0, 9);
    test<10, 10>(9, 0);
    test<10, 10>(9, 9);

    test<10, 10>(0, 0);
    test<10, 10>(0, 9);
    test<10, 10>(9, 0);
    test<10, 10>(9, 9);

    test<10, 10>(3, 5);
    test<10, 10>(4, 7);

    test<10, 10>(1, 1);
    test<10, 10>(8, 8);
    test<10, 10>(1, 8);
    test<10, 10>(8, 1);

    test<20, 30>(15, 20);

    return 0;
}
