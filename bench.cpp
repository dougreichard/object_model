#include "shape.h"
#include <iostream>

#include <chrono> 
using namespace std::chrono; 

struct SetViaVisit  : IVisitor, 
    IVisit<Circle>
    {
    Int value;

    void visit(Circle& c) {
        c._x =value;
    }
};

  
const int ITERATIONS = 100000;
void time_set_value()  {
    std::cout << "set full dynamic: ";
    DynCircle c(10, 20, 5);
    auto start = high_resolution_clock::now(); 
    for(int i=0;i<ITERATIONS;i++) {
        c.x() = i;
    }
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 
    if (c.x()) std::cout << duration.count() << "us" << std::endl; 
}
void time_dyn_set_value()  {
    std::cout << "set full dynamic set(): ";
    DynCircle c(10, 20, 5);
    auto start = high_resolution_clock::now(); 
    for(int i=0;i<ITERATIONS;i++) {
        c.set(Prop::X,Int(i));
    }
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 
    if (c.x()) std::cout << duration.count() << "us" << std::endl; 
}
void time_set_accessor()  {
    std::cout << "set via Value accessor: ";
    Circle c(10, 20, 5);
    auto start = high_resolution_clock::now(); 
    for(int i=0;i<ITERATIONS;i++) {
        c.x() = i;
    }
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 
    if (c.x())  std::cout << duration.count() << "us"  << std::endl; 
   
}
void time_set_get_accessor()  {
    std::cout << "set via Value accessor using get: ";
    Circle c(10, 20, 5);
    auto start = high_resolution_clock::now(); 
    for(int i=0;i<ITERATIONS;i++) {
        c.x() = i;
    }
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 
    if (c.px()) std::cout << duration.count() << "us"  << std::endl; 
    
}
void time_set_member()  {
    std::cout << "set class Value member: ";
    Circle c(10, 20, 5);
    auto start = high_resolution_clock::now(); 
    for(int i=0;i<ITERATIONS;i++) {
        c._x = i;
    }
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 
    // the if makes sure the code is not optimized out
    if (c._x)  std::cout << duration.count() << "us"  << std::endl; 

}
void time_set_function()  {
    std::cout << "set via set(): ";
    Circle c(10, 20, 5);
    auto start = high_resolution_clock::now(); 
    for(int i=0;i<ITERATIONS;i++) {
        c.set(Prop::X, Int(i));
    }
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 
    if (c.x()) std::cout << duration.count() << "us" << std::endl; 
}
void time_poly_set_function()  {
    std::cout << "set class Value via poly_set: ";
    Circle c(10, 20, 5);
    Object* o = &c;
    auto start = high_resolution_clock::now(); 
    for(int i=0;i<ITERATIONS;i++) {
        o->poly_set(Prop::X, Int(i));
    }
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 
    // the if makes sure the code is not optimized out
    if (c._x)  std::cout << duration.count() << "us"  << std::endl; 

}
void time_poly_get_function()  {
    std::cout << "poly_get: ";
    Circle c(10, 20, 5);
    Object* o = &c;
    int x=3;
    auto start = high_resolution_clock::now(); 
    for(int i=0;i<ITERATIONS;i++) {
    //for(int i=0;i<100;i++) {
        Int& v = (Int&)o->poly_get(Prop::X);
        x+=v; // Need to use v or it is optimized out
    }
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 
    // the if makes sure the code is not optimized out
    if (x)  std::cout << duration.count() << "us"  << std::endl; 
}
void time_set_via_poly_get_function()  {
    std::cout << "set class Value via poly_get: ";
    Circle c(10, 20, 5);
    Object* o = &c;
    int x=3;
    auto start = high_resolution_clock::now(); 
    for(int i=0;i<ITERATIONS;i++) {
    //for(int i=0;i<100;i++) {
        Int& v = (Int&)o->poly_get(Prop::X);
        v = x; // Need to use v or it is optimized out
    }
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 
    // the if makes sure the code is not optimized out
    if (x)  std::cout << duration.count() << "us"  << std::endl; 
}
void time_set_visitor()  {
    std::cout << "set class Value via visitor: ";
    Circle c(10, 20, 5);
    SetViaVisit v;
    auto start = high_resolution_clock::now(); 
    for(int i=0;i<ITERATIONS;i++) {
        v.value =i;
        c.accept(&v);
    }
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 
    // the if makes sure the code is not optimized out
    if (c._x)  std::cout << duration.count() << "us"  << std::endl; 

}


struct C {
    int x;
} c;
void time_set_struct()  {
    std::cout << "set C struct: ";
    auto start = high_resolution_clock::now(); 
    for(int i=0;i<ITERATIONS;i++) {
        c.x = i;
    }
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 
    if (c.x)std::cout << duration.count() << "us"  << std::endl; 

}

void main()
{
    std::cout << "Number of iterations: " << ITERATIONS << std::endl; 
    time_set_struct();
    time_set_value();
    time_dyn_set_value();
    time_set_accessor();
    time_set_get_accessor();
    time_set_member();
    time_set_function();
    time_poly_set_function();
    time_poly_get_function();
    time_set_via_poly_get_function();
    time_set_visitor();
}
