#include "shape.h"
#include <iostream>

#include <chrono> 
using namespace std::chrono; 
  
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
    time_set_accessor();
    time_set_get_accessor();
    time_set_member();
}
