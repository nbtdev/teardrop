#############################################################################
#  Build static library
#############################################################################

AR = ar
ARFLAGS = rcs
RM = rm
RMFLAGS = -rf

CC = gcc
CXX = g++
CFLAGS = -O2 -g -fPIC
LDFLAGS = -O2 -g
CXXFLAGS = $(CFLAGS) -std=c++11

INCLUDES = \
	$(ROOT)/tech
    
INCDIRS = $(addprefix -I, $(INCLUDES))

OBJS = $(addprefix $(OBJDIR)/, $(patsubst %.cpp, %.o, $(wildcard *.cpp)))

all: $(OBJDIR) $(LIBDIR) $(LIB)

$(LIB): $(OBJS)
	@echo "  [lib] $@"
	@$(AR) $(ARFLAGS) -o $@ $^

$(LIBDIR):
	@mkdir -p $(LIBDIR)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.cpp
	@echo "  [cxx] $< --> $@"
	@$(CXX) $(CXXFLAGS) $(INCDIRS) -c -o $@ $<
    
clean:
	$(RM) $(RMFLAGS) $(LIB) $(OBJDIR)
