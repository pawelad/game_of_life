Game of Life
============

It's a C implementation of Conway's [Game of Life](http://en.wikipedia.org/wiki/Conway's_Game_of_Life) simulator made for my university assigment.

##Instalation
The program is meant to run on Linux.

Makefile is included, so you can compile it simply by typing `make` which creates an executable `bin/life_sym` file.

You can also compile it with debug flag `make DEBUG=1 -B` which creates `bin/life_sym_debug` with additional output messages.

You can clean everything up (except results) with `make clean`.

##Usage

```
./life_sym [options]

    -h, --help                        show this help message and exit
    -i, --input=<str>                 path to input file
    -o, --output=<str>                path to output file
    -n, --generation_number=<int>     number of generations to simulate
    -p, --photos_number=<int>         number of photos to generate
    -d, --dir=<str>                   directory for results
    -D, --photo_dir=<str>             subdirectory for photos
    -m, --mod_file=<str>              path to rules modification file
    -M, --mod_input=<str>             rules modification    
```

Options: `--input`, `--output`, `--generation_number` and `--photos_number` are required to run program.

##File format
Sample files are included in the `sample` directory.

### Netting file
Sample input net file contains:

1. Grid dimensions (2D)
2. Coordinates of a living cell

### Rules file
Sample rules file is made of:

1. Numbers of living neighbors on which living cells remain alive
2. Slash (`/`)
3. Numbers of living neighbors on which dead cells become alive

So the standard rules in that format are `23/3`.

##Used libraries
- [C standard library](http://en.wikipedia.org/wiki/C_standard_library)
- [C POSIX library](http://en.wikipedia.org/wiki/C_POSIX_library)
- [libpng](http://www.libpng.org/pub/png/libpng.html) --- saving png files
- [argparse](https://github.com/Cofyc/argparse) --- parsing arguments (included)

##To do
- more thorough testing
- consider better memory managment with storing net (CRS?)
- better makefile ?

##Suggestions
It's my first 'proper' programming project, so if you have any suggestion about anything (directory tree, modules, functions, code clarity, comments placement, README file etc.) then let me know.
Thanks!