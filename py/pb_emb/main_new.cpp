
#include <pybind11/iostream.h>
#include <pybind11/embed.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "crc.h"

//#include <codecvt>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

#include "py_io.h"


PYBIND11_EMBEDDED_MODULE(pax, m)
{
    // `m` is a `py::module` which is used to bind functions and classes
}

namespace glm {
    struct vec2 {
        double x;
        double y;
    };
    struct vec3 {
        vec3(double x=0, double y=0, double z=0): x(x), y(y), z(z) {}
        double x;
        double y;
        double z;
    };
};
PYBIND11_EMBEDDED_MODULE(glm, m)
{
    using namespace glm;
    m.doc() = "GLM module";
    py::class_<vec2>(m, "vec2")
		.def_readwrite("x", &glm::vec2::x)
		.def_readwrite("y", &glm::vec2::y)
		;
	
	py::class_<vec3>(m, "vec3")
        .def(py::init<double,double,double>())
        .def(py::init<>())
		.def_readwrite("x", &vec3::x)
		.def_readwrite("y", &vec3::y)
		.def_readwrite("z", &vec3::z)
		;

}

// py::enum_<Pet::Kind>(pet, "Kind")
//     .value("Dog", Pet::Kind::Dog)
//     .value("Cat", Pet::Kind::Cat)
//     .export_values()
using namespace glm;
// struct vec3 {
//         double x;
//         double y;
//         double z;
//     };
struct SpaceObject {
    vec3 pos;
    SpaceObject () {
        pos.x = 1.0;
        pos.y = 2.0;
        pos.z = 3.0;
    }
    void move(vec3 v) {
        this->pos.x += v.x;
        this->pos.y += v.y;
        this->pos.z += v.z;
    }
};

struct ObjectHandle {
    int32_t handle;
};

struct Simulation {
    std::unordered_map<uint32_t, SpaceObject*> objects;
    std::unordered_set<uint32_t> bin;
    ~Simulation() {
        std::cout << "WTF";
       // clear();
    }

    SpaceObject* object_exists(uint32_t handle) {
        auto got = objects.find(handle);
        if (got != objects.end())  {
            return got->second;
        }
        return nullptr;
    }
    uint32_t AddSpaceObject(std::string aiTag, std::string dataTag, bool isPlayer, bool isActive) {
        auto ID = crc(dataTag.begin(), dataTag.end());
        SpaceObject* object = new SpaceObject();
        objects.insert(std::make_pair(ID, object));
        return ID;

    }
    void DeleteSpaceObject(uint32_t ID) {
        bin.insert(ID);
    }
    
    // take out trash
    void recycle() {
        for (const auto ID : bin) {
            auto got = objects.find(ID);
            if (got != objects.end())  {
                delete got->second;
            }
        }
        bin.clear();
    }

    void clear() {
        for (const auto & [ key, value ] : objects) {
            delete value;
        }
        objects.clear();
    }


    static  Simulation& instance() {
        static Simulation* simulation = new Simulation();
        return *simulation;
    }

      Simulation() = default;
private:
  // Delete copy/move so extra instances can't be created/moved.
  Simulation(const Simulation&) = delete;
  Simulation& operator=(const Simulation&) = delete;
  Simulation(Simulation&&) = delete;
  Simulation& operator=(Simulation&&) = delete;
};



PYBIND11_EMBEDDED_MODULE(sbs, m)
{
    // Using the glm module stuff
    py::module_::import("glm");

    
    m.def("get_simulation", &Simulation::instance, py::return_value_policy::reference);
    py::class_<SpaceObject>(m, "SpaceObject")
       // .def(py::init())
		.def_readwrite<SpaceObject, vec3>("pos", &SpaceObject::pos)
        // .def("get_pos", &SpaceObject::get_pos, py::return_value_policy::reference_internal);
        // .def_property("name", &Pet::getName, &Pet::setName)
        .def("move", &SpaceObject::move)
        ;
	py::class_<Simulation>(m, "Simulation")
        //.def_readwrite_static("instance", &Simulation::instance)
        .def("get_space_object", &Simulation::object_exists, py::return_value_policy::reference_internal)
        .def("add_player", [](Simulation& self, std::string ai, std::string data) {return self.AddSpaceObject(ai,data,true,true);})
        .def("add_active", [](Simulation& self, std::string ai, std::string data) {return self.AddSpaceObject(ai,data,false,true);})
        .def("add_passive", [](Simulation& self, std::string ai, std::string data) {return self.AddSpaceObject(ai,data,false,false);})
        .def("delete_object", &Simulation::DeleteSpaceObject)
        ;
}

class MissionScript
{
    std::unique_ptr<py::scoped_interpreter> context;
    py::module script;

public:
    MissionScript()
    {
    }
    void Setup(std::string basedir, std::string missionFolderName, bool remoteDebug, bool pyDebug);
    virtual void Shutdown(void);

    virtual void StartMission(void);
    virtual void TickMission(void);

    bool isRunning = false;
};

int main(int argc, char *argv[])
{
    // get dir of the EXE
    std::string exe(argv[0]);
    std::string basedir = fs::path(fs::absolute(exe)).parent_path().string();
    // Remote debugging: Also helpful to debug both C++ and Python
    // Debugg your C++ and then attach to this python
    bool remote = (argc >= 2 && argv[1][0] == 'r');
    bool pdb = (argc >= 2 && argv[1][0] == 'd');
    MissionScript m;
    m.Setup(basedir, "first", remote, pdb);
    Simulation::instance().clear();
}

void MissionScript::Setup(std::string basedir, std::string missionFolderName, bool remoteDebug, bool pyDebug)
{
    std::ofstream log;
    log.open("log.txt");
    std::wstring wdir = fs::path(basedir).wstring();
    // Path to python
    std::wstring python = wdir + L"\\..\\py_emb";
    // string
    std::string dir = fs::path(basedir).string();
    std::string script = dir + "\\..\\pb_emb\\missions\\" + missionFolderName + "\\script.py";
    // Why is there not a standard sting<>wstring??
    // But one can cheat this way
    std::wstring wMissionDir = fs::path(script).parent_path().wstring();
    //    std::string _python = _dir; // + "\\..\\py\\py_emb";
    log << "Script: " << script;
    // Build PyPath to the embedded zip file
    // Point to python dir and the python zip
    // putting mission dir first
    std::wstring pypath =   wMissionDir + L";" + python + L"\\python39.zip;" + python;
    log << "\nPython Path: " << fs::path(pypath).string();
    ;
    // Sets the class path, maybe add debugger?
    // if (remoteDebug) pypath = pypath+";" + dir + L"..\\debugpy"
    Py_SetPythonHome(python.c_str());
    // Set the ths python root path
    Py_SetPath(pypath.c_str());

    // Let the python begin
    this->context = std::make_unique<py::scoped_interpreter>(py::scoped_interpreter());

    auto glm = py::module::import("glm");
    auto pax = py::module::import("pax");
    auto artemis = py::module::import("sbs");

    /// The debugger looks for the file name in the sys.argv[0]
    auto sys = py::module::import("sys");
    sys.attr("argv") = py::make_tuple("script.py", "embed.cpp");
    // py::print(sys.attr("version"));
    // Don't remember what this is, Default scope?
    py::object scope = py::module::import("__main__").attr("__dict__");

    if (remoteDebug)
    {
        ///////////////////////////////////////////////////////////////////////
        // This sets up the debugger for Vsiual Studio and Visual Studio Code
        // This allows the python script to be debugged
        // This requires PIP install, or in PyPath
        auto debugpy = py::module::import("debugpy");
        py::exec(
            R"(
            debugpy.listen(5678)
            debugpy.wait_for_client()
            )",
            scope);
        // debugpy.listen(5678);
        // debugpy.wait_for_client();
        // debugpy.log_to('path/to/logs')
        py::print("Waiting for debugger to attach...");
    }
    else if (pyDebug)
    {
        //////////////////////////////////////////////////
        // Alternately start pdb python's built in debugger repl
        auto pdb = py::module::import("pdb");
        auto st = pdb.attr("set_trace")();
    }

    printf("Running %s...\n", script.c_str());
    PyStdErrOutStreamRedirect output;

    try
    {
        //py::eval_file(script, scope);
        this->script = py::module::import("script");
        this->StartMission();
        // Run for 10 ticks
        for(int i=0;i < 10;i++)
            this->TickMission();
    }
    catch (py::error_already_set &e)
    {
        // If python has exceptions they land here
        // You can examine the type for handling different things
        py::print(e.value());
    }
    printf("stdout:\n");
    printf(output.stdoutString().c_str());

    printf("stderr:\n");
    printf(output.stderrString().c_str());
}

void MissionScript::Shutdown()
{
}

void MissionScript::StartMission(void)
{
    try {
        py::object result = script.attr("HandleScriptStart");
        result();
    }
    catch (py::error_already_set &e)
    {
        // If python has exceptions they land here
        // You can examine the type for handling different things
        py::print(e.value());
    }

    // ManageTrappedOutput(60, 20);

    isRunning = true;
}

void MissionScript::TickMission(void)
{
    if (!isRunning)
        return;

    try
    {
        py::object result = script.attr("HandleScriptTick");
        result();
        Simulation::instance().recycle();
    }
    catch (py::error_already_set &e)
    {
        // If python has exceptions they land here
        // You can examine the type for handling different things
        py::print(e.value());
    }
}