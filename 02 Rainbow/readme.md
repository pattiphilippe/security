# Rainbow attack

## Goal
The objective of this homework is to implement an attack on password tables with a rainbow table.

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

## Know bugs
None.

## Authors
43197 Patti Philippe.

43121 Baltofski Nicolas