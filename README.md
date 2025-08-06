# Nim
Play a game of Nim against a computer opponent.

## Command Syntax
`nim [--first|-f|--second|-s] [--misere|--normal|--subtraction] [(--initial|-i) <heap sizes>] [--help|-h]`

### Options
#### Who goes first
- `--first`,`-f`: You go first. (default)
- `--second`,`-s`: The computer goes first.
#### Game Variation
- `--misere`: Play the misère variation. (default)
- `--normal`: Play the normal variation.
- `--subtraction`: Play the subtraction variation.
####  Setup
- `--initial`,`-i`: Initial configuration
  In the **misère** and **normal** variations, provide a space-separated list of heap sizes. In the **subtraction** variation, provide a number
- of objects in the heap followed by the maximum number that can be removed.

## Rules
- The game starts with one or more heaps of objects.
- Players alternate turns.
- On a player's turn, they must remove one or more objects from a single heap.
- The player who takes the last object wins in the **normal** and **subtraction** variations and loses in the **misère** variation.
- The game ends when all heaps are empty.

### Variations
#### Normal
In normal play, the player who takes the last object wins.
#### Misère
In misère play, the player who takes the last object loses.
#### Subtraction
In subtraction games, players can only remove a specific number of objects from a heap.

## Building
### Build Environment
The project uses CMake.
- There is no installation functionality.
- Tests are built if BUILD_TESTING is enabled.
- CMake 3.21 or higher
- C++17 compatible compiler

### Dependencies
- nlohmann_json - for reporting information about the AI's state
- CLI11 - for command line argument parsing
- GTest - for unit testing
- GamePlayer - my generic two-player perfect information game player (https://github.com/jambolo/GamePlayer)
