#############################################################################
#  Squish library build flags and variables
#############################################################################

SQUISH_DIR ?= /opt/squish

INCLUDES += $(SQUISH_DIR)/include
SQUISH_LIBS = squish
LDFLAGS += -L$(SQUISH_DIR)/lib $(addprefix -l, $(SQUISH_LIBS))

