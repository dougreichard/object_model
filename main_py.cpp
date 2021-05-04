#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "shape.h"

// 
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
    auto got = Symbol::symbols.find(attr);
    if ( got == Symbol::symbols.end() ) {
        Py_RETURN_NONE;
    }  else
    {
        Symbol* s = got->second;
        PyCircle* c = (PyCircle*)self;
        Value& v = c->circle->get(*s);
        if (v._type._key == obj::INT_SYMBOL) {
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
    auto got = Symbol::symbols.find(attr);
    if ( got != Symbol::symbols.end() ) {
        Symbol* s = got->second;
        PyCircle* c = (PyCircle*)self;
        Value& v = c->circle->get(*s);
        if (v._type._key == obj::INT_SYMBOL) {
            Int& i = (Int&)v;
            i = PyLong_AsLong(value);
            return 0;
        }
    }
    return -1;
}


static PyMethodDef PyCircle_methods[] = {
    // {"Func", (PyCFunction) py_func_wrapper, METH_NOARGS,
    //  "Description"
    // },
    {NULL}  /* Sentinel */
};


static PyTypeObject PyCircleType = {
    PyVarObject_HEAD_INIT(NULL, 0)
};


static PyModuleDef py_shape_module = {
    PyModuleDef_HEAD_INIT,
};


extern "C" PyMODINIT_FUNC  PyInit_shapes(void)
{
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

    // Finish initializing shape module
    py_shape_module.m_name = "shapes";
    py_shape_module.m_doc = "Example module that creates an extension type.";
    py_shape_module.m_size = -1;

    //cout << "Got here";
    PyObject *m;
    if (PyType_Ready(&PyCircleType) < 0)
        return NULL;


    m = PyModule_Create(&py_shape_module);
    if (m == NULL)
        return NULL;

    Py_INCREF(&PyCircleType);
    if (PyModule_AddObject(m, "Circle", (PyObject *) &PyCircleType) < 0) {
        Py_DECREF(&PyCircleType);
        Py_DECREF(m);
        return NULL;
    }
  
    return m;
}

