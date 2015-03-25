        __     _
    |  /  | __| |
    | /  /_| _   >
    |/  / _|<_| |
    |__/___|__|_|
    A simple Vector-Graphic editor for Retriever.

Vect is a simple vector graphics editor coded back in 1998 using
Allegro/DOS. As of 2015, it still compiles under Linux with only a few
minor tweaks. It's not exactly very feature rich, but it is able to
produce some graphics. Examples are provided in the `graphics/`
directory.


Compilation
-----------

Vect requires Allegro, which can be found under Ubuntu in the package
`liballegro4-dev`. Compilation works via `cmake`:

    mkdir build
    cd build
    cmake ..
    make


Running
-------

To run `vect`:

    build/vect

Command line options:

    $ build/vect  -h

    Usage: VECT.EXE [OPTIONS]
    ~~~~~~~~~~~~~~~~~~~~~~~~~
    -p PATH    Set the default path for load/save
    -f FILE    Open FILE, for editing (buggy)
    -h XXX     Use screenmode with height XXX
    -w XXX     Use screenmode with width XXX
    -v         Print Version

Right click opens a context menu, left clicking does actions. The file
`keys.txt` contains some further info.

