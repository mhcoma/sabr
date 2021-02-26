[![GitHub license](https://img.shields.io/github/license/mhcoma/sabr?style=for-the-badge)](https://github.com/mhcoma/sabr/blob/main/LICENSE)

# Sabr
Sabr is simple stack-oriented programming language.

# Build from source
```
git clone --depth=1 https://github.com/mhcoma/sabr.git
cd sabr
mkdir build
cd build
cmake ..
make
```

# How to run
## Compile to bytecode
```
sabrc {source file name} {output file name}
```
## Interprete bytecode
```
sabre {bytecode file name}
```

# Specification
Sabr programs must be written in UTF-8.
## Lexical structure
### Tokens
#### Control keywords
* `if`
* `else`
* `loop`
* `while`
* `break`
* `continue`
* `func`
* `return`
* `macro`
* `end`
#### Literals
##### Number literals
* Integers : `255`, `0255`, `0xff`, `0o377`, `0b11111111`
* Floating-point : `0.25`, `.25`, `00.250`, `0.25e0`, `2.5e-1`, `0.025e1`
##### Character literals
* Unicode characters : `'가'` -> `[ 44032 ]`
* Characters sequence : `'Hello\0'` -> `[ 0, 111, 108, 108, 72 ]`
##### Escape sequences
* `\a` -> 7
* `\b` -> 8
* `\e` -> 27
* `\f` -> 12
* `\n` -> 10
* `\r` -> 13
* `\t` -> 9
* `\v` -> 11
* `\\` -> 92
* `\'` -> 39
* `\"` -> 34
* `\nnn` -> The byte whose numerical value is given by nnn interpreted as an octal number
* `\xhh` -> The byte whose numerical value is given by hh… interpreted as a hexadecimal number
* `\uhhhh` -> Unicode code point below 10000 hexadecimal
* `\Uhhhhhhhh` -> Unicode code point where h is a hexadecimal digit
#### Identifiers
Control keywords, Literals cannot become identifiers.
* Identifier value : `$main`, `$a`  
Use for define functions, macros, variables.
* Identifier call : `main`, `a`  
Get values of variables or call functions and macros.
### Comments
* Line comments : `\this is comment`, `\ this is comment`, `2 5 + \this is comment`
