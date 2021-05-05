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
    #ifndef __DEFINE_STATIC__
        extern const PropertySymbol X;
        extern const PropertySymbol Y;
        extern const PropertySymbol R;
        extern const PropertySymbol W;
        extern const PropertySymbol H;
    #else
        extern const PropertySymbol X("X");
        extern const PropertySymbol Y("Y");
        extern const PropertySymbol R("R");
        extern const PropertySymbol W("W");
        extern const PropertySymbol H("H");
    #endif
}; // namespace ES

#ifdef __DEFINE_STATIC__
    extern SystemSymbol circle_type("Circle");
#else 
    extern SystemSymbol circle_type;
#endif

struct Circle : Object
{
    MAKE_VISITABLE_POLY(Circle, Object)
    Circle(const Circle& rhs) : Object(rhs) {
        _x = rhs._x;
        _y = rhs._y;
        _r = rhs._r;
        add_owned_props();
    }
    Circle(int x, int y, int r) : Object(circle_type)
    {
        _x = x;
        _y = y;
        _r = r;
        add_owned_props();
    }
    // This adds the class members to the 
    // key/value map
    // This way they are avaible when enumerating key/values
    void add_owned_props() {
        set_owned(Prop::X, _x);
        set_owned(Prop::Y, _y);
        set_owned(Prop::R, _r);
    }
    Int _x;
    Int _y;
    Int _r;
    inline Value &get(const Symbol &key)
    {
        if (key== Prop::X){
            return _x;
        } else if (key== Prop::Y){
            return _y;
        } else if (key== Prop::R){
            return _r;
        }
        return Object::get(key);
    }
    virtual std::shared_ptr<Value> make_shared()
    {
        return std::shared_ptr<Value>(new Circle(_x,_y,_r));
    }
    virtual ValuePtr clone(){
        return ValuePtr(new Circle(_x,_y,_r), false);
    }

    inline Int& px() {return (Int&)get(Prop::X);}
    inline Int& x() {return _x;}
    inline Int& y() {return _y;}
    inline Int& r() {return _r;}
};

#ifdef __DEFINE_STATIC__
    extern SystemSymbol dyn_circle_type("DynCircle");
#else 
    extern SystemSymbol dyn_circle_type;
#endif
struct DynCircle : Object
{
    MAKE_VISITABLE_POLY(DynCircle, Object)
    DynCircle(int x, int y, int r) : Object(dyn_circle_type)
    {
        set(Prop::X, Int(x));
        set(Prop::Y, Int(y));
        set(Prop::R, Int(r));
    }
    int &x() { return (Int &)get(Prop::X); }
    int &y() { return (Int &)get(Prop::Y); }
    int &r() { return (Int &)get(Prop::R); }
    virtual ValuePtr clone(){
        return ValuePtr(new DynCircle(*this), false);
    }
};


#ifdef __DEFINE_STATIC__
    SystemSymbol rect_type("Rect");
#else 
    extern SystemSymbol rect_type;
#endif


struct Rect : Object
{
    MAKE_VISITABLE_POLY(Rect, Object)
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
    virtual ValuePtr clone(){
        return ValuePtr(new Rect(*this), false);
    }
};

