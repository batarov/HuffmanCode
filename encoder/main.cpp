#include <iostream>
#include <cassert>

#include <huffmanencoder.h>

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        cout << "usage: encode <input-file> <output-file>" << endl;
        return 0;
    }

    HuffmanEncoder encoder;
    assert(encoder.Encode(argv[1], argv[2]));

    return 0;
}
