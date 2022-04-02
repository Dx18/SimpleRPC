int add(int x, int y)
{
    return x + y;
}

int subtract(int x, int y)
{
    return x - y;
}

int multiply(int x, int y)
{
    return x * y;
}

int divide(int x, int y)
{
    if (y == 0) {
        return 0;
    }
    return x / y;
}

int modulo(int x, int y)
{
    if (y == 0) {
        return 0;
    }
    return x % y;
}

int negate(int x)
{
    return -x;
}
