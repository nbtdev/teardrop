NAME = OpenGL
NODENAME = integrations

INCLUDES += \
	. \
	$(ROOT)/$(NODENAME)/$(NAME)/extern/glew/include

SRCS += \
	extern/glew/src/glew.c \
	platform/GLX/RenderWindowOpenGL.cpp \
	platform/GLX/RendererOpenGL_createRenderWindow.cpp

include $(ROOT)/build/mk/common.vars.mk
include $(ROOT)/build/mk/lib.static.mk
