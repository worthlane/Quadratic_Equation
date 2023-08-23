# Quadratic Equation Solver
## Compilation:
```
make
```
## Flags:
### Type flags:
flag:
```
-int            interactive type of equation enter
-std            standart type of equation enter (a1x^2 + b1x - a2x^2 + c1 ...) (coming soon)
```
Equation format: ax^2 + bx + c = 0. Program will ask you to enter each coefficient in interactive type of run.
### Input flags:
flag:
```
-fromfile       flag for input from file
-stdin          flag for input from stdin
-console a b c  flag for console input (a b c - coefficients)
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
 -tofile         flag for output into file
 -stdout         flag for output in stdout
 ``` 
### Type flags:
```
-test flag for test mode activation (for developers)
-help flag for help mode
``` 
flag -help for calling help
## How does it work?
Program finds the roots using a discriminant formula.
## Repeat:
Program will output the menu, in which you can choose how to continue run of the program.
