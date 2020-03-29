#NOM    : Makefile
#OBJET  : Compile project
#HOWTO	: make; make clean; make run; make build

run : build
	./build/vigenere -c ./exemple/bigPlain.txt ./exemple/bigCiphered.txt MYKEY
	./build/vigenere -d ./exemple/bigCiphered.txt ./exemple/bigDeciphered.txt MYKEY
	./build/vigenere -s ./exemple/bigCiphered.txt ./exemple/bigSolved.txt

build : chi.o ic.o vigenere.o main.o
	g++ ./build/*.o -o ./build/vigenere

chi.o : ./src/chi.cpp
	g++ -c ./src/chi.cpp -o ./build/chi.o

ic.o : ./src/IC.cpp
	g++ -c ./src/IC.cpp -o ./build/ic.o

main.o : ./src/main.cpp
	g++ -c ./src/main.cpp -o ./build/main.o

vigenere.o : ./src/vigenere.cpp
	g++ -c ./src/vigenere.cpp -o ./build/vigenere.o

clean:
	rm -f ./build/* 
