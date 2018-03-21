#pragma once

#include "huffmanfile.h"
#include "filewriter.h"

class HuffmanEncoder
{
    static const int BUF_SIZE = 4096;
    static const int NUM_SYMBOLS = 256;
    static const unsigned int MAX_FILE_SIZE = -1;

public:
    HuffmanEncoder()
        : mLeafNode(256)
        , mInternalNode(512)
    {
        for (auto& node : mInternalNode)
        {
            node.IsLeaf = false;
        }
    }

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

            auto root = BuildTree(freq, NUM_SYMBOLS);
            codeBook.Init(root);
            DestroyNodes();
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

private:
    struct NodeCmp
    {
        bool operator()(const Node* lhs, const Node* rhs)
        {
            return lhs->Frequence > rhs->Frequence;
        }
    };

    Node* CreateLeafNode()
    {
        auto node = &mLeafNode[mCurrentLeafNode];
        ++mCurrentLeafNode;
        return node;
    }

    Node* CreateInternalNode()
    {
        auto node = &mInternalNode[mCurrentInternalNode];
        ++mCurrentInternalNode;
        return node;
    }

    void DestroyNodes()
    {
        mCurrentLeafNode = 0;
        mCurrentInternalNode = 0;
    }

    Node* BuildTree(const unsigned int* aFreq, int aSize)
    {
        std::priority_queue<Node*, std::vector<Node*>, NodeCmp> trees;
        for (int i = 0; i < aSize; ++i)
        {
            if(aFreq[i] != 0)
            {
                auto node = CreateLeafNode();
                node->Init(i, aFreq[i]);
                trees.push(node);
            }
        }

        while (trees.size() > 1)
        {
            auto childL = trees.top();
            trees.pop();

            auto childR = trees.top();
            trees.pop();

            auto internal = CreateInternalNode();
            internal->Init(childL, childR);
            trees.push(internal);
        }

        return trees.top();
    }

private:
    size_t mCurrentLeafNode = 0;
    size_t mCurrentInternalNode = 0;
    std::vector<Node> mLeafNode;
    std::vector<Node> mInternalNode;
};
