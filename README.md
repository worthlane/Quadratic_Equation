# Quadratic Equation Solver
## Compilation:
```
g++ main.cpp solver.cpp getout_info.cpp testmode.cpp
```
## Input from:
### INTERACTIVE STDIN:
flag:
```
./a.out -std
./a.out
```
Equation format: ax^2 + bx + c = 0. Program will ask you to enter each coefficient.
### FILE:
flag:
```
./a.out -file
```
You will need to type file directory in console like an argument.
File input format (coefficients with ' ' / '\\t' / '\\n' between):
```
1 1 1              // a1 b1 c1
1   2              // a2 b2
-3                 // c2
300.34   777.424   // a3 b3
-100.324           // c3
```
> [!WARNING]
> Incorrect file input will skip all non-read data and ask user to run program again.
### CONSOLE INPUT
```
./a.out 1 2 -3          // a b c
```
Reads coefficients from console. 
> [!WARNING]
> If input is incorrect (one of coefs is not a number), program returns ERROR.
### STANDART STDIN (In development)
Reads full equation from STDIN (5x^2 + 6x-7+3x^2 =  x - x^2 + 3)
## Help mode
flag -help for calling help
## How does it work?
Program finds the roots using a discriminant formula.
## Repeat:
You can solve another equation. If you want to continue, you should type "1" or bigger number after the program's question. Any other answer will be regarded as "No".
> [!NOTE]
> You can use file input only with first run of the program. If you need to solve another equation, you should enter data using interactive STDIN.
