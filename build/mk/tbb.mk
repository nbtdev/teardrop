#############################################################################
#  TBB build flags and variables
#############################################################################

ifndef TBB_DIR
    TBB_DIR = /opt/tbb/current
endif 

INCLUDES += $(TBB_DIR)/include
TBBLIBS = tbb
LDFLAGS += -L$(TBB_DIR)/lib/intel64/gcc4.4 $(addprefix -l, $(TBBLIBS))

