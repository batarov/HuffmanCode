#pragma once

class FileReader
{
    static const int FILE_BUFFER_SIZE = 4096;

public:
    FileReader(File& aFile)
        : mFile(aFile)
    {
    }

    bool ReadBit()
    {
        if (mCurrentBit == 0)
        {
            ReadByte();
            mCurrentBit = 1;
        }

        bool bit = mBuffer & mCurrentBit;
        mCurrentBit <<= 1;
        return bit;
    }

    void Reset()
    {
        mCurrentBit = 0;
        mBuffer = 0;
        mCurrentByte = FILE_BUFFER_SIZE;
    }

private:
    void ReadByte()
    {
        if (mCurrentByte == FILE_BUFFER_SIZE)
        {
            if (mFile.Read((char*)&mFileBuffer, FILE_BUFFER_SIZE) == 0)
            {
                throw std::runtime_error("Cound not read file");
            }

            mCurrentByte = 0;
        }

        mBuffer = mFileBuffer[mCurrentByte];
        ++mCurrentByte;
    }

private:
    File& mFile;
    unsigned char mCurrentBit = 0;
    unsigned char mBuffer = 0;
    unsigned char mFileBuffer[FILE_BUFFER_SIZE];
    int mCurrentByte = FILE_BUFFER_SIZE;
};
