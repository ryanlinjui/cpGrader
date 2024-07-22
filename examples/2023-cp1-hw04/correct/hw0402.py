from sympy import symbols, expand, diff

def main():
    f_degree = int(input("Please enter f(x) degree: "))
    f_coefficients = list(map(int, input("Please enter f(x) coefficients: ").split()))
    g_degree = int(input("Please enter g(x) degree: "))
    g_coefficients = list(map(int, input("Please enter g(x) coefficients: ").split()))
    
    x = symbols("x")
    f_x = sum(c * x**(f_degree - i) for i, c in enumerate(f_coefficients))
    g_x = sum(c * x**(g_degree - i) for i, c in enumerate(g_coefficients))
    fg_m = diff(expand(f_x * g_x))
    fg_d = diff(expand(f_x / g_x))

    print(f"f(x): {f_x}")
    print(f"g(x): {g_x}")
    print(f"(f(x)g(x))': {fg_m}")
    print(f"(f(x)/g(x))': {fg_d}")
    
if __name__ == "__main__":
    main()
