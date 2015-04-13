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

#ifndef lualib_h
#define lualib_h

#include "lua.h"


/* Key to file-handle type */
#define LUA_FILEHANDLE		"FILE*"


#define LUA_COLIBNAME	"coroutine"
LUALIB_API int (luaopen_base) (lua_State *L);

#define LUA_TABLIBNAME	"table"
LUALIB_API int (luaopen_table) (lua_State *L);

#define LUA_IOLIBNAME	"io"
LUALIB_API int (luaopen_io) (lua_State *L);

#define LUA_OSLIBNAME	"os"
LUALIB_API int (luaopen_os) (lua_State *L);

#define LUA_STRLIBNAME	"string"
LUALIB_API int (luaopen_string) (lua_State *L);

#define LUA_MATHLIBNAME	"math"
LUALIB_API int (luaopen_math) (lua_State *L);

#define LUA_DBLIBNAME	"debug"
LUALIB_API int (luaopen_debug) (lua_State *L);

#define LUA_LOADLIBNAME	"package"
LUALIB_API int (luaopen_package) (lua_State *L);


/* open all previous libraries */
LUALIB_API void (luaL_openlibs) (lua_State *L); 



#ifndef lua_assert
#define lua_assert(x)	((void)0)
#endif


#endif
