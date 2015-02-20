#############################################################################
#  Build static library
#############################################################################

CXXFLAGS = $(CFLAGS)

LIB = $(LIBDIR)/$(BUILD_TYPE)/lib$(LIBNAME).a
OBJDIR = $(OBJROOT)/$(BUILD_TYPE)/tech/$(LIBNAME)

INCLUDES += \
	$(ROOT)/tech
    
INCDIRS = $(addprefix -I, $(INCLUDES))

OBJCPP = $(addprefix $(OBJDIR)/, $(patsubst %.cpp, %.o, $(wildcard *.cpp)))
OBJC = $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(wildcard *.c)))
OBJCC = $(addprefix $(OBJDIR)/, $(patsubst %.cc, %.o, $(wildcard *.cc)))
OBJS = $(OBJCPP) $(OBJC) $(OBJCC)

all: $(OBJDIR) $(LIBDIR) $(LIB)

$(LIB): $(OBJS)
	@echo "  [lib] $@"
	@$(AR) $(ARFLAGS) -o $@ $^

$(LIBDIR):
	@mkdir -p $(LIBDIR)/$(BUILD_TYPE)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.cpp
	@echo "  [cxx] $< --> $@"
	@$(CXX) $(CXXFLAGS) $(INCDIRS) -c -o $@ $<

$(OBJDIR)/%.o: %.cc
	@echo "  [cxx] $< --> $@"
	@$(CXX) $(CXXFLAGS) $(INCDIRS) -c -o $@ $<

$(OBJDIR)/%.o: %.c
	@echo "  [cxx] $< --> $@"
	@$(CXX) $(CXXFLAGS) -fpermissive $(INCDIRS) -c -o $@ $<
    
clean:
	@$(RM) $(RMFLAGS) $(LIB) $(OBJDIR)
