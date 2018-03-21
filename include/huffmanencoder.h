#pragma once

#include "huffmanfile.h"
#include "filewriter.h"
#include "tree.h"

class HuffmanEncoder
{
    static const int BUF_SIZE = 4096;
    static const int NUM_SYMBOLS = 256;
    static const unsigned int MAX_FILE_SIZE = -1;

public:
    bool Encode(const char* aInFileName, const char* aOutFileName)
    {
        File inFile(aInFileName);
        if (!inFile.Open(File::ReadOnlyBinary))
        {
            return false;
        }

        if (inFile.Size() > MAX_FILE_SIZE)
        {
            return false;
        }

        unsigned char buf[BUF_SIZE];
        CodeBook codeBook;
        {
            unsigned int freq[NUM_SYMBOLS] = {};
            while (auto size = inFile.Read((char*)buf, BUF_SIZE))
            {
                for (int i = 0; i < size; ++i)
                {
                    freq[buf[i]] += 1;
                }
            }

            Tree tree(freq, NUM_SYMBOLS);
            auto root = tree.GetRoot();
            if (!root)
            {
                return false;
            }

            codeBook.Init(root);
        }

        HuffmanHeader header{(unsigned int)inFile.Size(), codeBook.Encode()};
        if (header.codeBook.Count[0] == 0 || header.codeBook.Symbol.empty())
        {
            return false;
        }

        File outFile(aOutFileName);
        if (!outFile.Open(File::WriteOnlyBinary))
        {
            return false;
        }

        if (!header.Write(outFile))
        {
            return false;
        }

        inFile.Seek(0);
        FileWriter writer(outFile);
        while (auto size = inFile.Read((char*)buf, BUF_SIZE))
        {
            for (int i = 0; i < size; ++i)
            {
                writer.Write(codeBook.GetCodeWord(buf[i]));
            }
        }

        writer.Flush();

        return true;
    }
};
