#############################################################################
#  FBX SDK build flags and variables
#############################################################################

ifndef FBX_SDK_DIR
    FBX_SDK_DIR = /opt/fbx/current
endif

INCLUDES += $(FBX_SDK_DIR)/include
FBX_SDK_LIBS = fbxsdk
LDFLAGS += -L$(FBX_SDK_DIR)/lib/gcc4/x64/release $(addprefix -l, $(FBX_SDK_LIBS))

