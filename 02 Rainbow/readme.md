# Rainbow attack

## Goal
Implement a rainbow attack with a success rate of 75% for alphanumeric passwords of length[6-8].

## Structure
Build files are in folder `build`.

Source files are in folder `src`. You can find 3 subfolders, one to [generate the Rainbow Table](@ref be::esi::secl::pn::generateRT), one to do the [Rainbow Attack](@ref be::esi::secl::pn::crack), and some utils.

`rsc` will contain the rainbow table, the cracked passwords and the cracked passwords hashes.

You can find the documentation of the project into the `doc` folder. 

## How to
To set up the project and launch it with default values, use command `make`. It will :
* install or update `sqlite` and `libsqlite3-dev`,
* build the project,
* launch the project (generate the RT and crack some hashes).

To set up the project and launch it with custom values:
* install or update sqlite (wich is require) with command `make setup`,
* build the project with command `make build`.
* generate the rainbow table with command `build/generateRT numberOfHead numberOfReduce passwordsSize`,
* generate the rainbow table with command `build/crackRT numberOfReduce passwordsSize`.

## Know bugs
None

## Authors
43197 Patti Philippe.

43121 Baltofski Nicolas