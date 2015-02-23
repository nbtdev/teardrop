NAME = Editor

MOCFILES = \
	Editor.h \
	Preferences.h \
	RenderWindow.h \
	MaterialEditor/MaterialEditor.h \
	ObjectBrowser/ObjectBrowser.h \
	Viewers/ObjectViewer3D.h \
	ProjectExplorer/ProjectExplorer.h \
	ProjectExplorer/ProjectModel.h \
	PropertyGrid/PropertyGrid.h \
	PropertyGrid/PropertyGridDelegate.h \
	PropertyGrid/PropertyGridModel.h

UICFILES = Editor.ui Preferences.ui

NODENAME = tools
INCLUDES = $(ROOT)/$(NODENAME) $(ROOT)/tech/ThirdParty .

SRCS = $(wildcard MaterialEditor/*.cpp)
SRCS += $(wildcard ObjectBrowser/*.cpp)
SRCS += $(wildcard ProjectExplorer/*.cpp)
SRCS += $(wildcard PropertyGrid/*.cpp)
SRCS += $(wildcard ProjectExplorer/*.cpp)
SRCS += $(wildcard Viewers/*.cpp)

include $(ROOT)/build/mk/common.vars.mk
include $(ROOT)/build/mk/qt.mk
include $(ROOT)/build/mk/tbb.mk
include $(ROOT)/build/mk/fbx.mk
include $(ROOT)/build/mk/squish.mk

LIBS += \
	PackageManager \
	squish \
	HFZ \
	Asset \
	Core \
	Gfx \
	Math \
	Memory \
	Package \
	Reflection \
	Stream \
	tinyxml \
	Util \
	VFX \
	uuid \
	freeimage \
	z

include $(ROOT)/build/mk/exe.mk

$(OBJDIR)/Editor.o: $(QT_AUTOGEN)/Editor.ui.h $(QT_AUTOGEN)/Preferences.ui.h $(QT_AUTOGEN)/Editor.moc.cpp $(QT_AUTOGEN)/Preferences.moc.cpp

