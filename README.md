# Simple-RPC

Limited RPC-like protocol with automated server code generation.

## Building

Building everything:

```
mkdir build
cd build
cmake ..
make
```

All examples will be located at `examples` directory.

## Running examples

### Calculator

**Path:** `examples/calculator`.

**Path to executable (relative to build path):**
`examples/calculator/CalculatorServer`.

Calculator with following operations:

| Procedure name | Arguments    | Return type | Description                      |
|----------------|--------------|-------------|----------------------------------|
| add            | int x, int y | int         | x + y                            |
| subtract       | int x, int y | int         | x - y                            |
| multiply       | int x, int y | int         | x * y                            |
| divide         | int x, int y | int         | x / y if y is not 0, otherwise 0 |
| modulo         | int x, int y | int         | x % y if y is not 0, otherwise 0 |
| negate         | int x        | int         | -x                               |

### REPL

**Path:** `examples/repl`.

**Path to executable (relative to build path):** `examples/repl/REPL`.

Simple-RPC REPL for testing purposes. Each query is on separate line
in standard input.

**Example (interacting with calculator):**

```
>>> add 1 2
Result: 3
>>> divide 10 3
Result: 3
```
