# Quadratic Equation Solver
## Compilation:
```
make
```
## Flags:
Equation format: ax^2 + bx + c = 0. Program will ask you to enter each coefficient in interactive type of run.
### Input flags:
flag:
```
-ff, --fromfile [target]        flag for input from file [target]
-i , --stdin                    flag for input from stdin
-c , --console  [a b c]         flag for console input (a b c - coefficients)
```
You will need to type file directory if you choose file input format (coefficients with ' ' / '\\t' / '\\n' between):
```
1 1 1              // a1 b1 c1
1   2              // a2 b2
-3                 // c2
300.34   777.424   // a3 b3
-100.324           // c3
```
> [!WARNING]
> Incorrect file input will ask user to run program again.
### Output flags:
```
-tf, --tofile [target]         flag for output into file [target]
-o , --stdout                  flag for output in stdout
``` 
### Type flags:
```
-h , --help [target]           flag for help mode (or info about [target])
``` 
## How does it work?
Program finds the roots using a discriminant formula.
## Repeat:
Program will output the menu, in which you can choose how to continue run of the program.
