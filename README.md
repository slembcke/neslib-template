# neslib-template
Template NES project in C using neslib.

Should build OK on MacOS X, Linux, and Cygwin. Has a dependency on libpng for the png2chr tool, so make sure you have a "dev" package for that installed or built.

Rough build instructions:
* Checkout the cc65 submodule: `git submodule update --init --recursive`
* Build cc65: `pushd tools/cc65; make; popd`
* Build the other tools: `pushd tools; make; popd`
* Build the NES ROM: `make`

Enjoy some NES coding?
