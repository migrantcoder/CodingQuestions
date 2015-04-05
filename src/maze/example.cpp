#include <iostream>

#include <maze/maze.h>

using namespace std;
using namespace maze;

#define ROWS 20
#define COLUMNS 30

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
    const size_t exit_row = atoi(argv[1]);
    const size_t exit_col = atoi(argv[2]);
    const size_t start_row = atoi(argv[3]);
    const size_t start_col = atoi(argv[4]);

    auto maze = generate<ROWS, COLUMNS>(exit_row, exit_col);
    const auto path = find_path(maze, start_row, start_col);
    mark_path(maze, path, start_row, start_col);

    cout << ROWS << "X" << COLUMNS << ", exit (" << exit_row << ", " << exit_col
            << "), start (" << start_row << ", " << start_col << ")" << endl;
    cout << maze << endl;

    return 0;
}
