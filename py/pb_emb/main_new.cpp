
#include <pybind11/iostream.h>
#include <pybind11/embed.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "crc.h"
#include <pybind11/stl.h>

//#include <codecvt>
#include <string>
#include <sstream>
#include <filesystem>
namespace fs = std::filesystem;

#include "py_io.h"
#include "event.h"

#include <string>
#include <regex>
#include <chrono>
using namespace std::chrono;

PYBIND11_EMBEDDED_MODULE(pax, m)
{
    // `m` is a `py::module` which is used to bind functions and classes
}

namespace glm
{
    struct vec2
    {
        double x;
        double y;
    };
    struct vec3
    {
        vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
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
        .def_readwrite("y", &glm::vec2::y);

    py::class_<vec3>(m, "vec3")
        .def(py::init<double, double, double>())
        .def(py::init<const vec3 &>())
        .def(py::init<>())
        .def_readwrite("x", &vec3::x)
        .def_readwrite("y", &vec3::y)
        .def_readwrite("z", &vec3::z);
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
struct SpaceObject
{
    vec3 pos;
    SpaceObject()
    {
        pos.x = 1.0;
        pos.y = 2.0;
        pos.z = 3.0;
    }
    void move(vec3 v)
    {
        this->pos.x += v.x;
        this->pos.y += v.y;
        this->pos.z += v.z;
    }
};

struct ObjectHandle
{
    int32_t handle;
};


struct Simulation
{
    std::unordered_map<uint32_t, SpaceObject *> objects;
    std::unordered_set<uint32_t> bin;
//    std::vector<std::shared_ptr<SbsEvent::Event>> events;
    std::vector<std::unique_ptr<SbsEvent::Event>> uevents;
    std::vector<std::unique_ptr<SbsEvent::Event, py::nodelete>> u2events;
    std::vector<SbsEvent::Event*> pevents;
    //py::list events;

    /*
        This seems to be the one that works, 
        transfer ownership of the vector 
    */
    std::vector<std::unique_ptr<SbsEvent::Event>> get_events(){
        std::vector<std::unique_ptr<SbsEvent::Event>> e = std::move(uevents);
        return e;
    }
 
    std::vector<SbsEvent::Event*>& get_events_ref(){
        return pevents;
    }


    ~Simulation()
    {
        std::cout << "WTF";
        // clear();
    }
   

    SpaceObject *object_exists(uint32_t handle)
    {
        auto got = objects.find(handle);
        if (got != objects.end())
        {
            return got->second;
        }
        return nullptr;
    }



    uint32_t AddSpaceObject(std::string aiTag, std::string dataTag, bool isPlayer, bool isActive)
    {
        auto ID = crc(dataTag.begin(), dataTag.end());
        SpaceObject *object = new SpaceObject();
        objects.insert(std::make_pair(ID, object));
        return ID;
    }
    void DeleteSpaceObject(uint32_t ID)
    {
        bin.insert(ID);
    }

    // take out trash
    void recycle()
    {
        for (const auto ID : bin)
        {
            auto got = objects.find(ID);
            if (got != objects.end())
            {
                delete got->second;
            }
        }
        bin.clear();
    }

    void clear()
    {
        for (const auto &[key, value] : objects)
        {
            delete value;
        }
        objects.clear();
    }

    static Simulation &instance()
    {
        static Simulation *simulation = new Simulation();
        return *simulation;
    }

    Simulation() = default;

private:
    // Delete copy/move so extra instances can't be created/moved.
    Simulation(const Simulation &) = delete;
    Simulation &operator=(const Simulation &) = delete;
    Simulation(Simulation &&) = delete;
    Simulation &operator=(Simulation &&) = delete;
};

//////////////////////////////////////////////////
// A purely C++ object for capturing output stream
// to a stringstream
class Logger
{
    std::stringstream ss;

public:
    void write(std::string str)
    {
        ss << str;
    }
    void flush()
    {
        ss.flush();
    }
    std::string get()
    {
        std::string ret = ss.str();
        std::stringstream().swap(ss);
        return ret;
    }
};

PYBIND11_EMBEDDED_MODULE(sbs, m)
{
    // Using the glm module stuff
    py::module_::import("glm");
    py::module_::import("sbsevent");


    m.def("get_simulation", &Simulation::instance, py::return_value_policy::reference);
    py::class_<SpaceObject>(m, "SpaceObject")
        // .def(py::init())
        .def_readwrite<SpaceObject, vec3>("pos", &SpaceObject::pos)
        // .def("get_pos", &SpaceObject::get_pos, py::return_value_policy::reference_internal);
        // .def_property("name", &Pet::getName, &Pet::setName)
        .def("move", &SpaceObject::move);
    py::class_<Simulation>(m, "Simulation")
        //.def_readwrite_static("instance", &Simulation::instance)
        .def("get_space_object", &Simulation::object_exists, py::return_value_policy::reference_internal)
        .def("add_player", [](Simulation &self, std::string ai, std::string data)
             { return self.AddSpaceObject(ai, data, true, true); })
        .def("add_active", [](Simulation &self, std::string ai, std::string data)
             { return self.AddSpaceObject(ai, data, false, true); })
        .def("add_passive", [](Simulation &self, std::string ai, std::string data)
             { return self.AddSpaceObject(ai, data, false, false); })
        .def("delete_object", &Simulation::DeleteSpaceObject)
        .def("get_events_move", &Simulation::get_events, py::return_value_policy::reference_internal)
        .def("get_events_ref", &Simulation::get_events_ref, py::return_value_policy::reference_internal)

        ;
    py::class_<Logger>(m, "Writer")
        .def("write", &Logger::write)
        .def("flush", &Logger::flush);
}

void time_face(const std::string &layers)
{
    // Regular expression for a layer
    const std::regex layers_regex(R"((\w*)\s+(#[0-9A-Fa-f]*|\w*)\s*([+|-]?\d*)?\s*([+|-]?\d*)?\s*([+|-]?\d*)?\s*([+|-]?\d*)?;?)");
    // Iterator to get each match
    const std::sregex_iterator end;
    std::sregex_iterator it(layers.cbegin(), layers.cend(), layers_regex);
    for (; it != end; it++)
    {

        auto t = it->str(1);
        auto c = it->str(2);
        int oX, oY, cX, cY;

        // Look for cell and offsets, using stoi to show how to convert to int
        if (it->length(3))
            cX = std::stoi(it->str(3));
        if (it->length(4))
            cY = std::stoi(it->str(4));
        if (it->length(5))
            oX = std::stoi(it->str(5));
        if (it->length(6))
            oY = std::stoi(it->str(6));
    }
}

void parse_face(const std::string &layers)
{
    // Regular expression for a layer
    static const std::regex layers_regex("(\\w*)\\s+(#[0-9A-Fa-f]*|\\w*)\\s*([+|-]?\\d*)?\\s*([+|-]?\\d*)?\\s*([+|-]?\\d*)?\\s*([+|-]?\\d*)?;?");
    std::smatch layers_match;

    std::smatch m;

    // Iterator to get each match
    const std::sregex_iterator end;
    std::sregex_iterator it(layers.cbegin(), layers.cend(), layers_regex);
    for (; it != end; it++)
    {
        std::cout << "Matched: " << it->str(0) << std::endl;
        std::cout << "\tTexture: " << it->str(1) << std::endl;
        std::cout << "\tColor: " << it->str(2) << std::endl;
        // Look for cell and offsets, using stoi to show how to convert to int
        if (it->length(3))
            std::cout << "\tcellX: " << std::stoi(it->str(3)) << std::endl;
        if (it->length(4))
            std::cout << "\tcellY: " << std::stoi(it->str(4)) << std::endl;
        if (it->length(5))
            std::cout << "\toffsetX: " << std::stoi(it->str(5)) << std::endl;
        if (it->length(6))
            std::cout << "\toffsetY: " << std::stoi(it->str(6)) << std::endl;
    }
    std::cout << std::endl;
}

void time_regex()
{

    // Use auto keyword to avoid typing long
    // type definitions to get the timepoint
    // at this instant use function now()
    auto start = high_resolution_clock::now();
    for (int i = 0; i < 100; i++)
        time_face("ter white 0 0;ter #fff 5 2;ter #fff 6 3 -3 2;ter white 0 0;ter #fff 5 2;ter white 0 0;ter #fff 5 7;");
    auto stop = high_resolution_clock::now();
    // Check forgeting final semi
    // parse_face("ter white 0 0;ter #f0f 5 2;ter #Fff 6 3 -3 2;ter white 0 0;ter #fff 5 2;ter white 0 0;ter #fff 5 7");
    auto duration = duration_cast<milliseconds>(stop - start);

    std::cout << duration.count() << std::endl;
}

class MissionScript
{
    Logger log_stdout;
    Logger log_stderr;
    std::ofstream log;
    PyThreadState *_save;
    py::object script;

public:
    MissionScript()
    {
        _save = NULL;
    }
    void ReadyCPP()
    {
        this->_save = PyEval_SaveThread();
        //_save = PyEval_SaveThread();
    }
    void ReadyPython()
    {
        PyEval_RestoreThread(this->_save);
    }
    void Setup(std::string basedir, std::string missionFolderName, bool remoteDebug, bool pyDebug);
    void Run(std::string basedir, std::string missionFolderName);
    // void Setup(std::string basedir, std::string missionFolderName, bool remoteDebug, bool pyDebug);
    virtual void Shutdown(void);

    virtual void StartMission(void);
    virtual void DoEvents(void);
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
    for (int i = 0; i < 1; i++)
    {
        if ((i % 2) == 0)
        {
            m.Setup(basedir, "first", remote, pdb);
            m.Run(basedir, "first");
            m.Shutdown();
        }
        else
        {
            m.Setup(basedir, "second", remote, pdb);
            m.Run(basedir, "second");
            m.Shutdown();
        }
    }
    // m.Setup(basedir, "im", remote, pdb);
    // m.Run(basedir,"im");
    // m.Shutdown();
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
    std::wstring pypath = wMissionDir + L";" + python + L"\\python39.zip;" + python;
    this->log << "\nPython Path: " << fs::path(pypath).string();
    this->log.flush();
    ;
    // Sets the class path, maybe add debugger?
    // if (remoteDebug) pypath = pypath+";" + dir + L"..\\debugpy"
    Py_SetPythonHome(python.c_str());
    // Set the ths python root path
    Py_SetPath(pypath.c_str());

    py::initialize_interpreter();
    pybind11::detail::get_internals();

    // Hook stdout/stderr
    py::module::import("sbs");
    py::module::import("sbsevent");

    py::module::import("sys").attr("stdout") = &log_stdout; // Must be pointer so it is this one
    py::module::import("sys").attr("stderr") = &log_stderr; // Must be pointer so it is this one
    // load script
    this->script = py::module::import("script");
    this->ReadyCPP();
    printf("Running %s...\n", missionFolderName.c_str());

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

void MissionScript::Run(std::string basedir, std::string missionFolderName)
{

    try
    {

        this->StartMission();
        this->DoEvents();
        // Run for 10 ticks
        for (int i = 0; i < 10; i++)
        {
            this->TickMission();
            // Simulate C++ Blocking
            _sleep(1000);
        }
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

    this->ReadyPython();
    // printf("Finished %s...\n", missionFolderName.c_str());
    printf("Finished script...\n");
    printf("stdout:\n");
    // printf(output->stdoutString().c_str());
    printf("stderr:\n");
    // printf(output->stderrString().c_str());
    this->script.release();
    // delete this->output;
    // this->output = NULL;

    printf("preFinalEx:\n");
    py::finalize_interpreter();
    printf("finalize_:\n");
    this->_save = NULL;
}

void MissionScript::StartMission(void)
{
    try
    {
        this->ReadyPython();
        py::object result = script.attr("HandleScriptStart");
        py::object sim = py::cast(&Simulation::instance(), py::return_value_policy::reference);
        result(sim);
        Simulation::instance().recycle();
    }
    catch (py::error_already_set &e)
    {
        // If python has exceptions they land here
        // You can examine the type for handling different things
        py::print(e.value());
    }
    this->ReadyCPP();
    
    std::cout << log_stdout.get();
    std::cerr << log_stderr.get();

    isRunning = true;
}
struct Test {
    int type;
};

void MissionScript::DoEvents(void)
{
    try
    {
        this->ReadyPython();
       
        Simulation::instance().uevents.push_back(std::make_unique<SbsEvent::PresentGui>());
        Simulation::instance().uevents.push_back(std::make_unique<SbsEvent::PresentGuiMessage>("HelloInt", 12, 2));
        Simulation::instance().uevents.push_back(std::make_unique<SbsEvent::PresentGuiMessage>("HelloFloat", 12, 2.0f));
        Simulation::instance().uevents.push_back(std::make_unique<SbsEvent::PresentGuiMessage>("HelloStr", 12, "Two"));

        Simulation::instance().pevents.push_back(new SbsEvent::PresentGuiMessage("ValInt", 42,2));
        Simulation::instance().pevents.push_back(new SbsEvent::PresentGuiMessage("ValFloat", 42,3.14f));
        Simulation::instance().pevents.push_back(new SbsEvent::PresentGuiMessage("ValInt", 42, "Yo gabagaba"));

        Simulation::instance().pevents.push_back(new SbsEvent::PresentGui());

        //Simulation::instance().events.push_back(std::make_unique<SbsEvent::PresentGuiMessage>("butthead", 42));
        //Simulation::instance().events.push_back(std::make_unique<SbsEvent::PresentGui>());
        //Simulation::instance().events.push_back(std::make_shared<SbsEvent::PresentGuiMessage>("butthead", 42));


        py::object result = script.attr("HandleEvents");
        py::object sim = py::cast(&Simulation::instance(), py::return_value_policy::reference);
        
        if (true) {
            // Don't need the function to get the reference
            // if you cast properly as a reference
            py::object ev = py::cast(&Simulation::instance().pevents, py::return_value_policy::reference);
            result(sim, ev);
        }
        else {
            result(sim, Simulation::instance().get_events_ref());
        }

        

        Simulation::instance().recycle();
    }
    catch (py::error_already_set &e)
    {
        // If python has exceptions they land here
        // You can examine the type for handling different things
        py::print(e.value());
    }
    this->ReadyCPP();
    
    std::cout << log_stdout.get();
    std::cerr << log_stderr.get();

}


void MissionScript::TickMission(void)
{
    if (!isRunning)
        return;

    try
    {
        this->ReadyPython();
        py::object result = script.attr("HandleScriptTick");
        py::object sim = py::cast(&Simulation::instance(), py::return_value_policy::reference);
        result(sim);
        Simulation::instance().recycle();
    }
    catch (py::error_already_set &e)
    {
        // If python has exceptions they land here
        // You can examine the type for handling different things
        py::print(e.value());
    }
    this->ReadyCPP();
    std::cout << log_stdout.get();
    std::cerr << log_stderr.get();
}

