CPU := $(shell uname -p)
ifeq ($(CPU),arm)
export LD_LIBRARY_PATH=/opt/homebrew/lib/:$$LD_LIBRARY_PATH
INCD = -I /opt/homebrew/include/
LIBD = -L /opt/homebrew/lib/
endif

CC := gcc
SRCD := polynomial
TSTD := $(SRCD)/test
BLDD := build
BIND := bin
INCD += -I include

ALL_SRCF := $(shell find $(SRCD) -type f -name '*.c')
ALL_OBJF := $(patsubst $(SRCD)/%,$(BLDD)/%,$(ALL_SRCF:.c=.o))
FUNC_FILES := $(filter-out $(BLDD)/test_polynomial.o, $(ALL_OBJF))
TEST_SRC := $(shell find $(TSTD) -type f -name '*.c')
TEST_OBJ := $(patsubst $(SRCD)/%,$(BLDD)/%,$(TEST_SRC:.c=.o))

MAIN := $(BIND)/polynomial
TEST := $(BIND)/test_polynomial

CRITERION_DIR := criterion-2.4.1
CRITERION_LIB := $(CRITERION_DIR)/lib/libcriterion.so
CRITERION_INC := $(CRITERION_DIR)/include

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

# Build rule for Criterion library
$(CRITERION_LIB):
	curl -o criterion.tar.xz -L https://github.com/Snaipe/Criterion/releases/download/v2.4.1/criterion-2.4.1-linux-x86_64.tar.xz
	tar xf criterion.tar.xz
	mv criterion-2.4.1 $(CRITERION_DIR)

# Add your existing targets (check, gcov, clean) here...

.PHONY: all clean debug setup test

# Include Criterion library in the build process
all: $(MAIN) $(TEST) $(CRITERION_LIB)

# Add the following lines at the end of the Makefile
$(BLDD)/%.o: $(SRCD)/%.c
	$(CC) $(CFLAGS) $(INCD) -c -o $@ $<

setup: $(BIND) $(BLDD)

$(BIND):
	mkdir -p $(BIND)

$(BLDD):
	mkdir -p $(BLDD)

clean:
	rm -fr $(BLDD) $(BIND) $(TSTD).out *.out $(OCLINT_REPORT) $(GCOV_REPORT) $(TEST_RESULTS)

debug: CFLAGS += $(DFLAGS) $(PRINT_STATEMENTS)
debug: all

# Include Criterion library in the clean target
clean: clean_criterion

clean_criterion:
	rm -rf $(CRITERION_DIR) criterion.tar.xz
