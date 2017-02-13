Code Style
==========

Code-style for projects.

## Introduction
In this very file, you can check the the code styling and rules conventions for projects. 

## Tabs and indentation
 - Tab size is 4 spaces
 - Indentation is 1 tab size (4 spaces)
 - Empty lines MUST not keep indents
 - Blank lines CAN be added for readability

```c
function foo() {
    int a = 1,
        b = 2;

    if (a === b) {
        printf("OK...");
    }
}
```

## Word format

### Variables

```c
int myVar = 0;
```

### Functions

```c
void my_function() {

}
```

### Structures

```c
struct my_structure;
```

### Preprocessing

```c
#define VAR 3
```

## Spaces

### Before parenthesis
  - **DO NOT ADD** before:
    - function declaration
    - function call


```c
function foo() {
    return 0;
}

foo();
```

  - **ADD** before:
    - 'if'
    - 'for'
    - 'while'
    - 'switch'
    - 'catch'
    - in function expression


```c
if (i > 10) {
    for (int j = 0; j < 10; j++) {
        switch (j) {
            case 0:
                value = 1;
                break;
            case 1:
                value = 2;
                break;
        }
    }
}
```

### Around operators
  - **ADD** around:
    - assignment operators (=, +=, ...)
    - logical operators (&&, ||)
    - equality operators (==, ===, !=, !==)
    - relational operators (<, >, <=, >=)
    - bitwise operators (&, |, ^, ...)
    - additive operators (+, -) and multiplication operators (*,/,%)
    - shift operators (<<,>>,>>>, ...)


```c
int a = 0,
    b = (i == j || j > 5),
    c = (j << 2) & 4,
    d += 1,
    e = a + d;
```

  - **DO NOT ADD** around:
    - unary operators (!, -, +, ++, --)


```c
j++;
bar = !foo;
```

### Before left brace
  - **ADD** before:
    - function
    - 'if'
    - 'else'
    - 'for'
    - 'while'
    - 'do'
    - 'switch'

### Before keywords
  - **ADD** before:
    - 'else'
    - 'while'

### Other
  - **ADD**
    - after comma
    - after property name-value separator (:)
  - **DO NOT ADD**
    - before comma

## Comments

```c
/*
    What the function do...
*/
foo(a, b, c, d); 
```
