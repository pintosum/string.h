# comp & comp flags. Add -Wall -Wextra -Werror on mac later
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
LDFLAGS := -lcheck -fprofile-arcs --coverage
ifeq ($(shell uname), Linux)
    LDFLAGS += -pthread -lcheck_pic -lrt -lm -lsubunit
endif

# source files/dirs
SRCDIR = .
SRC = $(wildcard $(SRCDIR)/s21_*.c)
OBJ = $(SRC:.c=.o)
LIB = s21_string.a

# for testing & gcov report
TEST_DIR = tests
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
test: $(LIB)
	$(CC) $(CFLAGS) -Wno-stringop-overread $(TEST_DIR)/test_s21_memchr.c -L. -l:s21_string.a -o $(TEST_EXEC) $(LDFLAGS)
#./$(TEST_EXEC)
	 @rm -f $(OBJ) *.gcda *.gcno

# general gcov report in HTML format
gcov_report: 
	@echo "WIP"

# clean target
clean:
	rm -f $(OBJ) $(LIB) $(TEST_EXEC)
	rm -f *.gcda *.gcno

.PHONY: all clean test gcov_report
