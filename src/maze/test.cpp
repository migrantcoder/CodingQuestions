#include <iostream>

#include <maze/find-path.h>
#include <maze/generate.h>
#include <maze/maze.h>

using namespace std;
using namespace maze;

/// Example generate and find and mark a path to the exit.
template <size_t R, size_t C>
maze<R, C> example(const size_t exit_row, const size_t exit_col)
{
    const coord start = {R - 1 - exit_row, C - 1 - exit_col};
    const coord exit = {exit_row, exit_col};


    auto maze = generate<R, C>(exit);
    const auto path_recursive = find_path_recursive(maze, start, exit);
    const auto path_iterative = find_path_iterative(maze, start, exit);
    assert(path_recursive == path_iterative);
    mark_path(maze, path_recursive);

    cout << "size: " << R << "X" << C << endl;
    cout << "exit: " << exit << endl;
    cout << "start: " << start << endl;
    cout << "maze: " << endl;
    cout << maze << endl;

    return maze;
}

/// Some sanity tests.
static void test()
{
    example<1, 1>(0, 0);

    example<3, 4>(0, 0);
    example<3, 4>(0, 3);
    example<3, 4>(2, 0);
    example<3, 4>(2, 3);
    example<3, 4>(1, 2);

    example<10, 10>(0, 0);
    example<10, 10>(0, 9);
    example<10, 10>(9, 0);
    example<10, 10>(9, 9);

    example<10, 10>(0, 0);
    example<10, 10>(0, 9);
    example<10, 10>(9, 0);
    example<10, 10>(9, 9);

    example<10, 10>(3, 5);
    example<10, 10>(4, 7);

    example<10, 10>(1, 1);
    example<10, 10>(8, 8);
    example<10, 10>(1, 8);
    example<10, 10>(8, 1);
}

int main(int argc, char** argv)
{
    test();
    return 0;
}
