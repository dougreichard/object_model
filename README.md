# object_model
Current state (work in progress for demo/example purposes)

Creates a C++ Object model that can be exposed like a Java Script/Python object model

Can be used to create objects that are fast in C++, but can be exposed to a scripting language. Could also allow for letting scripting languages or end users create custom objects.


- main.cpp - Just a simple set of objects and visitor
- bench.cpp - Some test to compare overhead
- object-model.h - The object model definition
- shape.h - Example objects using the object model
 - Circle - An example Hybrid object internally a struct that can be exposed as an Object
 - DynCircle - Same circle as a full dynamic object deriving from Object
 - Rect - Object


## Example run of benchmark
Test accessing a Value 100,000 times
Release mode build with optimizations

```
Number of iterations: 100000
set C struct: 24us
set full dynamic: 220us
set via Value accessor: 17us
set via Value accessor using get: 24us
set class Value member: 15us

```

# Main Classes

Symbol - Symbols are the key for key value pairs. Internally uint16 but can be user facing as a string
Value - abstract base for Values
TValue - Template for Wrapping atomic types: Int, Float, String, Array, Object
Object - A key value Object
Array - An array of Values

# Visitor
Also provide extension of the model via a Visit pattern follow in Open-Closed Principle.
