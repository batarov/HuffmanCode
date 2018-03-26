#pragma once

#include "huffmanfile.h"
#include "filereader.h"
#include "filewriter.h"

class HuffmanDecoder
{
public:
    int Decode(FileReader& aReader, const EncodedCanonicalCodeBook& aCodeBook)
    {
        int code, first, count, index;
        code = first = index = 0;
        for (size_t i = 1; i < aCodeBook.Count.size(); ++i)
        {
            auto bit = aReader.ReadBit();
            code |= bit;
            count = aCodeBook.Count[i];
            if (code - count < first)
            {
                return aCodeBook.Symbol[index + (code - first)];
            }

            index += count;
            first += count;
            first <<= 1;
            code <<= 1;
        }

        return -1;
    }

    bool Decode(const char* aInFileName, const char* aOutFileName)
    {
        File inFile(aInFileName);
        if (!inFile.Open(File::ReadOnlyBinary))
        {
            return false;
        }

        HuffmanHeader header;
        if (!header.Load(inFile))
        {
            return false;
        }

        File outFile(aOutFileName);
        if (!outFile.Open(File::WriteOnlyBinary))
        {
            return false;
        }

        FileReader reader(inFile);
        FileWriter writer(outFile);
        auto dataSize = header.length - 1;
        unsigned char ch;
        for (size_t i = 0; i < dataSize; ++i)
        {
            ch = Decode(reader, header.codeBook);
            writer.WriteByte(ch);
        }

        ch = Decode(reader, header.codeBook);
        writer.WriteByte(ch, true);

        return true;
    }
};
