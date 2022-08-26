
#include "pybind11/pybind11.h"
namespace py = pybind11;
#include <pybind11/stl.h>

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
    // IMPORTANT: Need virtual so pybind knows how to cast
    virtual ~Event() {
        std::cout << "event delete" << std::endl;
    }
};
struct PresentGui : Event {
    PresentGui() :Event(PRESENT_GUI){}
};
struct PresentGuiMessage : Event {
    PresentGuiMessage(const std::string& tag, int client_id, std::variant< int, float, std::string > data) :Event(PRESENT_GUI_MESSAGE), message_tag(tag), client_id(client_id), data(data) {}
    std::string message_tag;
    int client_id;
    std::variant< int, float, std::string > data;
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

}

//PYBIND11_MAKE_OPAQUE(std::vector<SbsEvent::Event*>);
//PYBIND11_MAKE_OPAQUE(std::vector<std::shared_ptr <SbsEvent::Event>>);

// ...

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
        .def_readonly("data", &SbsEvent::PresentGuiMessage::data)
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
//    py::bind_vector<std::vector<SbsEvent::Event*>>(m, "VectorEvent");
    
}
