# 2048 in C

A portable console implementation of the 2048 game written in C.

## Build

Using Make:

```sh
make
```

Or compile directly:

```sh
gcc -std=c11 -O2 -Wall -Wextra -pedantic src/main.c -o 2048
```

## Run

```sh
./2048
```

On Windows PowerShell, run:

```powershell
.\2048.exe
```

## Controls

- `W`, `A`, `S`, `D` to move
- `R` to restart
- `Q` to quit