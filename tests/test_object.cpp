#include "catch2/catch.hpp"
#include "object.h"

using namespace obj;
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

static const inline SystemSymbol dyn_circle_type("DynCircle");
struct DynCircle : Object
{
    MAKE_VISITABLE(DynCircle, Object)
    static const Metadata &metadata;
    DynCircle(const Metadata &meta = DynCircle::metadata) : Object(metadata)
    {
    }
    virtual Value* clone()
    {
        return new DynCircle(*this);
    }
};
const Metadata &min_metadata{
    {
        {Prop::X, int_type, ValuePtr(new Int(1)), false},
        {Prop::Y, int_type, ValuePtr(new Int(2)), false},
        {Prop::W, int_type, ValuePtr(new Int(3)), false},
        {Prop::H, int_type, ValuePtr(new Int(4)), false},
    }};

using namespace obj;

TEST_CASE("Object min", "[object]")
{
    const SystemSymbol rect_type("MinRect");
    Object obj = Object(min_metadata, rect_type);

    SECTION("Check initial values via metadata")
    {
        REQUIRE(obj._type == rect_type);
        REQUIRE(obj.get<Int &>(Prop::X) == 1);
        REQUIRE(obj.get<Int &>(Prop::Y) == 2);
        REQUIRE(obj.get<Int &>(Prop::W) == 3);
        REQUIRE(obj.get<Int &>(Prop::H) == 4);
    }

    SECTION("Change values via get")
    {
        obj.get<Int &>(Prop::X) = 10;
        obj.get<Int &>(Prop::Y) = 20;
        obj.get<Int &>(Prop::W) = 30;
        obj.get<Int &>(Prop::H) = 40;

        REQUIRE(obj.get<Int &>(Prop::X) == 10);
        REQUIRE(obj.get<Int &>(Prop::Y) == 20);
        REQUIRE(obj.get<Int &>(Prop::W) == 30);
        REQUIRE(obj.get<Int &>(Prop::H) == 40);
    }

    SECTION("Change values via Set")
    {
        obj.set<Int &>(Prop::X, Int(100));
        obj.set<Int &>(Prop::Y, Int(200));
        obj.set<Int &>(Prop::W, Int(300));
        obj.set<Int &>(Prop::H, Int(400));

        REQUIRE(obj.get<Int &>(Prop::X) == 100);
        REQUIRE(obj.get<Int &>(Prop::Y) == 200);
        REQUIRE(obj.get<Int &>(Prop::W) == 300);
        REQUIRE(obj.get<Int &>(Prop::H) == 400);
    }
}

TEST_CASE("Object copy object ", "[object]")
{
    const SystemSymbol rect_type("MinRect");
    Object obj = Object(min_metadata, rect_type);

        
    obj.set<Int &>(Prop::X, Int(100));
    obj.set<Int &>(Prop::Y, Int(200));
    obj.set<Int &>(Prop::W, Int(300));
    obj.set<Int &>(Prop::H, Int(400));

    REQUIRE(obj.get<Int &>(Prop::X) == 100);
    REQUIRE(obj.get<Int &>(Prop::Y) == 200);
    REQUIRE(obj.get<Int &>(Prop::W) == 300);
    REQUIRE(obj.get<Int &>(Prop::H) == 400);
    

    Object ref(obj);

    REQUIRE(ref.get<Int &>(Prop::X) == 100);
    REQUIRE(ref.get<Int &>(Prop::Y) == 200);
    REQUIRE(ref.get<Int &>(Prop::W) == 300);
    REQUIRE(ref.get<Int &>(Prop::H) == 400);

    ref.set<Int &>(Prop::X, Int(1000));
    ref.set<Int &>(Prop::Y, Int(2000));
    ref.set<Int &>(Prop::W, Int(3000));
    ref.set<Int &>(Prop::H, Int(4000));

    REQUIRE(ref.get<Int &>(Prop::X) == 1000);
    REQUIRE(ref.get<Int &>(Prop::Y) == 2000);
    REQUIRE(ref.get<Int &>(Prop::W) == 3000);
    REQUIRE(ref.get<Int &>(Prop::H) == 4000);

    REQUIRE(obj.get<Int &>(Prop::X) == 1000);
    REQUIRE(obj.get<Int &>(Prop::Y) == 2000);
    REQUIRE(obj.get<Int &>(Prop::W) == 3000);
    REQUIRE(obj.get<Int &>(Prop::H) == 4000);
}

TEST_CASE("Object clone object ", "[object]")
{
    const SystemSymbol rect_type("MinRect");
    Object obj = Object(min_metadata, rect_type);

    obj.set<Int &>(Prop::X, Int(100));
    obj.set<Int &>(Prop::Y, Int(200));
    obj.set<Int &>(Prop::W, Int(300));
    obj.set<Int &>(Prop::H, Int(400));

    REQUIRE(obj.get<Int &>(Prop::X) == 100);
    REQUIRE(obj.get<Int &>(Prop::Y) == 200);
    REQUIRE(obj.get<Int &>(Prop::W) == 300);
    REQUIRE(obj.get<Int &>(Prop::H) == 400);
    

    ValuePtr clone = ValuePtr(obj.clone());
    Object& ref = clone.ref<Object>();

    REQUIRE(ref.get<Int &>(Prop::X) == 100);
    REQUIRE(ref.get<Int &>(Prop::Y) == 200);
    REQUIRE(ref.get<Int &>(Prop::W) == 300);
    REQUIRE(ref.get<Int &>(Prop::H) == 400);

    ref.set<Int &>(Prop::X, Int(1000));
    ref.set<Int &>(Prop::Y, Int(2000));
    ref.set<Int &>(Prop::W, Int(3000));
    ref.set<Int &>(Prop::H, Int(4000));

    REQUIRE(ref.get<Int &>(Prop::X) == 1000);
    REQUIRE(ref.get<Int &>(Prop::Y) == 2000);
    REQUIRE(ref.get<Int &>(Prop::W) == 3000);
    REQUIRE(ref.get<Int &>(Prop::H) == 4000);

    REQUIRE(obj.get<Int &>(Prop::X) == 100);
    REQUIRE(obj.get<Int &>(Prop::Y) == 200);
    REQUIRE(obj.get<Int &>(Prop::W) == 300);
    REQUIRE(obj.get<Int &>(Prop::H) == 400);
}

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
    virtual Value* clone()
    {
        return (Value*) new Circle(_x, _y, _r);
    }
};
const Metadata& Circle::metadata{{
    {Prop::X, int_type, defaultInt, true},
    {Prop::Y, int_type, defaultInt, true},
    {Prop::R, int_type, defaultInt, true},
}};

TEST_CASE("Hybird Object", "[object]")
{

}
