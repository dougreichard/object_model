#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "shape.h"
#include "py_circle.h"
#include "py_object.h"
#include "py_symbol.h"

static PyModuleDef py_shape_module = {
    PyModuleDef_HEAD_INIT,
};

extern "C" PyMODINIT_FUNC  PyInit_shapes(void)
{
    // Finish initializing shape module
    py_shape_module.m_name = "shapes";
    py_shape_module.m_doc = "Example module that creates an extension type.";
    py_shape_module.m_size = -1;

    //cout << "Got here";
    PyObject *m;
    PyTypeObject* PyCircleType = get_circle_type(); 
    if (PyType_Ready(PyCircleType) < 0)
        return NULL;

    PyTypeObject* PyDynObjectType = get_dyn_object_type(); 
    if (PyType_Ready(PyDynObjectType) < 0)
        return NULL;

    PyTypeObject* PySymbolType = get_symbol_type(); 
    if (PyType_Ready(PySymbolType) < 0)
        return NULL;


    m = PyModule_Create(&py_shape_module);
    if (m == NULL)
        return NULL;

    Py_INCREF(PyCircleType);
    if (PyModule_AddObject(m, "Circle", (PyObject *) PyCircleType) < 0) {
        Py_DECREF(PyCircleType);
        Py_DECREF(m);
        return NULL;
    }
    Py_INCREF(PyDynObjectType);
    if (PyModule_AddObject(m, "Object", (PyObject *) PyDynObjectType) < 0) {
        Py_DECREF(PyCircleType);
        Py_DECREF(m);
        return NULL;
    }
    Py_INCREF(PySymbolType);
    if (PyModule_AddObject(m, "Symbol", (PyObject *) PySymbolType) < 0) {
        Py_DECREF(PyCircleType);
        Py_DECREF(m);
        return NULL;
    }
  
    return m;
}

