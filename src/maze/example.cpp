#include <iostream>
#include <sstream>

#include <maze/find-path.h>
#include <maze/generate.h>
#include <maze/maze.h>

using namespace std;
using namespace maze;

#define ROWS 20
#define COLUMNS 30

size_t strtosize(const char*  const s)
{
    std::istringstream is(s);
    size_t size;
    is >> size;
    return size;
}

int main(int argc, char** argv)
{
    constexpr static const char* USAGE =
            "Usage: %s EXIT_ROW EXIT_COLUMN START_ROW START_COLUMN [recursive]\n"
            "    EXIT_ROW is in [0..%d)\n"
            "    EXIT_COLUMN is in [0..%d)\n"
            "    START_ROW is in [0..%d)\n"
            "    START_COLUMN is in [0..%d)\n";
    if (argc < 5 ) {
        fprintf(stderr, USAGE, argv[0], ROWS, COLUMNS, ROWS, COLUMNS);
        return 1;
    }

    const string impl = argc > 5 ? string(argv[5]) : "iterative";
    if (impl != "recursive" && impl != "iterative") {
        fprintf(stderr, USAGE, argv[0], ROWS, COLUMNS, ROWS, COLUMNS);
        return 1;
    }

    const coord exit = {strtosize(argv[1]), strtosize(argv[2])};
    const coord start = {strtosize(argv[3]), strtosize(argv[4])};
    if (!(exit.row < ROWS && exit.col < COLUMNS &&
                start.row < ROWS && start.col < COLUMNS)) {
        fprintf(stderr, USAGE, argv[0], ROWS, COLUMNS, ROWS, COLUMNS);
        return 1;
    }

    auto maze = generate<ROWS, COLUMNS>(exit);
    const auto path =
            (impl == "recursive" ?
                    find_path_recursive(maze, start, exit) :
                    find_path_iterative(maze, start, exit));
    mark_path(maze, path);

    cout << "size: " << ROWS << "X" << COLUMNS << endl;
    cout << "exit: " << exit << endl;
    cout << "start: " << start << endl;
    cout << "implementation: " << impl << endl;
    cout << "path: " << path << endl;
    cout << "maze:" << endl;
    cout << maze << endl;

    return 0;
}
