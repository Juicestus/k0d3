# k0d3
The C++ and Java (behind) implementation of the K0D3 esoteric programming language. Probably Turing complete?
**THE JAVA IMPLEMENTATION IS BEHIND THE C++ IMPLEMENTATION IN FEATURES**

## Run
### C++ Implementation
Build using the buildcpp script.\
$ ```./buildcpp.sh```\
Run the binary with input files\
$ ```./k0d3 examples/helloworld.k0d3```\
### Java Implementation
I guess the easiest way is to just use the Java CLI interpreter.\
```java k0d3.java examples/helloworld.k0d3```

## Writting .k0d3 files
### General semantics
Comments are contained withing parentheses.\
```(This is a comment)```
All statements are terminated with a semicolon.\
Elements of a statement are deliminated by spaces.\
```_ + #a #b #c```
### Goto Pointer
Statements start with a "goto pointer".\
Statements that should not be jumped back to should start with an underscore.\
```_ " 12 13;   (Prints out Ascii 12 and 13)```\
A statement that should be jumped back to with ? command should jave a unique identifier at the beginning.\
```a " 12 13;```
### Commands
The second element of a statement is the command.
**A list of commands**
| Command      | Description | 
| ------------ | ----------- | 
| _ | No command on this line | 
| $ | If param 1 is true (n > 0) jump to line with goto pointer of param 2 |
| # | Prints a number to the console takes unlimited params | 
| " | Prints the Ascii equivalent of numbers, takes unlimited params | 
| ? | Waits for number input from console writing to param 1 as number |
| + | Adds param 1 and 2 together and writes to param 3 as number |
| - | Subtracts param 1 by param 2 and writes to param 3 as number |
| * | Multiplies param 1 and 2 together and writes to param 3 as number |
| / | Divides param 1 by param 2 and writes to param 3 as number |
| % | Performs modulus operation on param 1 and 2, writes to param 3 as number |
| @ | Rounds param 1 to the digits of param 2 and writes to param 3 as number |
| = | Checks for equality between param 1 and 2, writes the result to param 3 as boolean |
| < | Checks if param 1 is less than param 2, writes the result to param 3 as boolean |
| > | Checks if param 1 is greater than param 2, writes the result to param 3 as boolean |
| ! | Performs the NOT operation on param 1, writes to param 2 as boolean |
| & | Performs the AND operation on param 1 and 2, writes to param 3 as boolean |
| \| | Performs the OR operation on param 1 and 2, writes to param 3 as boolean |

### Params
Everything else in the statments are parameters.\
A parameter can have a value constant in the format of a double,
or it can be a variable.\ 
A variable can be a number, deliminated by #.\
A variable can be a boolean, deliminated by ?.\
A variable can be a goto pointer, deliminated by $.\


