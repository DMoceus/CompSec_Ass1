GPP = g++ -g

all: RC6

RC6: RC6.o
	$(GPP) RC6.o -o RC6

RC6.o: RC6.cpp
	$(GPP) -c RC6.cpp

clean:
	rm -f *.o *~core RC6
