#pragma once

#include <array>
#include <queue>

#include "node.h"

struct EncodedCanonicalCodeBook
{
    std::vector<unsigned char> Count;
    std::vector<unsigned char> Symbol;

    void AddSymbol(unsigned char aSymbol)
    {
        Symbol.push_back(aSymbol);
    }
};

struct CodeWord
{
    int Bits = 0;
    unsigned int Code = 0;
};

struct CodeBookEntry
{
    CodeWord Code;
    unsigned char Symbol;

    CodeBookEntry(unsigned char aSymbol, int aBits)
        : Symbol(aSymbol)
    {
        Code.Bits = aBits;
    }

    void SetCode(unsigned int aCode)
    {
        Code.Code = aCode;
    }

    friend bool operator<(const CodeBookEntry& lhs, const CodeBookEntry& rhs);
};

inline bool operator<(const CodeBookEntry& lhs, const CodeBookEntry& rhs)
{
    return lhs.Code.Bits == rhs.Code.Bits ? lhs.Symbol < rhs.Symbol : lhs.Code.Bits < rhs.Code.Bits;
}

class CodeBook
{
public:
    void Init(const Node* aRoot)
    {
        mEntry.clear();
        mCodeWord.fill(nullptr);

        GenerateEntries(aRoot, 0);

        std::sort(mEntry.begin(), mEntry.end());

        unsigned int code = 0;
        auto nextBits = mEntry[0].Code.Bits;
        for (size_t i = 0; i < mEntry.size(); ++i)
        {
            mEntry[i].SetCode(code);
            if (i + 1 < mEntry.size())
            {
                nextBits = mEntry[i + 1].Code.Bits;
            }

            code = (code + 1) << (nextBits - mEntry[i].Code.Bits);
        }

        for (const auto& entry : mEntry)
        {
            mCodeWord[entry.Symbol] = &entry.Code;
        }
    }

    const CodeWord* GetCodeWord(unsigned char aSymbol) const
    {
        return mCodeWord[aSymbol];
    }

    EncodedCanonicalCodeBook Encode()
    {
        if (mEntry.empty())
        {
            return {};
        }

        EncodedCanonicalCodeBook book;
        auto maxBits = mEntry.back().Code.Bits;
        book.Count.resize(maxBits + 1);
        book.Count[0] = maxBits;
        for (const auto& entry : mEntry)
        {
            book.Count[entry.Code.Bits] += 1;
            book.AddSymbol(entry.Symbol);
        }

        return book;
    }

private:
    void GenerateEntries(const Node* aNode, int aBits)
    {
        if (aNode->IsLeaf())
        {
            mEntry.push_back(CodeBookEntry(aNode->Symbol, aBits == 0 ? 1 : aBits));
        }
        else
        {
            GenerateEntries(aNode->Left, aBits + 1);
            GenerateEntries(aNode->Right, aBits + 1);
        }
    }

private:
    std::vector<CodeBookEntry> mEntry;
    std::array<const CodeWord* , 256> mCodeWord;
};
