#include <cassert>
#include <iostream>

#include <maze/maze.h>

using namespace std;
using namespace maze;

void test_no_exit()
{
    room r1;
    room r2;
    room r0 = { {&r1, &r2} };

    path result = find_exit(&r0);

    assert(result.empty());
}

void test_3_deep()
{
    room r0010, r0011;
    room r001 = { {&r0010, &r0011, exit_door} };
    room r000, r002;
    room r00 = { {&r000, &r001, &r002} };
    room r01, r02;
    room r0 = { {&r00, &r01, &r02} };
    path control = { 0, 1, 2 };

    path result = find_exit(&r0);

    assert(result == control);
}

void test_singleton()
{

    room r0 = { {exit_door} };
    path control = { 0 };

    path result = find_exit(&r0);

    assert(result == control);
}

int main(int argc, char** argv)
{
    test_no_exit();
    test_3_deep();
    test_singleton();
    return 0;
}

