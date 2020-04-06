#NOM	: makefile
#HOWTO	: make; make clean; make run; make build
#AUTEUR	: Philippe Patti 43197
#CONTACT: 43197@etu.he2b.be


run : build
	@echo
	./build/generateRT

build : build/sha256-main build/gen-passwd build/check-passwd  build/generateRT

build/sha256-main : src/util/sha256-main.cpp src/util/sha256.cpp
	g++ -o build/sha256-main -std=c++17 src/util/sha256-main.cpp src/util/sha256.cpp

build/gen-passwd : src/util/random.hpp src/util/sha256.cpp src/util/gen-passwd.cpp src/util/passwd-utils.hpp
	g++ -o build/gen-passwd -std=c++17 src/util/random.hpp src/util/sha256.cpp src/util/gen-passwd.cpp src/util/passwd-utils.hpp

build/check-passwd : src/util/random.hpp src/util/sha256.cpp src/util/check-passwd.cpp src/util/passwd-utils.hpp
	g++ -o build/check-passwd -std=c++17 src/util/random.hpp src/util/sha256.cpp src/util/check-passwd.cpp src/util/passwd-utils.hpp

build/generateRT : src/util/random.hpp src/util/sha256.cpp src/util/passwd-utils.hpp src/generateRT/generateRT.cpp src/generateRT/main.cpp
	g++ -o build/generateRT -std=c++17 src/util/random.hpp src/util/sha256.cpp src/util/passwd-utils.hpp src/generateRT/generateRT.cpp src/generateRT/main.cpp

clean : 
	rm build/sha256-main
	rm build/gen-passwd
	rm build/check-passwd
	rm build/generateRT
