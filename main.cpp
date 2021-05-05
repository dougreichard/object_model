#include "shape.h"
#include <iostream>
#include <sstream>


struct Print_Trait_2D : IVisitor, IVisit<Circle>, IVisit<Rect>, IVisit<DynCircle>, IVisit<Int>

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
    void visit(Int &i) {
        std::cout << "I am Int " << i << std::endl;
    }
};


////////////////
// This is just a start, a more genric version is possible
struct XML_Export :  
IVisitor, 
IVisit<Object>,
IVisit<Array>,
IVisit<Int>
{
    std::stringstream  xml;
    XML_Export()
    {
        xml << "<mission_data>" << std::endl;
    }
    ~XML_Export()
    {
        xml << "</mission_data>" << std::endl;
        std::cout << xml.str() << std::endl;
    }
     void visit(Array &a)
    {
       for(auto i=a._vec->begin();i!=a._vec->end();i++) {
          (*i)->accept(this);
       }
    }
    void visit(Object &o)
    {
       xml << "\t<" << o._type._name << " ";
       // auto it = mymap.begin(); it != mymap.end(); ++it
       for(auto i=o._kv->begin();i!=o._kv->end();i++) {
            xml << i->first._name << "=\"";
            i->second->accept(this);
            xml << "\" ";
       }
        xml << "/>"  << std::endl;
    }
    void visit(Int &i) {
        xml <<   i;
    }
};

struct JSON_Export :  
IVisitor, 
IVisit<Object>, 
IVisit<Array>, 
IVisit<Int>
{
    std::stringstream  xml;
    JSON_Export()
    {
        
    }
    ~JSON_Export()
    {

        std::cout << xml.str() << std::endl;
    }
    void visit(Object &o)
    {
       xml << "\t{ \"type:\"" << o._type._name << "\"";
       // auto it = mymap.begin(); it != mymap.end(); ++it
       for(auto i=o._kv->begin();i!=o._kv->end();i++) {
            xml << ",";
            xml << "\"" << i->first._name << "\":";
            i->second->accept(this);
       }
        xml << "}";
    }
    void visit(Array &a)
    {
       xml << "[" << std::endl;
       bool com = false;
       for(auto i=a._vec->begin();i!=a._vec->end();i++) {
           if (com) xml << "," << std::endl;
           else com=true;
          (*i)->accept(this);
       }
        xml << std::endl << "]" ;
    }
    void visit(Int &i) {
        xml <<   i;
    }
};



void main()
{
    obj::Int i = 16;
    obj::Float f = 1.2f;
    obj::String s = "Hello";
    obj::Object o;
    Circle c(10, 20, 5);
    c.x() = 14;

    Circle c2(c); // These are the same circle
    c2.y() = 25;   // Access and Change as an IntValue
    c2.x() = 33;  // Access and change as just a plain int
    ((Int&)c2.get(Prop::X)) = 44;
    auto got = Symbol::symbols.find("X");
    ((Int&)c2.get(*got->second)) = 55;
    
    Value& X = c2.get(*got->second);
    printf("%s", X._type._name.c_str());


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
    arr.push(i);

    for(size_t i=0, l=arr.size();i<l;i++) {
        arr[i].accept(&v);
    }
    {
        Array arr;
        arr.push(c);
        arr.push(dc);
        arr.push(r);

        XML_Export xml;
        arr.accept(&xml);
    }
    {
        Array arr;
        arr.push(c);
        arr.push(dc);
        arr.push(r);

        JSON_Export json;
        arr.accept(&json);
    }

}