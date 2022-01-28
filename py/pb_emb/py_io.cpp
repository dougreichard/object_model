#include "py_io.h"

PyStdErrOutStreamRedirect::PyStdErrOutStreamRedirect()
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
std::string PyStdErrOutStreamRedirect::stdoutString()
{
    _stdout_buffer.attr("seek")(0);
    return py::str(_stdout_buffer.attr("read")());
}
std::string PyStdErrOutStreamRedirect::stderrString()
{
    _stderr_buffer.attr("seek")(0);
    return py::str(_stderr_buffer.attr("read")());
}
PyStdErrOutStreamRedirect::~PyStdErrOutStreamRedirect()
{}
void PyStdErrOutStreamRedirect::exit()
{
   auto sysm = py::module::import("sys");
   sysm.attr("stdout") = _stdout;
   sysm.attr("stderr") = _stderr;
   _stdout_buffer.release();
   _stderr_buffer.release();
}
