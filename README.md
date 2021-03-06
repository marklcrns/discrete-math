# Discrete Mathematics Algorithms

Discrete Mathematics compilation of algorithms for homeworks and projects

Compiled with [Mingw-w64](http://mingw-w64.org/doku.php) for
Windows cross-compatibility.

![Demo](./demo/demo.gif)

## Usage

For Windows, just simply run the program `bin/pos_int_algo_sols.exe`.

> Note: Windows Defender might flag the executable as potentially harmful
> program. Ignore the warning and proceed. Its not a virus. I'm not about that
> life.

For Unix systems

```sh
./bin/pos_int_algo_sols.exe
```

or

```
make run
```

## Building from Source

Requirements

- mingw-w64
  - Debian-based system installation `sudo apt install mingw-w64`

```sh
# Simply run make from project root
make

# Building for 32-bit system (64-bit default)
make MINGW_W32=1
```

