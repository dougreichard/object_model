#pragma once

#include "object-model.h"
using namespace obj;
// Symbol can be used as a string
// or as an uint16_t

// ES is short of EngineSymbols
// These are symbols used for properties
// What is nice is these can be shared across objects
// Like cross cutting data
namespace Prop
{
// Symbol can be used as a string
// or as an uint16_t
    static const inline PropertySymbol X("X");
    static const inline PropertySymbol Y("Y");
    static const inline PropertySymbol R("R");
    static const inline PropertySymbol W("W");
    static const inline PropertySymbol H("H");

}; // namespace ES



static inline SystemSymbol circle_type("Circle");
struct Circle : Object
{
    MAKE_VISITABLE(Circle, Object)
    MAKE_OBJECT_POLY(Circle)
    static const Metadata& metadata;
    Circle(const Circle &rhs) : Object(rhs)
    {
        _x = rhs._x;
        _y = rhs._y;
        _r = rhs._r;
    }
    Circle(const Metadata& meta = Circle::metadata) : Object(metadata) {
    
    }
    Circle(int x, int y, int r) : Object(circle_type)
    {
        _x = x;
        _y = y;
        _r = r;
    }
    // This adds the class members to the
    // key/value map
    // This way they are avaible when enumerating key/values
   
    Int _x;
    Int _y;
    Int _r;
    inline Value &get(const Symbol &key)
    {
        if (key == Prop::X)
        {
            return _x;
        }
        else if (key == Prop::Y)
        {
            return _y;
        }
        else if (key == Prop::R)
        {
            return _r;
        }
        return Object::get(key);
    }
    inline Object &set(const Symbol &key, Value &value)
    {
        if (key == Prop::X)
        {
            _x = (Int &)value;
        }
        else if (key == Prop::Y)
        {
            _y = (Int &)value;
        }
        else if (key == Prop::R)
        {
            _r = (Int &)value;
        }
        else
        {
            return Object::set(key, value);
        }
        return *this;
    }
    virtual std::shared_ptr<Value> make_shared()
    {
        return std::shared_ptr<Value>(new Circle(_x, _y, _r));
    }
    virtual ValuePtr clone()
    {
        return ValuePtr(new Circle(_x, _y, _r), false);
    }

    inline Int &px() { return (Int &)get(Prop::X); }
    inline Int &x() { return _x; }
    inline Int &y() { return _y; }
    inline Int &r() { return _r; }
};

const inline Metadata& Circle::metadata{{
    {Prop::X, int_type, defaultInt, true},
    {Prop::Y, int_type, defaultInt, true},
    {Prop::R, int_type, defaultInt, true},
}};




static const inline SystemSymbol dyn_circle_type("DynCircle");
struct DynCircle : Object
{
    MAKE_VISITABLE(DynCircle, Object)
    static const Metadata& metadata;
    DynCircle(const Metadata& meta = DynCircle::metadata) : Object(metadata) {
    
    }
    DynCircle(int x, int y, int r) : Object(dyn_circle_type)
    {
        set(Prop::X, Int(x));
        set(Prop::Y, Int(y));
        set(Prop::R, Int(r));
    }
    int &x() { return (Int &)get(Prop::X); }
    int &y() { return (Int &)get(Prop::Y); }
    int &r() { return (Int &)get(Prop::R); }
    virtual ValuePtr clone()
    {
        return ValuePtr(new DynCircle(*this), false);
    }
};

const inline  Metadata& DynCircle::metadata{{
    {Prop::X, int_type, defaultInt, false},
    {Prop::Y, int_type, defaultInt, false},
    {Prop::R, int_type, defaultInt, false},
}};




static const inline SystemSymbol rect_type("Rect");
struct Rect : Object
{
    MAKE_VISITABLE(Rect, Object)
    static const Metadata& metadata;
    Rect(const Metadata& meta = Rect::metadata) : Object(metadata) {
    
    }

    Rect(int x, int y, int w, int h) : Object(rect_type)
    {
        set(Prop::X, Int(x));
        set(Prop::Y, Int(y));
        set(Prop::W, Int(w));
        set(Prop::H, Int(h));
    }
    int &x() { return (Int &)get(Prop::X); }
    int &y() { return (Int &)get(Prop::Y); }
    int &w() { return (Int &)get(Prop::W); }
    int &h() { return (Int &)get(Prop::H); }
    virtual ValuePtr clone()
    {
        return ValuePtr(new Rect(*this), false);
    }
};

const inline Metadata& Rect::metadata{{
    {Prop::X, int_type, defaultInt, false},
    {Prop::Y, int_type, defaultInt, false},
    {Prop::W, int_type, defaultInt, false},
    {Prop::W, int_type, defaultInt, false},
}};

