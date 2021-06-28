# Sieve of Eratosthenes

C++ program compiled with [Mingw-w64](http://mingw-w64.org/doku.php) for
Windows cross-compatibility.

![Demo](./demo/prime_2000.gif)

## Usage

For Windows, just simply run the program.

> Note: Windows Defender might flag the executable as potential harmful program.
> Ignore the warning and proceed. Its not a virus. I'm not about that life.

For Unix systems

```sh
./bin/sieve_of_eratosthenes.exe [ -h ] [ -n <integer> ]
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

