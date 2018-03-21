#include <iostream>

#include <huffmanencoder.h>

using namespace std;

static size_t GetFileSize(const char* aFilename)
{
    File file(aFilename);
    return file.Open(File::ReadOnlyBinary) ? file.Size() : 0;
}

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
    else
    {
        auto size = GetFileSize(argv[1]);
        auto packedSize = GetFileSize(argv[2]);

        cout << "Size " << size << " Packed size " << packedSize << " Ratio " << ((double)packedSize / size) << endl;
    }

    return 0;
}
