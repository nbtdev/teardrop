#!/bin/bash

# Prereq:
#    Cygwin/*nix: wget, dos2unix

DIR=$(cd `dirname -- $0` && pwd)
WINDIR=$(cd `dirname -- $0` && cmd /C cd | dos2unix)

# explain to the user what we will be doing
echo "We need to grab some dependencies from the interwebs. These include:"
echo "    1) TBB (current 4.x OSS version, used by engine and tools) [download, unpack]"
echo "    2) FreeImage (image manipulation, used by the editor/tools) [submodule]"
echo "    3) libsquish (texture compression, used by the editor/tools) [submodule]"
echo "    4) Qt 5.x (UI framework, used by the editor) [download, install]"
echo " "

read -p "Press Enter to continue..."
echo " "


# grab TBB into tech/ThirdParty

TBB_NAME=tbb43_20150316oss
TBB_ZIPNAME=$TBB_NAME
TBB_ZIPNAME+="_win.zip"
TBB_ZIP_SRC="https://www.threadingbuildingblocks.org/sites/default/files/software_releases/windows/$TBB_ZIPNAME"
TBB_ZIP_DEST=/tmp/tbb.zip
TBB_THIRD_PARTY=tech/ThirdParty/TBB
TBB_THIRD_PARTY_WIN=tech\\ThirdParty\\TBB

echo -n "1) Checking for installed TBB..."
if [ "x$TBB_INSTALL_DIR" == "x" ]
then
	if [ ! -e $TBB_ZIP_DEST ]
	then
		echo "no, fetching"
		wget --show-progress -O $TBB_ZIP_DEST $TBB_ZIP_SRC
	else
		echo "found $TBB_ZIP_DEST"
	fi

	if [ -e $TBB_ZIP_DEST ]
	then
		echo -n "   Unpacking TBB zipfile into tech/ThirdParty/TBB..."
		rm -rf $TBB_THIRD_PARTY
		unzip -q -o -d tech/ThirdParty $TBB_ZIP_DEST
		mv tech/ThirdParty/$TBB_NAME $TBB_THIRD_PARTY
		echo "done"

		# TODO: do we really want to delete these huge downloads?
		#rm -f $TBB_ZIP_DEST
		
		echo $WINDIR
		TBB_INSTALL_DIR_VAL="$WINDIR\\$TBB_THIRD_PARTY_WIN"
		setx TBB_INSTALL_DIR "$TBB_INSTALL_DIR_VAL"
		TBB_INSTALL_DIR="$DIR/$TBB_THIRD_PARTY"
		#export TBB_INSTALL_DIR
	else
		echo "Encountered error while trying to download and/or unpack $TBB_ZIP_DEST, exiting"
		exit
	fi
else
	if [ -e TBB_INSTALL_DIR ]
	then
		echo "exists in '$TBB_INSTALL_DIR'"
	else
		echo "TBB_INSTALL_DIR variable is set, but '$TBB_INSTALL_DIR' does not exist! Corrupt installation?"
		exit
	fi
fi

# explain that we will defer update/sync/build of FreeImage and squish for later
echo "2) FreeImage is referenced as a submodule to this repo, and will be sync'ed/built shortly"
echo "3) libsquish is referenced as a submodule to this repo, and will be sync'ed/built shortly"

# grab Qt 5.4.x (Community) and install it

QTINSTALLER=qt-opensource-windows-x86-msvc2013_opengl-5.4.1.exe
QTURL=http://download.qt.io/official_releases/qt/5.4/5.4.1/qt-opensource-windows-x86-msvc2013_opengl-5.4.1.exe

echo -n "4) Checking for installed Qt..."
if [ "x$QTDIR" == "x" ]
then
	echo -n "no, checking for installer..."
	if [ ! -e /tmp/qt.exe ]
	then
		echo "no, downloading installer"
		wget --show-progress -O /tmp/qt.exe $QTURL
	
	else
		echo "found"
	fi
	
	# quiet install
	echo "We will now launch the Qt GUI installer. Please make a note of the root Qt install location, you will need to supply that information when the install is completed."
	echo 
	read -p "Press Enter when ready to launch Qt installer..."
	chmod +x /tmp/qt.exe
	/tmp/qt.exe
	
	echo "In the following prompt, if using Windows-style path separators, please "
	echo "escape them properly (i.e. C:\\\\MyDir and not C:\\MyDir)"
	echo
	
	read -p "Root Qt install dir [C:\\Qt\\Qt5.4.1]: " QT_DIR
	
	if [ "x$QT_DIR" == "x" ]
	then
		QT_DIR="C:\\Qt\\Qt5.4.1"
	fi
	
	echo "Using '$QT_DIR'; if this is not correct, you will need to correct manually with 'setx' command"
	setx QTDIR "$QT_DIR\\5.4\\msvc2013_opengl"
	
	# since we are running in Bash, and possibly on Cygwin, export this as well to the current terminal
	QTDIR="$QT_DIR\\5.4\\msvc2013_opengl"
	export QTDIR
else
	echo "found in '$QTDIR'"
fi

echo 
echo "************************************************************************"
echo "Updating submodules, if necessary"
echo "************************************************************************"
echo 

git submodule update --remote --recursive

echo 
echo "************************************************************************"
echo Building libsquish, if necessary
echo "************************************************************************"
echo 

if [ "x$SQUISH_DIR" == "x" ]
then
	#cmd /C tools\\ThirdParty\\squish\\vs2013\\build.bat

	SQUISH_DIR_VAL="$WINDIR\\tools\\ThirdParty\\squish"
	setx SQUISH_DIR "$SQUISH_DIR_VAL"
	SQUISH_DIR="$DIR/tools/ThirdParty/squish"
	#export SQUISH_DIR
fi


echo 
echo "************************************************************************"
echo Building FreeImage, if necessary
echo "************************************************************************"
echo 

if [ "x$FREEIMAGE_DIST" == "x" ]
then
	#cmd /C tools\\ThirdParty\\FreeImage\\build.bat

	FREEIMAGE_DIST_VAL="$WINDIR\\tools\\ThirdParty\\FreeImage"
	setx FREEIMAGE_DIST "$FREEIMAGE_DIST_VAL"
	FREEIMAGE_DIST="$DIR/tools/ThirdParty/FreeImage"
	#export FREEIMAGE_DIST
fi

echo 
echo "************************************************************************"
echo Copy DLLs to sdk/bin and tools/bin...
echo "************************************************************************"
echo 

rm -f sdk/bin/Release/tbb.dll
rm -f sdk/bin/Debug/tbb_debug.dll
rm -f sdk/bin/Release/tbbmalloc.dll
rm -f sdk/bin/Debug/tbbmalloc_debug.dll
mkdir -p sdk/bin/Release
mkdir -p sdk/bin/Debug
cp $TBB_INSTALL_DIR/bin/ia32/vc12/tbb.dll sdk/bin/Release/tbb.dll
cp $TBB_INSTALL_DIR/bin/ia32/vc12/tbb_debug.dll sdk/bin/Debug/tbb_debug.dll
cp $TBB_INSTALL_DIR/bin/ia32/vc12/tbbmalloc.dll sdk/bin/Release/tbbmalloc.dll
cp $TBB_INSTALL_DIR/bin/ia32/vc12/tbbmalloc_debug.dll sdk/bin/Debug/tbbmalloc_debug.dll

rm -f tools/bin/Release/tbb.dll
rm -f tools/bin/Debug/tbb_debug.dll
rm -f tools/bin/Release/tbbmalloc.dll
rm -f tools/bin/Debug/tbbmalloc_debug.dll
mkdir -p tools/bin/Release
mkdir -p tools/bin/Debug
cp $TBB_INSTALL_DIR/bin/ia32/vc12/tbb.dll tools/bin/Release/tbb.dll
cp $TBB_INSTALL_DIR/bin/ia32/vc12/tbb_debug.dll tools/bin/Debug/tbb_debug.dll
cp $TBB_INSTALL_DIR/bin/ia32/vc12/tbbmalloc.dll tools/bin/Release/tbbmalloc.dll
cp $TBB_INSTALL_DIR/bin/ia32/vc12/tbbmalloc_debug.dll tools/bin/Debug/tbbmalloc_debug.dll

rm -f tools/bin/Release/FreeImage.dll
rm -f tools/bin/Debug/FreeImage.dll
cp $FREEIMAGE_DIST/bin/Win32/Release/FreeImage.dll tools/bin/Release/FreeImage.dll
cp $FREEIMAGE_DIST/bin/Win32/Debug/FreeImage.dll tools/bin/Debug/FreeImage.dll

# set the base SDK dir variable
TEARDROP_SDK_VAL=$WINDIR\\sdk
setx TEARDROP_SDK "$TEARDROP_SDK_VAL"

echo
echo "******** Environment Variables ********"
echo "TBB_INSTALL_DIR: $TBB_INSTALL_DIR_VAL"
echo "SQUISH_DIR: $SQUISH_DIR_VAL"
echo "FREEIMAGE_DIST: $FREEIMAGE_DIST_VAL"
echo "QTDIR: $QT_DIR"
echo "TEARDROP_SDK: $TEARDROP_SDK_VAL"
echo
echo "Done!"
