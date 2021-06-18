
#include <pybind11/iostream.h>
namespace py = pybind11;

class PyStdErrOutStreamRedirect
{
    py::object _stdout;
    py::object _stderr;
    py::object _stdout_buffer;
    py::object _stderr_buffer;

public:
    PyStdErrOutStreamRedirect();
    std::string stdoutString();
    std::string stderrString();
    ~PyStdErrOutStreamRedirect();
};
