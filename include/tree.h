#pragma once

#include "node.h"

class Tree
{
public:
    Tree(const unsigned int* aFreq, size_t aSize)
        : mLeafNode(256)
        , mInternalNode(512)
    {
        if (aFreq && aSize != 0)
        {
            for (auto& node : mInternalNode)
            {
                node.IsLeaf = false;
            }

            mRoot = Build(aFreq, aSize);
        }
    }

    const Node* GetRoot() const
    {
        return mRoot;
    }

private:
    Node* Build(const unsigned int* aFreq, size_t aSize)
    {
        std::priority_queue<Node*, std::vector<Node*>, NodeCmp> trees;
        for (size_t i = 0; i < aSize; ++i)
        {
            if (aFreq[i] != 0)
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

private:
    size_t mCurrentLeafNode = 0;
    size_t mCurrentInternalNode = 0;
    std::vector<Node> mLeafNode;
    std::vector<Node> mInternalNode;
    Node* mRoot = nullptr;
};

