# Teardrop SDK

## Building

### Windows

#### Prerequisites

* [Microsoft Visual Studio 2013](https://www.visualstudio.com/en-us/downloads/download-visual-studio-vs.aspx) or newer (Express editions may or may not work)
* [Microsoft Windows SDK For Windows 8.1](https://msdn.microsoft.com/en-us/windows/desktop/bg162891.aspx) or later (this is where Direct3D, DirectInput, OpenGL and other SDKs we need can be found, for Windows 7 and later)
* [Cygwin](https://cygwin.com/install.html) (POSIX environment for Windows), used for environment initialization/maintenance. You'll need to choose at least the following utilities/apps to install:
  * bash shell
  * git
  * wget
  * make
  * dos2unix

#### Step 1: Install Visual Studio
You will need to choose at least the C++ language support (currently only 32-bit tools are needed); no other languages or features are used

#### Step 2: Install Microsoft SDK
We want to install this after installing Visual Studio so that necessary paths are set up in the Visual Studio encironment

#### Step 3: Install Cygwin
It will make the following steps simpler if done in a Cygwin Bash shell. After installing the utilities listed above, open a Bash terminal and cd/mkdir the location you would like to use for development (I usually put my stuff in ~/projects, so using ~/projects -- or /home/&lt;username&gt;/projects -- I would "mkdir -p ~/projects" and then "cd ~/projects")

For example:

```
user@host ~
$ mkdir -p projects

user@host ~
$ cd projects

user@host ~/projects
$ 
```

#### Step 4: Clone this repo
If you have not already done so...

In your Bash terminal, do
```
user@host ~/projects
$ git clone git@github.com:nbtdev/teardrop.git
...
```

This will clone the Teardrop SDK source code into ~/projects/teardrop.

#### Step 5: Initialize the development environment

Change to the repo source directory and run "init.sh".
```
user@host ~/projects
$ cd teardrop

user@host ~/projects/teardrop
$ ./init.sh
...
```
Follow the prompts to install the remaining Teardrop SDK dependencies. These dependencies are:
* [Threading Building Blocks (TBB)](https://www.threadingbuildingblocks.org), used by both the SDK libaries and by the tools
* [Qt SDK](http://www.qt.io), used by the Editor
* [libsquish](https://code.google.com/p/libsquish), used by the tools for texture compression
* [FreeImage](http://freeimage.sourceforge.net), used by the tools for texture/image manipulation
TBB and Qt are downloaded in binary form from their respective sites; TBB is unpacked into the Teardrop SDK source tree and Qt is installed onto the local system via Qt's installer. libsquish and FreeImage are inserted into the tools source tree as git submodules and built with the previously installed Visual Studio, for binary compatibility reasons.

This script will persist a few environment variables that are used by the various Teardrop SDK Visual Studio solutions.

#### Step 6: Open and build the Teardrop SDK solution (found in the source root directory).

This will build all of the common libraries used by the tools and various integrations.

#### Step 7: Open and build the DirectX integration.

The DirectX integration and Visual Studio solution file is found in <teardrop>/integrations/DirectX. This integration provides a Direct3D9 renderer and DirectInput8 input handler.

#### Step 8 (optional): Open and build the OpenGL integration.

The OpenGL integration and Visual Studio solution file is found in <teardrop>/integrations/OpenGL. This integration provides an OpenGL4 renderer.

#### Step 9: Open and build the Tools.

The tools source and Visual Studio solution can be found in <teardrop>/tools, and provide the GUI-based asset/game editor application. 
