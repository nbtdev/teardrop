#############################################################################
#  Qt build flags and variables
#############################################################################

QT_SDK = /opt/qt/5.4.0/5.4/gcc_64
INCLUDES += $(QT_SDK)/include
LIBS += Qt5Core Qt5Gui Qt5Widgets
LDFLAGS += -L$(QT_SDK)/lib -Wl,-rpath,$(QT_SDK)/lib

MOC = $(QT_SDK)/bin/moc
UIC = $(QT_SDK)/bin/uic

MOC_FLAGS = 
UIC_FLAGS = 

QT_AUTOGEN = ./qt-autogen
MOCS = $(addprefix $(QT_AUTOGEN)/, $(MOCFILES:h=moc.cpp))
UICS = $(addprefix $(QT_AUTOGEN)/, $(UICFILES:ui=ui.h))
INCLUDES += $(QT_AUTOGEN)
EXTRA_CLEANS += $(QT_AUTOGEN)
SRCS += $(MOCS)

$(QT_AUTOGEN)/%.moc.cpp: %.h
	@mkdir -p $(dir $@)
	@echo "  [moc] $@ --> $<"
	@$(MOC) $(MOC_FLAGS) $< -o $@

$(QT_AUTOGEN)/%.ui.h: %.ui
	@mkdir -p $(dir $@)
	@echo "  [uic] $@ --> $<"
	@$(UIC) $(UIC_FLAGS) $< -o $@
