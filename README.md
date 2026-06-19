# 2048 in C

A portable console implementation of the 2048 game written in C.

The game now supports per-user accounts with a username, password, saved board state, and high score.

## Build

Using Make:

```sh
make
```

Or compile directly:

```sh
gcc -std=c11 -O2 -Wall -Wextra -pedantic src/*.c -o 2048
```

## Run

```sh
./2048
```

On Windows PowerShell, run:

```powershell
.\2048.exe
```

## Account Flow

When the game starts, choose one of these options:

- `L` to log in to an existing account
- `R` to register a new account
- `Q` to quit

Each account stores:

- current game state, if one exists
- username and password
- high score

## Controls

- `W`, `A`, `S`, `D` to move
- `R` to restart
- `L` to log out and return to the account screen
- `Q` to quit