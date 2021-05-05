#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "object-model.h"
#include "py_object.h"

using namespace obj;

typedef struct {
    PyObject_HEAD
    /* Type-specific fields go here. */
    Object* object;
} PyDynObject;


static int
py_dyn_object_init(PyDynObject *self, PyObject *args, PyObject *kwds)
{
    // Python should probably NOT allocate objects, but for now
    Object* object = new Object();
    // Check?
    self->object = object;
    return 0;
}

static void 
py_dyn_object_dealloc(PyDynObject *self) {
    PyTypeObject *tp = Py_TYPE(self);
    // free references and buffers here
    delete (self->object);

    tp->tp_free(self);
    Py_DECREF(tp);
}

PyObject *py_dyn_object_get_attro(PyObject *self, PyObject *attrName) {
    const char *attr = PyUnicode_AsUTF8(attrName);
    // This should look up the symbol for attr
    auto got = Symbol::symbols.find(attr);
    if ( got == Symbol::symbols.end() ) {
        Py_RETURN_NONE;
    }  else
    {
        Symbol* s = got->second;
        PyDynObject* c = (PyDynObject*)self;
        Value& v = c->object->get(*s);
        if (v._type == obj::INT_SYMBOL) {
           return PyLong_FromLong((Int&)v);
        } else {
             return PyLong_FromLong(9999);
        }
    }
    Py_RETURN_NONE;
}

int py_dyn_object_set_attro(PyObject *self, PyObject *attrName, PyObject* value) {
    const char *attr = PyUnicode_AsUTF8(attrName);
    
    auto got = Symbol::symbols.find(attr);
    if ( got != Symbol::symbols.end() ) {
        Symbol* s = got->second;
        PyDynObject* obj = (PyDynObject*)self;
        //////////////////
        //
        if (PyLong_Check(value)) {
            Int i = PyLong_AsLong(value);
            obj->object->set(*s,i);
        }
        return 0;
    }
    return -1;
}


extern PyMethodDef PyDynObject_methods[] = {
    // {"Func", (PyCFunction) py_func_wrapper, METH_NOARGS,
    //  "Description"
    // },
    {NULL}  /* Sentinel */
};

//
extern PyTypeObject PyDynObjectType = {
    PyVarObject_HEAD_INIT(NULL, 0)
};

extern PyTypeObject* get_dyn_object_type() {
// Finish initializing Gateway object
    PyDynObjectType.tp_name = "shapes.Object";
    PyDynObjectType.tp_basicsize = sizeof(PyDynObject);
    PyDynObjectType.tp_itemsize = 0;
    PyDynObjectType.tp_doc = "An Object ";
    PyDynObjectType.tp_flags = Py_TPFLAGS_DEFAULT;
    PyDynObjectType.tp_new = PyType_GenericNew;
    PyDynObjectType.tp_methods = PyDynObject_methods;
    PyDynObjectType.tp_init = (initproc) py_dyn_object_init;
    PyDynObjectType.tp_dealloc = (destructor) py_dyn_object_dealloc;
    PyDynObjectType.tp_getattro = (getattrofunc) py_dyn_object_get_attro;
    PyDynObjectType.tp_setattro = (setattrofunc) py_dyn_object_set_attro;
    return &PyDynObjectType;
}