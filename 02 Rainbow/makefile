#NOM	: makefile
#HOWTO	: make; make clean; make run; make build; make install_sqlite
#AUTEUR	: Philippe Patti 43197
#CONTACT: 43197@etu.he2b.be


run : build #setup
	@echo
	#./build/generateRT
	./build/crackRT

build : build/generateRT build/crackRT #build/sha256-main build/gen-passwd build/check-passwd  

build/sha256-main : src/util/sha256-main.cpp src/util/sha256.cpp
	g++ -o build/sha256-main -std=c++17 src/util/sha256-main.cpp src/util/sha256.cpp

build/gen-passwd : src/util/random.hpp src/util/sha256.cpp src/util/gen-passwd.cpp src/util/passwd-utils.hpp
	g++ -o build/gen-passwd -std=c++17 src/util/random.hpp src/util/sha256.cpp src/util/gen-passwd.cpp src/util/passwd-utils.hpp

build/check-passwd : src/util/random.hpp src/util/sha256.cpp src/util/check-passwd.cpp src/util/passwd-utils.hpp
	g++ -o build/check-passwd -std=c++17 src/util/random.hpp src/util/sha256.cpp src/util/check-passwd.cpp src/util/passwd-utils.hpp

build/generateRT : src/util/random.hpp src/util/sha256.cpp src/util/passwd-utils.hpp src/generateRT/generateRT.cpp src/generateRT/generateRT.h src/generateRT/main.cpp src/util/rt-utils.hpp
	g++ -O3 -o build/generateRT -std=c++17 -Wall src/util/random.hpp src/util/sha256.cpp src/util/rt-utils.hpp src/util/passwd-utils.hpp src/generateRT/generateRT.cpp src/generateRT/main.cpp -lsqlite3 -pthread

build/crackRT : src/generateRT/generateRT.h src/crack/main.cpp src/crack/crackRT.cpp src/crack/crackRT.h src/util/random.hpp src/util/rt-utils.hpp
	g++ -O3 -o build/crackRT -std=c++17 src/util/passwd-utils.hpp src/util/rt-utils.hpp src/util/sha256.cpp src/crack/crackRT.cpp src/crack/main.cpp src/util/random.hpp -lsqlite3 -pthread

setup : 
	sudo apt install sqlite libsqlite3-dev

clean : 
	rm build/*
