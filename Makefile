CC=gcc -std=c11 -D_GNU_SOURCE
CFLAGS=-c -Wall -Wextra -Werror
GCOV=-fprofile-arcs -ftest-coverage

OS=$(shell uname)

ifeq ($(OS), Linux)
	OS_LIBS=-lcheck -lsubunit -lrt -lpthread -lm
	CC+=-D OS_LINUX -g -s
	CHECK_LEAKS=CK_FORK=no valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=log.txt
else
	OS_LIBS=-lcheck
	CC+=-D OS_MAC
	CHECK_LEAKS=CK_FORK=no leaks --atExit --
	OPEN_GCOV=open coverage/index.html
endif

all: clean s21_matrix.a gcov_report

s21_matrix.a:
	@$(CC) $(CFLAGS) s21_matrix.c
	@ar -rcs s21_matrix.a s21_matrix.o
	@ranlib $@
	@cp $@ lib$@

test: clean s21_matrix.a
	@$(CC) $(CFLAGS) s21_matrix_test.c
	@$(CC) s21_matrix_test.o s21_matrix.a -o Test $(OS_LIBS)
	@./Test
	@rm -rf *.o *.a Test

gcov_report: s21_matrix.c s21_matrix_test.c s21_matrix.h
	@$(CC) s21_matrix_test.c s21_matrix.c -o Test $(OS_LIBS) $(GCOV)
	@./Test
	@lcov -c -d . -o coverage.info
	@genhtml coverage.info -o coverage
	@$(OPEN_GCOV)
	
leaks: clean s21_matrix.a
	@$(CC) s21_matrix_test.c s21_matrix.a $(OS_LIBS) -o ./Test
	@$(CHECK_LEAKS) ./Test
	@rm -f *.o *.a Test

style:	
	@cp ../materials/linters/CPPLINT.cfg ./
	@python3 ../materials/linters/cpplint.py --extension=c s21_matrix.c s21_matrix.h s21_matrix_test.c
	@rm -f CPPLINT.cfg

cppcheck:
	@cppcheck s21_matrix.c s21_matrix.h s21_matrix_test.c

check: style cppcheck leaks

clean:
	@rm -rf *.o *.a *.out *.txt *.gcno *.gch *.gcda *.info coverage Test

rebuild: clean s21_matrix.a
	@rm -rf *.o