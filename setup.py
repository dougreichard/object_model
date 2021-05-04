from distutils.core import setup, Extension

module1 = Extension('shapes',
                    sources = [ "statics.cpp", 'main_py.cpp'],
                    #  include_dirs=['../../include'],
                    #  library_dirs=['../../target/release/'],
                    #  libraries=['cape_core.dll'],
                    extra_compile_args=["/std:c++latest"]
                    )

setup (name = 'shapes',
       version = '1.0',
       description = 'This is a demo package',
       ext_modules = [module1])