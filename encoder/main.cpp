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
    if (!encoder.Encode(argv[1], argv[2]))
    {
        cout << "File could not be encoded" << endl;
    }

    return 0;
}
