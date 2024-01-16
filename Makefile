# Makefile

# Compiler
CC := gcc

# Compilation flags
CFLAGS := -Wall -Wextra -Wshadow -Wdouble-promotion -Wformat=2 -Wundef -pedantic
GCOV := -fprofile-arcs -ftest-coverage
DFLAGS := -g -DDEBUG
PRINT_STATEMENTS := -DERROR -DSUCCESS -DWARN -DINFO

# Standard
STD := -std=gnu11
LIBS := -lm

# Additional paths
INCD := -I include
LIBD := -L /path/to/criterion/lib   # Replace with the actual path to your Criterion library

# Additional flags
CFLAGS += $(STD)
CFLAGS += $(GCOV)
CFLAGS += $(DFLAGS)

# Directories
SRCD := src
TSTD := tests
AUXD := tests_aux
BLDD := build
BIND := bin

# Main program
MAIN := your_main_program  # Replace with the actual name of your main program

# Test program
TEST := unit_tests

# Source files
ALL_SRCF := $(shell find $(SRCD) -type f -name '*.c')
ALL_OBJF := $(patsubst $(SRCD)/%,$(BLDD)/%,$(ALL_SRCF:.c=.o))
FUNC_FILES := $(filter-out build/main.o, $(ALL_OBJF))
TEST_SRC := $(shell find $(TSTD) -type f -name '*.c')
TEST_OBJ := $(patsubst $(TSTD)/%,$(BLDD)/%,$(TEST_SRC:.c=.o))
AUX_SRC := $(shell find $(AUXD) -type f -name '*.c')

AUX_OBJS := $(patsubst $(AUXD)/%,$(BLDD)/%,$(AUX_SRC:.c=.o))
AUX_PGMS := $(patsubst $(AUXD)/%,$(BIND)/%,$(AUX_SRC:.c=))

# Targets
all: setup $(BIND)/$(MAIN)

test: setup $(BIND)/$(TEST)
	@rm -fr $(TSTD).out
	@mkdir -p $(TSTD).out
	@$(BIND)/$(TEST) -j1 --full-stats --verbose --json=test_results.json

$(BIND)/$(MAIN): $(FUNC_FILES)
	$(CC) $(CFLAGS) $(INCD) $(FUNC_FILES) -o $@ $(LIBS)

$(BIND)/$(TEST): $(FUNC_FILES) $(TEST_OBJ)
	$(CC) $(CFLAGS) $(INCD) $(FUNC_FILES) $(TEST_OBJ) -o $@ $(LIBS)

$(BLDD)/%.o: $(TSTD)/%.c
	$(CC) $(CFLAGS) $(INCD) -c -o $@ $<

$(BLDD)/%.o: $(SRCD)/%.c 
	$(CC) $(CFLAGS) $(INCD) -c -o $@ $<

setup: $(BIND) $(BLDD)
$(BIND):
	mkdir -p $(BIND)
$(BLDD):
	mkdir -p $(BLDD)

clean:
	rm -fr $(BLDD) $(BIND) $(TSTD).out test_results.json

.PHONY: all test clean setup
