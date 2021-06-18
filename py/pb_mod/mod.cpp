#include <pybind11/pybind11.h>

struct Simulation {
    uint32_t AddSpaceObject(std::string ai, std::string data, bool a, bool b){return 1;}

};
Simulation theSimulation;



namespace py = pybind11;

PYBIND11_MODULE(learn, m) {
    m.doc() = "Shapes module"; // optional module docstring
    m.def("Simulation", theSimulation);

    py::class_<Simulation>(m, "Simulation");


}