#include <iostream>
#include <cassert>

#include <huffmandecoder.h>

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        cout << "usage: decode <input-file> <output-file>" << endl;
        return 0;
    }

    HuffmanDecoder decoder;
    if (!decoder.Decode(argv[1], argv[2]))
    {
        cout << "File could not be decoded" << endl;
    }

    return 0;
}
