# comp & comp flags
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wno-stringop-overread

LDFLAGS := -lcheck -fprofile-arcs --coverage
ifeq ($(shell uname), Linux)
    LDFLAGS += -pthread -lcheck_pic -lrt -lm -lsubunit
endif

# source files/dirs
SRCDIR = .
SRC = $(filter-out $(SRCDIR)/s21_sprintf.c, $(wildcard $(SRCDIR)/s21_*.c))  # REMOVE FILTER-OUT FOR S21_SSPTINF LATER
OBJ = $(SRC:.c=.o)
LIB = s21_string.a


# for testing & gcov report
TEST_DIR = tests
TEST_SRC = $(wildcard $(TEST_DIR)/test_s21_*.c)  # Find all test_s21_*.c files in the tests directory
TEST_OBJ = $(TEST_SRC:.c=.o)  # Corresponding .o files for the tests
TEST_EXEC = s21_string_test

# default target
all: clean $(LIB)
	@rm -f $(OBJ) *.gcda *.gcno

# building static lib from obj files
$(LIB): $(OBJ)
	ar rcs $@ $^

# obj
%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# build and run tests
test: $(LIB) $(TEST_OBJ)
	$(CC) $(CFLAGS) $(TEST_OBJ) -L. -l:s21_string.a -o $(TEST_EXEC) $(LDFLAGS) 
	@rm -f $(OBJ) *.gcda *.gcno

# general gcov report in HTML format
gcov_report: 
	@echo "WIP"

# clean target
clean:
	rm -f $(OBJ) $(LIB) $(TEST_EXEC) $(TEST_OBJ)
	rm -f *.gcda *.gcno

.PHONY: all clean test gcov_report
