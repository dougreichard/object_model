#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "shape.h"
#include "py_circle.h"

typedef struct {
    PyObject_HEAD
    /* Type-specific fields go here. */
    Circle* circle;
} PyCircle;


static int
py_circle_init(PyCircle *self, PyObject *args, PyObject *kwds)
{
    // Python should probably NOT allocate objects, but for now
    Circle* circle = new Circle(0,0,1);
    // Check?
    self->circle = circle;
    return 0;
}

static void 
py_circle_dealloc(PyCircle *self) {
    PyTypeObject *tp = Py_TYPE(self);
    // free references and buffers here
    delete (self->circle);

    tp->tp_free(self);
    Py_DECREF(tp);
}

PyObject *py_circle_get_attro(PyObject *self, PyObject *attrName) {
    const char *attr = PyUnicode_AsUTF8(attrName);
    // This should look up the symbol for attr
    Symbol& got = Scope<Symbol>::instance().get(attr);
    if ( got == Symbol::get_undefined() ) {
        Py_RETURN_NONE;
    }  else
    {
        PyCircle* c = (PyCircle*)self;
        Value& v = c->circle->get(got);
         
        if (v._type == obj::INT_SYMBOL) {
           return PyLong_FromLong((Int&)v);
        }
    }
    Py_RETURN_NONE;
}

int py_circle_set_attro(PyObject *self, PyObject *attrName, PyObject* value) {
    const char *attr = PyUnicode_AsUTF8(attrName);
    //  if (!PyArg_ParseTuple(argList, "s", &attr))
    //     return NULL;    
    // This should look up the symbol for attr
    auto got = Scope<Symbol>::instance().get(attr);
    if ( got != Symbol::get_undefined() ) {
        PyCircle* c = (PyCircle*)self;
        Value& v = c->circle->get(got);
        if (v._type._key == obj::INT_SYMBOL) {
            Int& i = (Int&)v;
            i = PyLong_AsLong(value);
            return 0;
        }
    }
    return -1;
}


extern PyMethodDef PyCircle_methods[] = {
    // {"Func", (PyCFunction) py_func_wrapper, METH_NOARGS,
    //  "Description"
    // },
    {NULL}  /* Sentinel */
};

//
extern PyTypeObject PyCircleType = {
    PyVarObject_HEAD_INIT(NULL, 0)
};

extern PyTypeObject* get_circle_type() {
// Finish initializing Gateway object
    PyCircleType.tp_name = "shapes.Circle";
    PyCircleType.tp_basicsize = sizeof(PyCircle);
    PyCircleType.tp_itemsize = 0;
    PyCircleType.tp_doc = "A circle ";
    PyCircleType.tp_flags = Py_TPFLAGS_DEFAULT;
    PyCircleType.tp_new = PyType_GenericNew;
    PyCircleType.tp_methods = PyCircle_methods;
    PyCircleType.tp_init = (initproc) py_circle_init;
    PyCircleType.tp_dealloc = (destructor) py_circle_dealloc;
    PyCircleType.tp_getattro = (getattrofunc) py_circle_get_attro;
    PyCircleType.tp_setattro = (setattrofunc) py_circle_set_attro;
    return &PyCircleType;
}