# Rainbow attack

## Goal
The objective of this homework is to implement an attack on password tables with a rainbow table with a success rate of 75%.

## Structure
Build files are in folder `build`.

Source files are in folder `src`. You can find 3 subfolders, one to [generate the Rainbow Table](@ref be::esi::secl::pn::generateRT), one to do the [Rainbow Attack](@ref be::esi::secl::pn::crack), and some utils.

`rsc` will contain the rainbow table, the cracked passwords and the cracked passwords hashes.

## How to
To set up the project and launch it with default values, use command `make`. It will :
* install or update `sqlite`, `libsqlite3-dev`, `gcc-8` and `g++-8`,
* build the project,
* launch the project (generate the RT and crack some hashes).

To set up the project and launch it :
* install or update sqlite and gcc/g++ (wich are require) with command `make setup`,
* build the project with command `make build`.
* generate the rainbow table with command `build/generateRT numberOfHead numberOfReduce`,
* generate the rainbow table with command `build/crackRT`.

## Important note
The constraints of the homework are :
* passwords of length 6 to 8,
* alphanumeric passwords,
* 75% success rate,
* size of RT 12Gib or lower,
* 50.000 reduces (indicated in the course),
* the hashes to crack are probably generated with [rainbow::mass_generate](@ref rainbow::generate_passwd), which mean there is 33% of chance to get a password of size 6, 7 or 8.

The util to [generate the passwords](@ref rainbow::generate_passwd), provided by the teachers, generate alphanumerical passwords, lower and uppercase.
The point is that we can't build a table smaller than 12Gib which reach 75% success rate. Here is why :
* each row take at least 8*2 = 16 byte (at least because the DB hold some informations),
* an 12Gib table can hold 12.884.901.888/16 = 805.306.368 rows,
* with 805.306.368 rows, 50.000 reduce, a password of lenght 8, and 62 possible values for each char of the password, you can expect a success rate lower than 20% (according to the formula 3, page 6, of [this work](https://lasecwww.epfl.ch/pub/lasec/doc/Oech03.pdf)),
* but we need at least 25% of success to get 75% of global success (`(100+100+25)/3`)

> Due to these problems, we decided to work with the same constraints, except for the alphanumerical password. We will use alphanumerical **lower case** password only

## Know bugs
None.

## Authors
43197 Patti Philippe.

43121 Baltofski Nicolas