# Game of Life
C implementation of Conway's [Game of Life](http://en.wikipedia.org/wiki/Conway's_Game_of_Life)
simulator made for my university assigment.

Main features:
- read first generation from file
- simulate it given number of times
- generate a given number of `PNG` files
- modify rules of the game (either by file or directly from terminal)

All the options are listed in the [Usage](#usage) section.

## Instalation
The program is meant to run on Linux and wasn't tested on other platforms.  
Makefile is included, so you can compile it simply by typing `make` in termial, which creates an executable `bin/life_sym` file.
You can also compile it with debug flag `make DEBUG=1 -B` which creates `bin/life_sym_debug` with additional output messages.

You can clean everything up (except results) with `make clean`.

## Usage

```
./life_sym [options]

    -h, --help                        show this help message and exit
    -i, --input=<str>                 path to input file
    -n, --generation_number=<int>     number of generations to simulate
    -p, --photos_number=<int>         number of photos to generate
    -s, --scale=<int>                 graphic enlargement scale factor (default: 2)
    -d, --dir=<str>                   results directory (default: results)
    -D, --photo_dir=<str>             photos subdirectory (default: gfx)
    -r, --random                      creates random net; overrides input file
    -m, --mod_file=<str>              path to rules modification file
    -M, --mod_input=<str>             rules modification string   
```
Options: `--input`, `--generation_number` and `--photos_number` are required to run program.

## File format
Sample input files are included in the `sample` directory.

### Netting file
Input net file contains:

1. Grid dimensions (2D)
2. Coordinates of living cells

### Rules file
Rules file is made of:

1. Numbers of living neighbors on which living cell remain alive
2. Slash (`/`)
3. Numbers of living neighbors on which dead cell becomes alive

So the standard rules in that format are `23/3`.

## Used libraries
- [C standard library](http://en.wikipedia.org/wiki/C_standard_library)
- [C POSIX library](http://en.wikipedia.org/wiki/C_POSIX_library)
- [libpng](http://www.libpng.org/pub/png/libpng.html)
- [argparse](https://github.com/Cofyc/argparse) (included)

## Contributions
Feel free to use, ask, fork, star, report bugs, fix them,
suggest enhancements and point out any mistakes.

## Authors
Developed and maintained by [Paweł Adamczak](https://github.com/pawelad).

Released under [MIT License](LICENSE).
