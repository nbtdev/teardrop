#############################################################################
#  Build static library
#############################################################################

CXXFLAGS = $(CFLAGS)

LIB = $(LIBDIR)/$(BUILD_TYPE)/lib$(LIBNAME).a
OBJDIR = $(OBJROOT)/$(BUILD_TYPE)/tech/$(LIBNAME)

INCLUDES += \
	$(ROOT)/tech
    
INCDIRS = $(addprefix -I, $(INCLUDES))
OBJS = $(addprefix $(OBJDIR)/, $(patsubst %.cpp, %.o, $(wildcard *.cpp)))


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
    
clean:
	@$(RM) $(RMFLAGS) $(LIB) $(OBJDIR)
