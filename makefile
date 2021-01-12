exec: main.o gogen.o
	  	g++ -Wall -g main.o gogen.o -o exec

main.o: main.cpp gogen.h mask.h common.h
		g++ -Wall -g -c main.cpp

gogen.o: gogen.cpp gogen.h mask.h common.h
		  g++ -Wall -g -c gogen.cpp

clean:
	  rm -f *.o exec