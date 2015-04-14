#include <iostream>
#include <sstream>

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
            "Usage: %s EXIT_ROW EXIT_COLUMN START_ROW START_COLUMN\n"
            "    EXIT_ROW is in 0..%d\n"
            "    EXIT_COLUMN is in 0..%d\n"
            "    START_ROW is in 0..%d\n"
            "    START_COLUMN is in 0..%d\n";
    if (argc < 5 ) {
        fprintf(stderr, USAGE, argv[0], ROWS, COLUMNS, ROWS, COLUMNS);
        return 1;
    }
    const coord exit = {strtosize(argv[1]), strtosize(argv[2])};
    const coord start = {strtosize(argv[3]), strtosize(argv[4])};

    auto maze = generate<ROWS, COLUMNS>(exit);
    const auto path = find_path(maze, start);
    mark_path(maze, path);

    cout << ROWS << "X" << COLUMNS << ", exit " << exit << ", start " << start << endl;
    cout << "path: ";
    for (const coord& c : path)
        cout << c << ' ';
    cout << endl;
    cout << maze << endl;

    return 0;
}
