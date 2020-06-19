# Laro
Laro is a programming language for working with numbers that compiles down to MIPS.

## Prerequisites
```
qmake
make
git
```
## Build
```
git clone https://github.com/dimitaracev/Laro.git
cd Laro
qmake && make
```
## Usage

Laro consists of functions, conditionals and arithmetic operations.
<br>
Functions in Laro are defined with the keyword func. Laro also comes with a predefined function print that takes 1 parameter.
<br>
A Laro code must contain a main function defined as such:

```
func main()
{

}
```

Arithmetic operations supported by Laro: +, -, *, /, %.
<br>

An example program that prints even numbers up to 100:

```
func main()
{
    x = 0

    while(x < 100)
    {
        even(x)
        x = x + 1
    }
}

func even(x)
{
    mod = x % 2
    if(mod == 0)
    {
        print(x)
    }
}
```



