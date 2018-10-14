# Teardrop SDK

## What Is It?

Without getting into the deep sordid historical details, this is the fifth (!) iteration on a working game engine and toolset, originally intended to support the development of a mecha-style combat game. Why five? Well, it took that many tries to get to a tool(set) and architecture that I liked.

You may notice that some things in the SDK remind you of other engines/SDKs you may have worked with. The editor is heavily influenced by UnrealEd (as any useful game editor should be) with a touch of Unity3D. The architecture (especially the Reflection system) is primarily influenced by an internally-developed game toolkit we had at one of my previous studios, with various flavors of GameBryo thrown in. 

The code style is a mishmash; as it's been over a decade since I've started this, I've had to deal with various different coding standards at various studios/employers, and I tended to adopt the style and standards used by wherever I was working at the time (it helps me retain sanity). 

At first the SDK was implemented wholly on Windows (the reason was that when we implemented physics/collision and animation, we used Havok, which is only on Windows if you want the free version). I've since decided that I want to provide a 100% open-source game development solution, which is why there is a Linux build, and the various implementations were moved out of the tech tree and into their own "integrations" trees. This allows you, for instance, if you like, to provide implementations (wrappers, if you prefer that term) for whatever subsystem you care to use. Out of the box, however, my intent is to provide only open-source implementations...within reason, of course. For example, I will continue to provide FBX import and Direct3D renderer implementations, but if someone wants to build their game on Linux using only FOSS SDKs and tools, they can (for example) use the OpenGL renderer, Bullet physics integration and the (forthcoming) .blend import and never have to touch a closed-source, proprietary piece of software if they prefer. 

The codebase evolves according to the YAGNI principle of continuous refactoring. In other words, you will find plenty of places in the code where something you might expect to exist, simply does not, because it's not been needed yet. Feel free to implement it if you like, and even better, offer a pull request if you do. My goal was to achieve MVP (minimum viable product) first, and flesh out the missing bits as the SDK grows. This growth, assuming I am the only one to do anything with it, will happen according to the needs of the aformentioned mecha game, which itself (unfortunately, in case you were wondering), will remain closed source (at least for current-gen releases; I plan to open the source to older, obsolete versions as releases occur, for the sake of having a sample codebase for the SDK if nothing else). 

## License
The license (not as of this writing, if you look at the file headers) is MIT/BSD -- liberal, in other words. Feel free to use it however you want (or don't), and don't feel guilty if you just use it without "giving back". I made the SDK open source for the (unlikely) occasion that anyone else might find it useful, not because I am on some Stallman-esque mission to force everyone into my world-view(-projection...sorry, that just came out on its own...and if you don't get it, this might not be the SDK for you... ;) ). 

So MIT license it is. If you find it useful (and only marginally buggy), then awesome...and if not, then I promise you your money back.

## Building

The goal is eventually to provide handheld targets for Android and iOS (and Windows Phone if anyone is still using/developing-for that by the time I get there), but up front, it's PC desktop targets on Windows and Linux (although I'll bet someone would be able to create an OSX XCode build at some point...and that someone would probably be me if I intend to do iOS...).

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

### Linux

#### Prerequisites

* TBB
* FBX SDK
* QT5
* Ubuntu: uuid-dev zlib1g-dev libfreeimage-dev libglew-dev
