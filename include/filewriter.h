#pragma once

class FileWriter
{
    static const int FILE_BUFFER_SIZE = 4096;

public:
    FileWriter(File& aFile)
        : mFile(aFile)
    {
    }

    void Write(const CodeWord* aCode)
    {
        if (aCode->Bits == 0)
        {
            return;
        }

        unsigned int currentBit = (1 << (aCode->Bits - 1));
        while (currentBit)
        {
            WriteBit(aCode->Code & currentBit);
            currentBit >>= 1;
        }
    }

    void WriteByte(unsigned char aData, bool aFlush = false)
    {
        mFileBuffer[mCurrentByte] = aData;
        ++mCurrentByte;
        if (mCurrentByte == FILE_BUFFER_SIZE || aFlush)
        {
            mFile.Write((char*)mFileBuffer, aFlush ? mCurrentByte : FILE_BUFFER_SIZE);
            mCurrentByte = 0;
        }
    }

    void Flush()
    {
        WriteByte(mBuffer, true);
        mCurrentBit = 1;
        mBuffer = 0;
    }

private:
    void WriteBit(bool aBit)
    {
        if (aBit)
        {
            mBuffer |= mCurrentBit;
        }

        mCurrentBit <<= 1;
        if (mCurrentBit == 0)
        {
            WriteByte(mBuffer);
            mCurrentBit = 1;
            mBuffer = 0;
        }
    }

private:
    File& mFile;
    unsigned char mCurrentBit = 1;
    unsigned char mBuffer = 0;
    unsigned char mFileBuffer[FILE_BUFFER_SIZE];
    int mCurrentByte = 0;
};
