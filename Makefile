all: mytest.o utree.o dtree.o
	g++ -Wall -std=c++17 mytest.o utree.o dtree.o -o output.out

mytest.o: mytest.cpp
	g++ -Wall -std=c++17 -c mytest.cpp

dtree.o: dtree.cpp
	g++ -Wall -std=c++17 -c dtree.cpp

utree.o: utree.cpp
	g++ -Wall -std=c++17 -c utree.cpp

run:
	./output.out

val:
	valgrind --leak-check=full ./output.out

# -f removes any error messages if the file is not present
clean:
	rm -rf *.o *.out
	rm -f *~ *.h.gch *#
	clear
	ls


