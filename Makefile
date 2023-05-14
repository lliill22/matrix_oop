gcc = g++ -std=c++17 -Wall -Werror -Wextra

all: test

clean:
	rm -rf *.o test_exec *.a *.gc* report *.info *.dSYM a.out gcov_report 

test: s21_matrix_oop.a
	$(gcc)  s21_matrix_oop.a test.cc -lgtest -lgtest_main -pthread -g -fsanitize=address -o test_exec 
	./test_exec

s21_matrix_oop.a: s21_matrix_oop.o
	ar rcs s21_matrix_oop.a *.o

s21_matrix_oop.o: s21_matrix_oop.h
	$(gcc) -c *.cc 

gcov_report: clean
	$(gcc) --coverage *.cc -o gcov_report -lgtest -lgtest_main -lstdc++
	./gcov_report
	lcov -t "stest" -o unit_test.info -c -d .
	genhtml -o report unit_test.info
	open ./report/index.html

style:
	cp ../materials/linters/.clang-format ./
	clang-format -n *.cc *.h
	rm .clang-format