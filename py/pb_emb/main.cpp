
#include <pybind11/iostream.h>
#include <pybind11/embed.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

//#include <codecvt>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;
std::wstring get_absolute_path(std::string rel)
{

    auto dir = fs::path(rel).parent_path();
    //std::experimental::filesystem::path cp = std::experimental::filesystem::
    return dir.wstring();
}

// // convert UTF-8 string to wstring
// std::wstring utf8_to_wstring (const std::string& str)
// {
//     std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
//     return myconv.from_bytes(str);
// }

// // convert wstring to UTF-8 string
// std::string wstring_to_utf8 (const std::wstring& str)
// {
//     std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
//     return myconv.to_bytes(str);
// }

namespace py = pybind11;

class PyStdErrOutStreamRedirect
{
    py::object _stdout;
    py::object _stderr;
    py::object _stdout_buffer;
    py::object _stderr_buffer;

public:
    PyStdErrOutStreamRedirect()
    {
        auto sysm = py::module::import("sys");
        _stdout = sysm.attr("stdout");
        _stderr = sysm.attr("stderr");
        auto stringio = py::module::import("io").attr("StringIO");
        _stdout_buffer = stringio(); // Other filelike object can be used here as well, such as objects created by pybind11
        _stderr_buffer = stringio();
        sysm.attr("stdout") = _stdout_buffer;
        sysm.attr("stderr") = _stderr_buffer;
    }
    std::string stdoutString()
    {
        _stdout_buffer.attr("seek")(0);
        return py::str(_stdout_buffer.attr("read")());
    }
    std::string stderrString()
    {
        _stderr_buffer.attr("seek")(0);
        return py::str(_stderr_buffer.attr("read")());
    }
    ~PyStdErrOutStreamRedirect()
    {
        auto sysm = py::module::import("sys");
        sysm.attr("stdout") = _stdout;
        sysm.attr("stderr") = _stderr;
    }
};

class Animal
{
public:
    virtual ~Animal() {}
    virtual std::string go(int n_times) = 0;
};
class PyAnimal : public Animal
{
public:
    /* Inherit the constructors */
    using Animal::Animal;

    /* Trampoline (need one for each virtual function) */
    std::string go(int n_times) override
    {
        PYBIND11_OVERLOAD_PURE(
            std::string, /* Return type */
            Animal,      /* Parent class */
            go,          /* Name of function in C++ (must match Python name) */
            n_times      /* Argument(s) */
        );
    }
};
class Dog : public Animal
{
public:
    std::string go(int n_times) override
    {
        std::string result;
        for (int i = 0; i < n_times; ++i)
            result += "woof! ";
        return result;
    }
};

class DogOne
{
public:
    std::string go(int n_times)
    {
        std::string result;
        for (int i = 0; i < n_times; ++i)
            result += "woof! ";
        return result;
    }
};

int add(int i, int j)
{
    return i + j;
}

std::string call_go(Animal *animal)
{
    return animal->go(3);
}

PYBIND11_EMBEDDED_MODULE(my_api, m)
{
    // `m` is a `py::module` which is used to bind functions and classes
    m.def("add", &add, "A function which adds two numbers");
    py::class_<Animal, PyAnimal>(m, "Animal")
        .def(py::init<>())
        .def("go", &Animal::go);

    py::class_<Dog, Animal>(m, "Dog")
        .def(py::init<>());

    m.def("call_go", &call_go);
    m.def("djr",
          []()
          {
              return std::string("This string needs to be UTF-8 encoded");
          });
}

int main(int argc, char *argv[])
{
    std::ofstream log;
    log.open("log.txt");
    // Get EXE absolute path
    std::wstring exe = Py_GetProgramFullPath();
    // get dir of the EXE
    std::wstring dir = fs::path(exe).parent_path().wstring();
    //std::string _dir = fs::path(exe).string();
    // Path to python
    std::wstring python = dir; // + L"\\..\\py\\py_emb";

    // string to the 'mod' script
    std::string _dir = fs::path(exe).parent_path().string();
    std::string script = _dir + "\\..\\pb_emb\\script.py";
    std::string _python = _dir; // + "\\..\\py\\py_emb";
    // Build PyPath to the embedded zip file
    log << script;

    // Point to python dir and the python zip
    std::wstring pypath = python + L"\\python39.zip;" + python;
    ;

    Py_SetPath(pypath.c_str());
    Py_SetPythonHome(python.c_str());

    // Let the python begin
    py::scoped_interpreter guard{};
    auto my_api = py::module::import("my_api");

    /// The debugger looks for the file name in the sys.argv[0]
    auto sys = py::module::import("sys");
    sys.attr("argv") = py::make_tuple("script.py", "embed.cpp");
    py::print(sys.attr("version"));

    py::object scope = py::module::import("__main__").attr("__dict__");

    // Remote debugging: Also helpful to debug both C++ and Python
    // Debugg your C++ and then attach to this python
    if (argc >= 2 && argv[1][0] == 'r')
    {
        ///////////////////////////////////////////////////////////////////////
        // This sets up the debugger for Vsiual Studio and Visual Studio Code
        // This allows the python script to be debugged
        auto ptvsd = py::module::import("ptvsd");
        auto en = ptvsd.attr("enable_attach")();
        py::print("Waiting for debugger to attach...");
        auto att = ptvsd.attr("wait_for_attach")();
    }
    else if (argc >= 2 && argv[1][0] == 'd')
    {
        //////////////////////////////////////////////////
        // Alternately start pdb python's debugger
        auto pdb = py::module::import("pdb");
        auto st = pdb.attr("set_trace")();
    }
    printf("Running %s...\n", script.c_str());
    PyStdErrOutStreamRedirect output;

    try
    {
        py::eval_file(script, scope);
    }
    catch (py::error_already_set &e)
    {
        py::print(e.value());
    }
    printf("stdout:\n");
    printf(output.stdoutString().c_str());

    printf("stderr:\n");
    printf(output.stderrString().c_str());
}
