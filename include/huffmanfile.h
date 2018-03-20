#pragma once

#include "codebook.h"
#include "file.h"

struct HuffmanHeader
{
    unsigned int length;
    CanonicalCodeBook codeBook;

    bool Write(File& aFile)
    {
        if (!aFile.Write((char*)&length, sizeof(length)))
        {
            return false;
        }

        if (!aFile.Write((char*)codeBook.Count.data(), codeBook.Count[0] + 1))
        {
            return false;
        }

        auto numSymbols = codeBook.Symbol.size();
        if (!aFile.Write((char*)&numSymbols, 1))
        {
            return false;
        }

        if (!aFile.Write((char*)codeBook.Symbol.data(), numSymbols))
        {
            return false;
        }

        return true;
    }

    bool Load(File& aFile)
    {
        if (aFile.Read((char*)&length, sizeof(length)) != sizeof(length))
        {
            return false;
        }

        auto& count = codeBook.Count;
        if (aFile.Read((char*)count.data(), 1) != 1)
        {
            return false;
        }

        int size = count[0];
        assert(size > 0 && size < 16);
        if (size == 0 || aFile.Read((char*)count.data() + 1, size) != size)
        {
            return false;
        }

        if (aFile.Read((char*)&size, 1) != 1)
        {
            return false;
        }

        auto& symbol = codeBook.Symbol;
        symbol.resize(size != 0 ? size : 256);
        if (aFile.Read((char*)symbol.data(), symbol.size()) != (int)symbol.size())
        {
            return false;
        }

        return true;
    }
};
