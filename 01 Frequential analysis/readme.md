# Frequential analysis #
## Project description ##
Ciphers, deciphers and solve a text using the Vigenere algo.
This application is impemented with the index of coïncidence and the chi square test. Because of that, solving some small text can sometimes fail (try to solve file _./exemple/breakTheSolver.txt_. The key valid key is _CIPHERS_). To avoid this problem, a good way is to use the _local random search_ (which is not used here).
## How to build ##
Run the makefile from the root directory.
- _make_ to build into _./build_ the project and launch a demo.
- _make build_ to build only.
- _make clean_ to clean the _./build_ directory
## How to use ##
- To ciphers, run the program with arguments : _-c in out key_
  - _-c_ tell to the programm to ciphers.
  - _in_ is the input file. The alphabetical values must be coded same as ASCII.
  - _out_ is the output file.
  - _key_ is the key to cipher. The key is set upper case, and all non alphabetical letters are dropped.
- To deciphers, run the program with argumets : _-d in out key_
  - _-d_ tell to the programm to deciphers.
  - _in_ is the input file. The alphabetical values must be coded same as ASCII. All caracters must be upper case, between A and Z.
  - _out_ is the output file. The output is upper case.
  - _key_ is the key to decipher. The key is set upper case, and all non alphabetical letters are dropped.
- To solve a ciphered message, use the program with arguments  _-s in out_
  - _-s_ tell to the programm to deciphers without the key.
  - _in_ is the input file. All caracters must be upper case, between A and Z. The alphabetical values must be coded same as ASCII. The text is supposed to be English
  - _out_ is the output file. The output is upper case.
