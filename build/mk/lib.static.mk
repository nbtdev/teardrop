#############################################################################
#  Build static library
#  This file should be included last in the including Makefile
#
#  Define the following variables prior to including this file:
#    (required)
#    NAME - filename of the static library
#############################################################################


LIBPATH = $(LIBDIR)/$(BUILD_TYPE)
LIB = $(LIBPATH)/lib$(NAME).a

opt-debug: $(OBJDIR) $(LIBPATH) $(LIB)
debug: $(OBJDIR) $(LIBPATH) $(LIB)
opt: $(OBJDIR) $(LIBPATH) $(LIB)

$(LIB): $(OBJS)
	@echo "  [lib] $@"
	@$(AR) $(ARFLAGS) -o $@ $^

$(LIBPATH):
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
    
clean-opt-debug:
	@$(RM) $(RMFLAGS) $(LIB) $(OBJDIR) $(EXTRA_DEPS) $(EXTRA_CLEANS)

clean-debug:
	@$(RM) $(RMFLAGS) $(LIB) $(OBJDIR) $(EXTRA_DEPS) $(EXTRA_CLEANS)

clean-opt:
	@$(RM) $(RMFLAGS) $(LIB) $(OBJDIR) $(EXTRA_DEPS) $(EXTRA_CLEANS)

