#include <anagrams/anagrams.h>

#include <cctype>
#include <iostream>

using namespace std;

list<string> from_stdin()
{
    list<string> words;
    while (!cin.eof()) {
        string word;
        char c;
        while (!cin.eof()) {
            cin.get(c);
            if (!isspace(c))
                word.push_back(c);
            else
                break;
        }
        if (word.size() > 0)
            words.push_back(word);
    }
    return words;
}

list<string> from_args(const int argc, const char** const argv)
{
    list<string> words;
    for (int i = 1; i < argc; ++i)
        words.push_back(argv[i]);
    return words;
}

int main(int argc, const char** argv)
{
    const auto words = argc > 1 ? from_args(argc, argv) : from_stdin();
    const auto anagrams = find_anagrams(words);
    for (const auto& a : anagrams)
        cout << a << " ";
    cout << endl;
    return 0;
}
