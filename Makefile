compile: main.o parser.o scanner.o checkSem.o assembly.o
	g++ -o compile main.o parser.o scanner.o checkSem.o assembly.o

main.o: main.cpp token.h parser.h node.h checkSem.h assembly.h
	g++ -g -c main.cpp

parser.o: parser.cpp parser.h scanner.h token.h node.h
	g++ -g -c parser.cpp

scanner.o: scanner.cpp scanner.h token.h
	g++ -g -c scanner.cpp

checkSem.o: checkSem.cpp checkSem.h node.h
	g++ -g -c checkSem.cpp

assembly.o: assembly.cpp assembly.h node.h
	g++ -g -c assembly.cpp

clean:
	rm *.o compile
