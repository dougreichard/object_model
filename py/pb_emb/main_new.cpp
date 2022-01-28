
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
        .def(py::init<const vec3& >())
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
    std::ofstream log;
    py::object script;

public:
    MissionScript()
    {
    }
    void Setup(std::string basedir, std::string missionFolderName, bool remoteDebug, bool pyDebug);
    void Run(std::string basedir, std::string missionFolderName);
    //void Setup(std::string basedir, std::string missionFolderName, bool remoteDebug, bool pyDebug);
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
    ///////////////////////////////
    // Run two different scripts multiple times
    for(int i =0; i<5;i++) {
        if ((i%2) == 1) {
            m.Setup(basedir, "first", remote, pdb);
            m.Run(basedir,"first");
            m.Shutdown();
        }
        else { 
            m.Setup(basedir, "second", remote, pdb);
            m.Run(basedir,"second");
            m.Shutdown();
        }
    }
    printf("DONE - Running Scripts");
    
    Simulation::instance().clear();
}

void MissionScript::Setup(std::string basedir, std::string missionFolderName, bool remoteDebug, bool pyDebug)
{
    
    this->log.open("log.txt");

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
    this->log << "Script: " << script;
    // Build PyPath to the embedded zip file
    // Point to python dir and the python zip
    // putting mission dir first
    std::wstring pypath =   wMissionDir + L";" + python + L"\\python39.zip;" + python;
    this->log << "\nPython Path: " << fs::path(pypath).string();
    ;
    // Sets the class path, maybe add debugger?
    // if (remoteDebug) pypath = pypath+";" + dir + L"..\\debugpy"
    Py_SetPythonHome(python.c_str());
    // Set the ths python root path
    Py_SetPath(pypath.c_str());

    py::initialize_interpreter();
    pybind11::detail::get_internals();

    // /// The debugger looks for the file name in the sys.argv[0]
    // auto sys = py::module::import("sys");
    // sys.attr("argv") = py::make_tuple("script.py", "embed.cpp");
    // // py::print(sys.attr("version"));
    // // Don't remember what this is, Default scope?
    

    // if (remoteDebug)
    // {
    //     ///////////////////////////////////////////////////////////////////////
    //     // This sets up the debugger for Vsiual Studio and Visual Studio Code
    //     // This allows the python script to be debugged
    //     // This requires PIP install, or in PyPath
    //     auto debugpy = py::module::import("debugpy");
    //     py::exec(
    //         R"(
    //         debugpy.listen(5678)
    //         debugpy.wait_for_client()
    //         )",
    //         scope);
    //     // debugpy.listen(5678);
    //     // debugpy.wait_for_client();
    //     // debugpy.log_to('path/to/logs')
    //     py::print("Waiting for debugger to attach...");
    // }
    // else if (pyDebug)
    // {
    //     //////////////////////////////////////////////////
    //     // Alternately start pdb python's built in debugger repl
    //     auto pdb = py::module::import("pdb");
    //     auto st = pdb.attr("set_trace")();
    // }
 
}

// void MissionScript::Run(std::string basedir, std::string missionFolderName){
 
//     printf("Running %s...\n", missionFolderName.c_str());
  
//     PyThreadState * main = PyThreadState_Get() ;
//     PyThreadState * scriptInter = Py_NewInterpreter() ;  //     
    
//     try
//     {
//         PyThreadState_Swap(scriptInter);
//         this->script = py::module::import("script");
//         PyStdErrOutStreamRedirect output;
        
//         PyThreadState_Swap(main);

//         PyThreadState_Swap(scriptInter);
//         this->StartMission();
//         PyThreadState_Swap(main);

        
//         // // Run for 10 ticks
//         for(int i=0;i < 10;i++) {
//           // PyEval_RestoreThread(_save);
//            PyThreadState_Swap(scriptInter);
//             this->TickMission();
//            PyThreadState_Swap(main);
//             // Fake artimeis sim running
//             _sleep(1000);

//         }
//         PyThreadState_Swap(scriptInter);

//         printf("Finished %s...\n", missionFolderName.c_str());
//         printf("stdout:\n");
//         printf(output.stdoutString().c_str());

//         printf("stderr:\n");
//         printf(output.stderrString().c_str());

//         this->script.release();
//         output.exit();
        
//         // printf("acq:\n");
//         printf("endED:\n");
//         // This will close all daemon threads        
//         //output.exit();
//         Py_FinalizeEx();
        
        
//         //Py_EndInterpreter(scriptInter);
//         PyThreadState_Swap(main);

        

//         printf("Done:\n");
//     }
//     catch (py::error_already_set &e)
//     {
//         // If python has exceptions they land here
//         // You can examine the type for handling different things
//         py::print(e.value());
//     }
//     printf("Released:\n");
// }



void MissionScript::Run(std::string basedir, std::string missionFolderName){
 
    printf("Running %s...\n", missionFolderName.c_str());
    PyStdErrOutStreamRedirect output;
    try
    {
        this->script = py::module::import("script");
        PyThreadState *_save; 

        // Save when you are going to block in C++
        _save = PyEval_SaveThread();

        // And sometime later
        // Restore to call Python
        PyEval_RestoreThread(_save);
        this->StartMission();
        // Save again to allow C++ to block
        _save = PyEval_SaveThread();
        

        // Run for 10 ticks
        for(int i=0;i < 10;i++) {
            // Restore to call Python
           PyEval_RestoreThread(_save);
            this->TickMission();
            // Save when you are going to block in C++
           _save = PyEval_SaveThread();

           // Simulate C++ Blocking
            _sleep(1000);
        }

        // Restore to cleanup 
        PyEval_RestoreThread(_save);

        printf("Finished %s...\n", missionFolderName.c_str());
        printf("stdout:\n");
        printf(output.stdoutString().c_str());
        printf("stderr:\n");
        printf(output.stderrString().c_str());
        this->script.release();
    }
    catch (py::error_already_set &e)
    {
        // If python has exceptions they land here
        // You can examine the type for handling different things
        py::print(e.value());
    }

}

void MissionScript::Shutdown()
{
    // Release memory of script module
    //this->script.release();
    // SubInterpreter
    //Py_EndInterpreter(script_inter);
    printf("preFinalEx:\n");
    py::finalize_interpreter();
    printf("finalize_:\n");
}

void MissionScript::StartMission(void)
{
    try {
        py::object result = script.attr("HandleScriptStart");
        py::object sim = py::cast(&Simulation::instance(),py::return_value_policy::reference);
        result(sim);
        //Not sure why they'd delete but...
        Simulation::instance().recycle();
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
    //        py::object sim = py::cast(&Simulation::instance());
    //  result(&Simulation::instance());
        py::object sim = py::cast(&Simulation::instance(),py::return_value_policy::reference);
        result(sim);
        
        Simulation::instance().recycle();
    }
    catch (py::error_already_set &e)
    {
        // If python has exceptions they land here
        // You can examine the type for handling different things
        py::print(e.value());
    }
}