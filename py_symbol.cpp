#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "object-model.h"
#include "py_symbol.h"

using namespace obj;

typedef struct {
    PyObject_HEAD
    /* Type-specific fields go here. */
    Symbol* symbol;
} PySymbol;


static int
py_symbol_init(PySymbol *self, PyObject *args, PyObject *kws)
{
    const char *name;
    if (!PyArg_ParseTuple(args, "s", &name))
         return -1;    
    Symbol* symbol = Scope<Symbol>::instance().get_or_create_symbol<UserSymbol>(name);
    self->symbol = symbol;
    return 0;
}

static void 
py_symbol_dealloc(PySymbol *self) {
    PyTypeObject *tp = Py_TYPE(self);
    // The engine owns symbols
    tp->tp_free(self);
    Py_DECREF(tp);
}



extern PyMethodDef PySymbol_methods[] = {
    // {"Func", (PyCFunction) py_func_wrapper, METH_NOARGS,
    //  "Description"
    // },
    {NULL}  /* Sentinel */
};

//
extern PyTypeObject PySymbolType = {
    PyVarObject_HEAD_INIT(NULL, 0)
};

extern PyTypeObject* get_symbol_type() {
// Finish initializing Gateway object
    PySymbolType.tp_name = "shapes.Symbol";
    PySymbolType.tp_basicsize = sizeof(PySymbol);
    PySymbolType.tp_itemsize = 0;
    PySymbolType.tp_doc = "An Symbol ";
    PySymbolType.tp_flags = Py_TPFLAGS_DEFAULT;
    PySymbolType.tp_new = PyType_GenericNew;
    PySymbolType.tp_methods = PySymbol_methods;
    PySymbolType.tp_init = (initproc) py_symbol_init;
   // PySymbolType.tp_dealloc = (destructor) py_symbol_dealloc;
    //PySymbolType.tp_getattro = (getattrofunc) py_symbol_get_attro;
    //PySymbolType.tp_setattro = (setattrofunc) py_symbol_set_attro;
    return &PySymbolType;
}