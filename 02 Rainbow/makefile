#NOM	: makefile
#HOWTO	: make; make clean; make run; make build
#AUTEUR	: Philippe Patti 43197
#CONTACT: 43197@etu.he2b.be


run : build
	./build/gen-passwd 1000 6 8 rsc/pwd.txt rsc/hash.txt
	./build/check-passwd rsc/pwd.txt rsc/hash.txt

build : build/gen-passwd build/check-passwd 

build/gen-passwd : src/util/random.hpp src/util/sha256.cpp src/util/gen-passwd.cpp src/util/passwd-utils.hpp
	g++ -o build/gen-passwd -std=c++17 src/util/random.hpp src/util/sha256.cpp src/util/gen-passwd.cpp src/util/passwd-utils.hpp

build/check-passwd : src/util/random.hpp src/util/sha256.cpp src/util/check-passwd.cpp src/util/passwd-utils.hpp
	g++ -o build/check-passwd -std=c++17 src/util/random.hpp src/util/sha256.cpp src/util/check-passwd.cpp src/util/passwd-utils.hpp


clean : 
	rm build/gen-passwd
	rm build/check-passwd
