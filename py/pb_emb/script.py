import my_api

help(my_api)

def test():
    d = my_api.Dog()
    print(my_api.call_go(d))
    print(d.go() )
