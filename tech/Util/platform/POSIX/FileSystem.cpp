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

#include "Util/FileSystem.h"
#include <fcntl.h>
#include <glob.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>

using namespace Teardrop;

const char FileSystem::PATHSEP = '/';

void FileSystem::glob(
    const String& dirName,
    const String& searchPattern,
    FileList& files,
    bool recursive/* =false */)
{
    String findSpec(dirName);
    findSpec.append(PATHSEP).append(searchPattern);

    // first glob all matches in dirName
    glob_t g;
    ::glob(searchPattern, 0, nullptr, &g);
    for (int i=0; i<g.gl_pathc; ++i) {
        files.push_back(g.gl_pathv[i]);
    }

    // if recursive, dive into subdirectories too
    if (recursive) {
        for (int i=0; i<g.gl_pathc; ++i) {
            String pathName(dirName);
            pathName.append(PATHSEP);
            pathName += g.gl_pathv[i];

            struct stat st;
            if (0 == stat(pathName, &st)) {
                // if this is a directory, and isn't . or .., and we are recursing, then
                // recurse into this directory
                if (S_ISDIR(st.st_mode)) {
                    if (String(".") != g.gl_pathv[i] &&
                        String("..") != g.gl_pathv[i]) {
                            glob(pathName, searchPattern, files, recursive);
                    }
                }
            }
        }
    }

    globfree(&g);
}
//---------------------------------------------------------------------------
void FileSystem::findAll(
    const String& dirName,
    const String& fileName,
    FileList& files,
    bool recursive/* =false */)
{
}
//---------------------------------------------------------------------------
void FileSystem::makeRelativePath(
    String& /*result*/, const String& /*from*/, const String& /*to*/)
{
}
//---------------------------------------------------------------------------
void FileSystem::getAppDataPath(String& path)
{
    path.clear();

    // get home directory
    const char* home = getenv("HOME");

    if (home) {
        path = home;
        path.append(PATHSEP).append("Teardrop");
    }
}

bool FileSystem::createDirectory(const String& dirPath)
{
    int rtn = mkdir(dirPath, 0755);
    if (rtn) {
        return false;
    }

    return true;
}

// path names in Windows are not case-sensitive (i.e. F:\TMP is the same
// directory as f:\tmp or F:\tmp), so compare everything as if it were upper-case
bool FileSystem::isSamePath(const String& lhs, const String& rhs)
{
    return lhs == rhs;
}

// copy file from 'from' to 'to'; returns false if copy failed for any reason
bool FileSystem::copyFile(const String& from, const String& to, bool overwrite)
{
    int fromFd, toFd;

    fromFd = open(from, O_RDONLY);
    if (fromFd == -1) return false;

    toFd = open(to, O_WRONLY);
    if (toFd == -1) {
        close(fromFd);
        return false;
    }

    struct stat st = {0};
    fstat(fromFd, &st);

    off_t offset = 0;
    int bytes = sendfile(toFd, fromFd, &offset, st.st_size);

    close(fromFd);
    close(toFd);

    return bytes == st.st_size;
}
