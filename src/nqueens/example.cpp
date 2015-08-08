#include <nqueens/nqueens.h>

int main(int argc, char** argv)
{
    using namespace std;
    using namespace nqueens;

    auto b = board<8>::solve();
    cout << b << endl;
    return 0;
}
