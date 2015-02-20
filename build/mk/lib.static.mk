#############################################################################
#  Build static library
#############################################################################

CXXFLAGS = $(CFLAGS)

LIB = $(LIBDIR)/$(BUILD_TYPE)/lib$(LIBNAME).a
OBJDIR = $(OBJROOT)/$(BUILD_TYPE)/tech/$(LIBNAME)

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

all: $(OBJDIR) $(LIBDIR) $(LIB)

$(LIB): $(OBJS)
	@echo "  [lib] $@"
	@$(AR) $(ARFLAGS) -o $@ $^

$(LIBDIR):
	@echo $(OBJS)
	@mkdir -p $(LIBDIR)/$(BUILD_TYPE)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.cpp
	@echo "  [cxx] $< --> $@"
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(INCDIRS) -c -o $@ $<

$(OBJDIR)/%.o: %.cc
	@echo "  [cxx] $< --> $@"
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(INCDIRS) -c -o $@ $<

$(OBJDIR)/%.o: %.c
	@echo "  [cxx] $< --> $@"
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -fpermissive $(INCDIRS) -c -o $@ $<
    
clean:
	@$(RM) $(RMFLAGS) $(LIB) $(OBJDIR)
