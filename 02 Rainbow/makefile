#NOM	: makefile
#HOWTO	: make; make clean; make run; make build
#AUTEUR	: Philippe Patti 43197
#CONTACT: 43197@etu.he2b.be


run : build
	./build/gen-passwd
	./build/check-passwd

build : 
	g++ -o gen-passwd -std=c++17 random.hpp sha256.cpp gen-passwd.cpp passwd-utils.hpp
	g++ -o check-passwd -std=c++17 random.hpp sha256.cpp check-passwd.cpp passwd-utils.hpp

clean : 
	rm build/gen-passwd
	rm build/check-passwd