#############################################################################
#  Common make variables
#############################################################################

CXXFLAGS = $(CFLAGS) -std=c++11
OBJDIR = $(OBJROOT)/$(BUILD_TYPE)/$(NODENAME)/$(NAME)

INCLUDES += \
    $(ROOT)/tech
    
INCDIRS = $(addprefix -I, $(INCLUDES))

SRCS += $(wildcard *.cpp)
SRCS += $(wildcard *.c)
SRCS += $(wildcard *.cc)

TMPOBJ1 = $(filter %.o, $(SRCS:cpp=o))
TMPOBJ2 = $(filter %.o, $(SRCS:c=o))
TMPOBJ3 = $(filter %.o, $(SRCS:cc=o))

OBJS = $(addprefix $(OBJDIR)/, $(TMPOBJ1) $(TMPOBJ2) $(TMPOBJ3))

LIBPATH = $(LIBDIR)/$(BUILD_TYPE)
