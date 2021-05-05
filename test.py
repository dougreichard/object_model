import shapes;

def main():
    print("Python module")
    #############################
    ## Pure C++ object wrapped
    c = shapes.Circle()
    print(c.X) # Prints 0
    print(c.R) # prints 1
    print(c.V) # prints None
    c.X = 1234 # changes value
    print(c.X) # Prints 1234

    #############################
    ## Totaly dynamic C++ object wrapped
    o = shapes.Object()
    o.X = 456 # 
    print(o.X) # Prints 1234
    ## NEED a better way to add symbols and avoid duplicates
    s = shapes.Symbol("Gromit")
    o.Gromit = 12
    print(o.Gromit)


if __name__ == "__main__":
    main()