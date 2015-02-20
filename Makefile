SUBDIRS = tech

DIR = `pwd`

ROOT := $(CURDIR)
export ROOT

export AR = ar
export ARFLAGS = rcs
export RM = rm
export RMFLAGS = -rf

export CC = gcc
export CXX = g++
export LIBDIR = $(ROOT)/lib
export OBJROOT = $(ROOT)/obj

COMMON_CFLAGS = -std=c++11 -fPIC
DEBUG_CFLAGS = $(COMMON_CFLAGS) -g -O0 -DDEBUG -D_DEBUG
OPTDEBUG_CFLAGS = $(COMMON_CFLAGS) -g -O2 -DNDEBUG
OPT_CFLAGS = $(COMMON_CFLAGS) -O3 -DNDEBUG

COMMON_LDFLAGS = 
DEBUG_LDFLAGS = $(COMMON_LDFLAGS)
OPTDEBUG_LDFLAGS = $(COMMON_LDFLAGS)
OPT_LDFLAGS = $(COMMON_LDFLAGS)

# default to opt-debug builds
all: opt-debug

opt-debug:
	@export CFLAGS="$(OPTDEBUG_CFLAGS)"; \
	export LDFLAGS="$(OPTDEBUG_LDFLAGS)"; \
	export BUILD_TYPE="opt-debug"; \
	for s in $(SUBDIRS); do \
		$(MAKE) -C $$s; \
	done

debug:
	@export CFLAGS=$(DEBUG_CFLAGS); \
	export LDFLAGS=$(DEBUG_LDFLAGS); \
	export BUILD_TYPE="debug"; \
	for s in $(SUBDIRS); do \
		$(MAKE) -C $$s; \
	done

opt:
	@export CFLAGS=$(OPT_CFLAGS); \
	export LDFLAGS=$(OPT_LDFLAGS); \
	export BUILD_TYPE="opt"; \
	for s in $(SUBDIRS); do \
		$(MAKE) -C $$s; \
	done

clean: clean-opt-debug

clean-opt-debug:
	@export BUILD_TYPE="opt-debug"; \
	for s in $(SUBDIRS); do \
		echo Cleaning in $$s; \
		$(MAKE) -C $$s clean; \
	done

clean-debug:
	@export BUILD_TYPE="debug"; \
	for s in $(SUBDIRS); do \
		echo Cleaning in $$s; \
		$(MAKE) -C $$s clean; \
	done

clean-opt:
	@export BUILD_TYPE="opt"; \
	for s in $(SUBDIRS); do \
		echo Cleaning in $$s; \
		$(MAKE) -C $$s clean; \
	done
