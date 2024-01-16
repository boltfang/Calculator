CPU := $(shell uname -p)
ifeq ($(CPU),arm)
export LD_LIBRARY_PATH=/opt/homebrew/lib/:$$LD_LIBRARY_PATH
INCD = -I /opt/homebrew/include/
LIBD = -L /opt/homebrew/lib/
endif

CC := gcc
SRCD := polynomial
TSTD := polynomial
BLDD := build
BIND := bin
INCD += -I include

ALL_SRCF := $(shell find $(SRCD) -type f -name '*.c')
ALL_OBJF := $(patsubst $(SRCD)/%,$(BLDD)/%,$(ALL_SRCF:.c=.o))
FUNC_FILES := $(filter-out $(BLDD)/test.o, $(ALL_OBJF))
TEST_SRC := $(shell find $(TSTD) -type f -name '*.c')
TEST_OBJ := $(patsubst $(TSTD)/%,$(BLDD)/%,$(TEST_SRC:.c=.o))

MAIN := $(BIND)/polynomial
TEST := $(BIND)/test_polynomial

CFLAGS := -Wall -Wextra -Wshadow -Wdouble-promotion -Wformat=2 -Wundef -pedantic
GCOV := -fprofile-arcs -ftest-coverage
DFLAGS := -g -DDEBUG
PRINT_STATEMENTS := -DERROR -DSUCCESS -DWARN -DINFO

STD := -std=gnu11
TEST_LIB := -lcriterion
LIBS := -lm

CFLAGS += $(STD)
CFLAGS += $(GCOV)
CFLAGS += $(DFLAGS)

TEST_RESULTS := "test_results.json"
OCLINT_REPORT := "oclint_report.html"
GCOV_REPORT := "matrix.c.gcov"

# Default target to build main program and tests
all: $(MAIN) $(TEST)

# Main program target
$(MAIN): $(FUNC_FILES) $(BLDD)/main.o
	$(CC) $(CFLAGS) $(INCD) $^ -o $@ $(LIBS)

# Test target
$(TEST): $(FUNC_FILES) $(TEST_OBJ)
	$(CC) $(CFLAGS) $(INCD) $^ -o $@ $(TEST_LIB) $(LIBS)

# Add your existing targets (check, gcov, clean) here...

.PHONY: all clean debug setup test
