#pragma once

struct Node
{
    Node* Left = nullptr;
    Node* Right = nullptr;
    bool IsLeaf = true;

    unsigned char Symbol;
    unsigned int Frequence = 0;
    int Bits = 0;

    void Init(char aSymbol, unsigned int aFrequence)
    {
        Bits = 0;
        Symbol = aSymbol;
        Frequence = aFrequence;
    }

    void Init(Node* aLeft, Node* aRight)
    {
        Bits = 0;
        Frequence = aLeft->Frequence + aRight->Frequence;
        Left = aLeft;
        Right = aRight;
    }
};
