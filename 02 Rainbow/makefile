#NOM	: makefile
#HOWTO	: make; make clean; make run; make build; make install_sqlite
#AUTEUR	: Philippe Patti 43197
#CONTACT: 43197@etu.he2b.be


run : build setup
	@echo
	./build/rainbow-attack

build : build/sha256-main build/gen-passwd build/check-passwd  build/rainbow-attack

build/sha256-main : src/util/sha256-main.cpp src/util/sha256.cpp
	g++ -o build/sha256-main -std=c++17 src/util/sha256-main.cpp src/util/sha256.cpp

build/gen-passwd : src/util/random.hpp src/util/sha256.cpp src/util/gen-passwd.cpp src/util/passwd-utils.hpp
	g++ -o build/gen-passwd -std=c++17 src/util/random.hpp src/util/sha256.cpp src/util/gen-passwd.cpp src/util/passwd-utils.hpp

build/check-passwd : src/util/random.hpp src/util/sha256.cpp src/util/check-passwd.cpp src/util/passwd-utils.hpp
	g++ -o build/check-passwd -std=c++17 src/util/random.hpp src/util/sha256.cpp src/util/check-passwd.cpp src/util/passwd-utils.hpp

build/rainbow-attack : src/util/random.hpp src/util/sha256.cpp src/util/sha256.h src/util/passwd-utils.hpp src/generateRT/generateRT.cpp src/generateRT/generateRT.h src/util/rt-utils.hpp src/crack/crackRT.cpp src/crack/crackRT.h src/main.cpp
	g++ -O3 -o build/rainbow-attack -std=c++17 src/util/random.hpp src/util/sha256.cpp src/util/rt-utils.hpp src/util/passwd-utils.hpp src/generateRT/generateRT.cpp src/crack/crackRT.cpp src/main.cpp -lsqlite3 -pthread

setup : 
	sudo apt install sqlite libsqlite3-dev

clean : 
	rm build/*
