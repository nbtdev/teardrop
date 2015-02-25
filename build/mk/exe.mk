#############################################################################
#  Build Linux executable
#  This file should be included last in the including Makefile
#
#  Define the following variables prior to including this file:
#    (required)
#    NAME - filename of the final executable
#    (optional)
#    LIBS - additional libraries to link with the executable
#############################################################################

CXXFLAGS = $(CFLAGS)

BINDIR = $(ROOT)/bin/$(BUILD_TYPE)
EXE = $(BINDIR)/$(NAME)

ADDLIBS = $(addprefix -l, $(LIBS))
LDFLAGS += -L$(ROOT)/lib/$(BUILD_TYPE) $(ADDLIBS)

opt-debug: $(OBJDIR) $(BINDIR) $(EXE)
debug: $(OBJDIR) $(BINDIR) $(EXE)
opt: $(OBJDIR) $(BINDIR) $(EXE)

$(EXE): $(OBJS)
	@echo "  [exe] $@"
	@$(CXX) -o $@ $^ $(LDFLAGS) 

$(BINDIR):
	@mkdir -p $(BINDIR)

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

clean-opt-debug:
	$(RM) $(RMFLAGS) $(LIB) $(OBJDIR) $(EXTRA_DEPS) $(EXTRA_CLEANS)

clean-debug:
	@$(RM) $(RMFLAGS) $(LIB) $(OBJDIR) $(EXTRA_DEPS) $(EXTRA_CLEANS)

clean-opt:
	@$(RM) $(RMFLAGS) $(LIB) $(OBJDIR) $(EXTRA_DEPS) $(EXTRA_CLEANS)
