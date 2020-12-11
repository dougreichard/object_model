#include "shape.h"
#include <iostream>


struct Print_Trait_2D : IVisitor, IVisit<Circle>, IVisit<Rect>, IVisit<DynCircle>
{
    Print_Trait_2D()
    {
        std::cout << "** Print 2D **" << std::endl;
    }
    void visit(Rect &r)
    {
        std::cout << "I am Rect "
                  << r.x() << "," << r.y()
                  << "," << r.w() << "," << r.h()
                  << std::endl;
    }
    void visit(Circle &c)
    {
        std::cout << "I am Circle "
                  << c.x() << "," << c.y()
                  << "," << c.r()
                  << std::endl;
    }
    void visit(DynCircle &c)
    {
        std::cout << "I am DynCircle "
                  << c.x() << "," << c.y()
                  << "," << c.r()
                  << std::endl;
    }
};

void main()
{
    obj::Int i = 16;
    obj::Float f = 1.2;
    obj::String s = "Hello";
    obj::Object o;
    Circle c(10, 20, 5);
    c.x() = 14;

    Circle c2(c); // These are the same circle
    c2.y() = 25;   // Access and Change as an IntValue
    c2.x() = 33;  // Access and change as just a plain int

    DynCircle dc(10, 20, 5);
    dc.x() = 14;

    DynCircle dc2(dc); // These are the same circle
    dc2.y() = 25;   // Access and Change as an IntValue
    dc2.x() = 33;  // Access and change as just a plain int



    Rect r(1,2,3,4);
    r.set(Prop::R, Int(2));

    SystemSymbol round_rect_type("rect");   
    r.add_role(round_rect_type);
    if (r.has_role(round_rect_type)) {
        r.remove_role(round_rect_type);
        std::cout << "Role Added";
    }
    if (!r.has_role(round_rect_type)) {
        std::cout << "Role removed";
    }


    Print_Trait_2D v;
    Array arr;
    arr.push(c2);
    arr.push(c);
    arr.push(dc);
    arr.push(dc2);
    arr.push(r);

    for(int i=0, l=arr.size();i<l;i++) {
        arr[i].accept(&v);
    }
    
    // c.accept(&v);
    // dc2.accept(&v);
    // dc.accept(&v);
    // r.accept(&v);
}