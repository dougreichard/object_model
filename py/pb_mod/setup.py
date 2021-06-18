from distutils.core import setup, Extension

module1 = Extension('shapes',
                    sources = [  
                     'mod.cpp', 
                     ],
                      include_dirs=['../../include', '../../shapes/include', '../../extern/pybind11/include'],
                    #  library_dirs=['../../target/release/'],
                    #  libraries=['some.dll'],
                    extra_compile_args=["/std:c++latest"]
                    )

setup (name = 'shapes',
       version = '1.0',
       description = 'This is a demo package',
       ext_modules = [module1])