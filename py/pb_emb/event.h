
#include "pybind11/pybind11.h"
namespace py = pybind11;
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/embed.h>




#include <pybind11/stl.h>
#include <pybind11/pybind11.h>
#include <iostream>
#include <memory>
#include <vector>

class MyBase {
    int j;
public:
    MyBase() { j = 2;}
    virtual void print() const {
        std::cout << "MyBase::print()" << std::endl;
    }
};

class MyDerived : public MyBase {
    float d;
public:
    MyDerived() : MyBase() {}
    virtual void print() const override {
        std::cout << "MyDerived::print()" << std::endl;
    }
};

std::vector<std::unique_ptr<MyBase>> mylist() {
    std::vector<std::unique_ptr<MyBase>> v;
    v.push_back(std::make_unique<MyBase>());
    v.push_back(std::make_unique<MyDerived>());
    return v;
}

PYBIND11_EMBEDDED_MODULE(example, m) {
    pybind11::class_<MyBase>(m, "MyBase")
        .def(pybind11::init<>())
        .def("print", &MyBase::print)
        .def("__repr__", [](MyBase const&) { return "MyBase"; });

    pybind11::class_<MyDerived>(m, "MyDerived")
        .def(pybind11::init<>())
        .def("print", &MyDerived::print)
        .def("__repr__", [](MyDerived const&) { return "MyDerived"; });

    m.def("mylist", &mylist, "returns a list");
}



namespace SbsEvent {


enum Type {
    UNKNOWN=0,
    PRESENT_GUI,
    PRESENT_GUI_MESSAGE,
    CLIENT_CONNECT,
    SIMULATION_TICK,
    DAMAGE,
    CONSOLE_OBJECT_SELECTION,
    COMMS_BUTTON,
    // Place new events above
    LAST
};


struct Event {
    Event(const Type& type) : type(type) {}
    Type type;
    virtual ~Event() {
        std::cout << "event delete" << std::endl;
    }
};
struct PresentGui : Event {
    PresentGui() :Event(PRESENT_GUI){}
};
struct PresentGuiMessage : Event {
    PresentGuiMessage(const std::string& tag, int client_id) :Event(PRESENT_GUI_MESSAGE), message_tag(tag), client_id(client_id) {}
    std::string message_tag;
    int client_id;
};
struct ClientConnect : Event {
    ClientConnect() :Event(CLIENT_CONNECT){}
    int client_id;
};
struct SimulationTick : Event {
    SimulationTick() :Event(SIMULATION_TICK){}
};
struct Damage : Event {
    Damage() :Event(CONSOLE_OBJECT_SELECTION){}
};
struct ConsoleObjectSelection : Event {
    ConsoleObjectSelection() :Event(CONSOLE_OBJECT_SELECTION){}
    std::string console;
    int id1;
    int id2;
};
struct CommsButton :  Event {
    CommsButton() :Event(COMMS_BUTTON){}
    std::string button_tag;
    int id1;
    int id2;
};


/*
class Event {
public:
    Event(const Type& type) : type(type) {}
    Type type;
    virtual void some() {}
};
class PresentGui : public Event {
public:
    PresentGui() :Event(PRESENT_GUI){}
    virtual void some() {}
};
class PresentGuiMessage : public Event {
public:
    PresentGuiMessage(const std::string& tag, int client_id) :Event(PRESENT_GUI_MESSAGE), message_tag(tag), client_id(client_id) {}
    std::string message_tag;
    int client_id;
    // Values?
    virtual void some() {}
};
class ClientConnect : public Event {
public:
    ClientConnect() :Event(CLIENT_CONNECT){}
    int client_id;
    virtual void some() {}
};
class SimulationTick : public Event {
public:
    SimulationTick() :Event(SIMULATION_TICK){}
    virtual void some() {}
};
class Damage : public Event {
public:    
    Damage() :Event(CONSOLE_OBJECT_SELECTION){}
    virtual void some() {}
};
class ConsoleObjectSelection : public Event {
public:
    ConsoleObjectSelection() :Event(CONSOLE_OBJECT_SELECTION){}
    std::string console;
    int id1;
    int id2;
    virtual void some() {}
};
class CommsButton : public Event {
public:
    CommsButton() :Event(COMMS_BUTTON){}
    std::string button_tag;
    int id1;
    int id2;
    virtual void some() {}
};
*/
}

//PYBIND11_MAKE_OPAQUE(std::vector<SbsEvent::Event*>);
PYBIND11_MAKE_OPAQUE(std::vector<std::shared_ptr <SbsEvent::Event>>);

// ...

// later in binding code:



PYBIND11_EMBEDDED_MODULE(sbsevent, m)
{
     py::enum_<SbsEvent::Type>(m, "EventType")
    .value("PRESENT_GUI",SbsEvent::PRESENT_GUI)
    .value("PRESENT_GUI_MESSAGE",SbsEvent::PRESENT_GUI_MESSAGE)
    .value("CLIENT_CONNECT",SbsEvent::CLIENT_CONNECT)
    .value("SIMULATION_TICK",SbsEvent::SIMULATION_TICK)
    .value("DAMAGE",SbsEvent::DAMAGE)
    .value("CONSOLE_OBJECT_SELECTION",SbsEvent::CONSOLE_OBJECT_SELECTION)
    .value("COMMS_BUTTON",SbsEvent::COMMS_BUTTON)
    ;

    py::class_<SbsEvent::Event>(m, "Event")
        //.def(py::init<const SbsEvent::Type&>())
        .def_readonly("type", &SbsEvent::Event::type)
        ;

    py::class_<SbsEvent::PresentGui, SbsEvent::Event>(m, "PresentGui")
        //.def(py::init<>())
        ;

    py::class_<SbsEvent::PresentGuiMessage, SbsEvent::Event>(m, "PresentGuiMessage")
        //.def(py::init<>())
        .def_readonly("message_tag", &SbsEvent::PresentGuiMessage::message_tag)
        .def_readonly("client_id", &SbsEvent::PresentGuiMessage::client_id)
        ;

    py::class_<SbsEvent::ClientConnect, SbsEvent::Event>(m, "ClientConnect")
        //.def(py::init<>())
        .def_readonly("client_id", &SbsEvent::ClientConnect::client_id)
        ;

    py::class_<SbsEvent::SimulationTick, SbsEvent::Event>(m, "SimulationTick")
        //.def(py::init<>())
        ;

    py::class_<SbsEvent::Damage, SbsEvent::Event>(m, "Damage")
        //.def(py::init<>())
        ;

    py::class_<SbsEvent::ConsoleObjectSelection, SbsEvent::Event>(m, "ConsoleObjectSelection")
        //.def(py::init<>())
        .def_readonly("id1", &SbsEvent::ConsoleObjectSelection::id1)
        .def_readonly("id2", &SbsEvent::ConsoleObjectSelection::id2)
        ;

    py::class_<SbsEvent::CommsButton, SbsEvent::Event>(m, "CommsButton")
        //.def(py::init<>())
        .def_readonly("button_tag", &SbsEvent::CommsButton::button_tag)
        .def_readonly("id1", &SbsEvent::CommsButton::id1)
        .def_readonly("id2", &SbsEvent::CommsButton::id2)
        ;

   // py::bind_vector<std::vector<std::shared_ptr <SbsEvent::Event>>>(m, "VectorEvent2");
    //py::bind_vector<std::vector<SbsEvent::Event*>>(m, "VectorEvent");
    
}
