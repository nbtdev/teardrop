/******************************************************************************
Copyright (c) 2015 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

#include "Config.h"
#include "Stream/FileStream.h"
#include "Util/Logger.h"
#include <cstring>
#include <fstream>

using namespace Teardrop;

namespace {
    struct Internal
    {
        ~Internal() { delete mStrm; }
        std::fstream* mStrm = nullptr;
    };
}

//---------------------------------------------------------------------------
FileStream::FileStream()
{
    m_eof = false;
    m_handle = nullptr;
    m_mode = 0;
    m_length = 0;
    m_position = 0;
    m_asyncState = NOT_ASYNC;
    memset(&m_async, 0, sizeof(m_async));
}
//---------------------------------------------------------------------------
FileStream::~FileStream()
{
    delete (Internal*)m_handle;
}
//---------------------------------------------------------------------------
int FileStream::read(void* buffer, size_t size, bool /*async*/)
{
    if (((m_mode & READ) && m_handle) == 0)
        return 0;

    std::fstream* in = ((Internal*)m_handle)->mStrm;

    size_t bytesRead = 0;

    if (in && in->is_open() && in->good()) {
        std::streampos start = in->tellg();
        in->read((char*)buffer, size);
        bytesRead = in->tellg() - start;
    }

    m_eof = (in->eof() && bytesRead == 0);
    m_position += bytesRead;
    return bytesRead;
}
//---------------------------------------------------------------------------
int FileStream::write(const void* buffer, size_t size, bool async)
{
    if (((m_mode & (WRITE|APPEND)) && m_handle) == 0)
        return 0;

    std::fstream* out = ((Internal*)m_handle)->mStrm;

    size_t bytesWritten = 0;

    if (out && out->is_open() && out->good()) {
        std::streampos start = out->tellp();
        out->write((char*)buffer, size);
        bytesWritten = out->tellp() - start;
    }

    m_position += bytesWritten;
    return bytesWritten;
}
//---------------------------------------------------------------------------
bool FileStream::open(const char* filename, int mode, bool /*async*/)
{
    // close any open file first
    if (m_handle != 0)
        close();

    m_mode = mode;
    m_position = 0;

    std::ios_base::openmode iosMode = std::ios_base::binary;

    if (mode & APPEND) iosMode |= std::ios_base::app;
    if (mode & TRUNCATE) iosMode |= std::ios_base::trunc;

    if (mode & READ) {
        iosMode |= std::ios_base::in;
        Internal* i = new Internal;
        i->mStrm = new std::fstream(filename, iosMode);
        if (i->mStrm->is_open()) {
            m_handle = i;

            i->mStrm->seekg(0, std::ios_base::end);
            m_length = i->mStrm->tellg();
            i->mStrm->seekg(0, std::ios_base::beg);
        }
    }

    if (mode & WRITE) {
        iosMode |= std::ios_base::out;
        Internal* i = new Internal;
        i->mStrm = new std::fstream(filename, iosMode);
        if (i->mStrm->is_open()) {
            m_handle = i;
        }
    }

    if (m_handle == nullptr) {
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
void FileStream::close(bool /*async*/)
{
    if (m_handle) {
        Internal* i = (Internal*)m_handle;
        if (i->mStrm) {
            i->mStrm->close();
            delete i;
        }
    }

    m_handle = nullptr;
    m_position = 0;
}
//---------------------------------------------------------------------------
bool FileStream::isOpen() {
    return (m_handle && ((Internal*)m_handle)->mStrm && ((Internal*)m_handle)->mStrm->is_open());
}
//---------------------------------------------------------------------------
bool FileStream::seek(int offset, SeekType seekType, bool /*async*/)
{
    if (m_handle) {
        Internal* i = (Internal*)m_handle;

        if (i->mStrm && i->mStrm->is_open()) {
            std::ios_base::seekdir seekDir = std::ios_base::cur;
            if (seekType == BEGIN) seekDir = std::ios_base::beg;
            else if (seekType == END) seekDir = std::ios_base::end;

            if (m_mode & READ) i->mStrm->seekg(offset, seekDir);
            else if (m_mode & WRITE) i->mStrm->seekp(offset, seekDir);

            return i->mStrm->good();
        }
    }

    return false;
}
//---------------------------------------------------------------------------
void FileStream::update()
{
}
