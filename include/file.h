#pragma once

#include <string>
#include <fstream>

class File
{
public:
    enum Mode
    {
        ReadOnlyText,
        ReadOnlyBinary,
        WriteOnlyText,
        WriteOnlyBinary
    };

public:
    explicit File(const char* aFileName)
        : mFileName(aFileName)
        , mSize(0)
    {
    }

    ~File()
    {
        Close();
    }

    bool Open(Mode aMode)
    {
        std::fstream::openmode mode;
        switch (aMode) {
        case ReadOnlyText:
            mode = std::fstream::in;
            break;
        case ReadOnlyBinary:
            mode = std::fstream::in | std::fstream::binary;
            break;
        case WriteOnlyText:
            mode = std::fstream::out;
            break;
        case WriteOnlyBinary:
            mode = std::fstream::out | std::fstream::binary;
            break;
        default:
            return false;
        }

        mFileStream.open(mFileName, mode);
        if (!IsOpen())
        {
            return false;
        }

        mFileStream.seekg(0, mFileStream.end);
        mSize = mFileStream.tellg();
        mFileStream.seekg(0, mFileStream.beg);

        return true;
    }

    int Read(char* aData, size_t aSize)
    {
        if (mFileStream.eof())
        {
            return 0;
        }

        mFileStream.read(aData, aSize);
        if (mFileStream.eof() && mFileStream.fail())
        {
            mFileStream.clear(mFileStream.eofbit);
        }

        return mFileStream.gcount();
    }

    size_t Write(const char* aData, size_t aSize)
    {
        mFileStream.write(aData, aSize);
        return mFileStream.good() ? aSize : 0;
    }

    size_t Size() const
    {
        return mSize;
    }

    bool Seek(size_t mPos)
    {
        mFileStream.seekg(mPos);
        return mFileStream.good();
    }

    void Close()
    {
        mFileStream.close();
    }

    bool IsOpen() const
    {
        return mFileStream.is_open();
    }

private:
    std::string mFileName;
    std::fstream mFileStream;
    size_t mSize;
};

