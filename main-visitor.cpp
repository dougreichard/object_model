#include <iostream>


namespace Better {

/// Base interface fo a visitor
struct IVisitor {
    virtual ~IVisitor(){}
};
/// Base for anything that can be visited
struct IVisitable {
    virtual ~IVisitable(){}
    virtual void accept(IVisitor* v) {}
};

/// A way to implement the behavior
/// The cast and if is overhead, but give use super powers
/// And these days not a lot over execution overhead
template <typename T, typename B=IVisitable>
struct BVisitable : public B {
    void accept(IVisitor* v) {
        IVisit<T>* visit_me = dynamic_cast<IVisit<T>*>(v);
        if (visit_me) visit_me->visit((T*)this);
        else B::accept(v);
    }
};

// Used to build visitors 
template <typename T>
struct IVisit {
    virtual void visit(T* obj)=0;
};

////////////////////////////////
// Example Shape objects
struct Shape: public BVisitable<Shape>  {
};

// They can be small and data only
struct Circle: public BVisitable<Circle, Shape>
{
    float _radius;
    float _cx;
    float _cy;
};

struct Rect: public BVisitable<Rect, Shape>
{
    float _width;
    float _height;
    float _cx;
    float _cy;
};

////////////////////////////////////////////////////////
//  This 'trait' is similar to having a 
//  virtual print_2d() function on Shape
//      However, 
//         * You don't need to touch the shape classes at all
//         * You can have multiple visitors
//         * I don't do it here, but visitors can have state
//         
//  Note: It used multiple inheritance which is not evil in this use
//        You are selectively choosing what objects can be visited
struct Print_Trait_2D : IVisitor, IVisit<Circle>, IVisit<Rect> {
    Print_Trait_2D() {
            std::cout << "** Print 2D **" << std::endl;    
        }
    void visit(Rect* ) {
        std::cout << "I am Rect" << std::endl;
    }
    void visit(Circle* ) {
        std::cout << "I am Circle" << std::endl;
    }
};

struct Print_Trait_3D : IVisitor, IVisit<Circle>, IVisit<Rect> {
    Print_Trait_3D() {
            std::cout << "** Print 3D **" << std::endl;    
        }
    void visit(Rect* ) {
        std::cout << "I am 3D Rect" << std::endl;
    }
    void visit(Circle* ) {
        std::cout << "I am 3D Circle" << std::endl;
    }
};

// Cross cutting thingies
// Animals are not shapes, but are visitable
struct Animal : BVisitable<Animal> {

};

struct Dog : BVisitable<Dog,Animal> {

};
struct Cat : BVisitable<Cat,Animal> {

};

struct Frog : BVisitable<Frog,Animal> {

};
struct Bull : BVisitable<Bull,Animal> {

};

////////////////////////////
// A cross cutting visitor
// It can be used on Multiple things

struct Speak_Trait_All_The_Things : IVisitor, 
    IVisit<Circle>, IVisit<Rect>,
    IVisit<Dog>, IVisit<Cat>
    {
        Speak_Trait_All_The_Things() {
            std::cout << "** Speak All the things **" << std::endl;    
        }
    void visit(Rect* ) {
        std::cout << "This is what a Rect says" << std::endl;
    }
    void visit(Circle* ) {
        std::cout << "I'm no square" << std::endl;
    }
    void visit(Dog* ) {
        std::cout << "Bow, wow, wow" << std::endl;
    }
    void visit(Cat* ) {
        std::cout << "Whatever <eye roll> human" << std::endl;
    }
};

// Something to demonstrate polymorphic 
// behavior in this type of visitor
// Will be provide default Animale behavior
// Will be called with a BUll, that has no default behavior
// Will be called with a Frog, that has also calls base
struct Polly_Trait : IVisitor, 
      IVisit<Dog>, IVisit<Cat>,
      IVisit<Frog>, // But no BUll
      IVisit<Animal>  // Must include base
    {
        Polly_Trait() {
            std::cout << "** Polly wanna cast **" << std::endl;    
        }
    void visit(Animal* ) {
        std::cout << "Animal: I sit and stare at you" << std::endl;
    }
    void visit(Dog* ) {
        std::cout << "Bow, wow, wow" << std::endl;
    }
    void visit(Cat* ) {
        std::cout << "Whatever <eye roll> human" << std::endl;
    }
    void visit(Frog* obj) {
        // Call base
       this->visit((Animal*)obj);
        std::cout << "Ribbit" << std::endl;
    }
};

}



void main () {
    Better::Rect myrect;
    Better::Circle mycircle;

    Better::Shape* shapes[2] = {&myrect, &mycircle};

    Better::Print_Trait_2D print;
    for(int i=0; i<2;i++) {
        shapes[i]->accept(&print);
    }
    Better::Print_Trait_3D print3d;
    for(int i=0; i<2;i++) {
        shapes[i]->accept(&print3d);
    }
    
    Better::Dog mydog;
    Better::Cat mycat;
    // Example showing building collection of diverse objects
    // Animals and shapes in the same collections
    // but having cross cutting behavior
    Better::IVisitable* things[4] = {&mydog, &mycat, &myrect, &mycircle};
    Better::Speak_Trait_All_The_Things speak;
    for(int i=0; i<4;i++) {
        things[i]->accept(&speak);
    }


    Better::Frog myfrog;
    Better::Bull mybull;

    Better::IVisitable* morph[4] = {&mydog, &mycat, &myfrog, &mybull};
    Better::Polly_Trait polly;
    for(int i=0; i<4;i++) {
        morph[i]->accept(&polly);
    }
    
}


/////////////////////////////////////////////////////////
// If you google visitor this is more likely what you'll find
///////////////////////////////////////////////////////////
// Typically how people show visitors
// Can lead to just another hierarchy 
// Not as flexible as my "Better" visitor
namespace Typical {

// The typical one leads to the visitor knowing all the types
// and worse requiring all the types
// And a very strict visitor, not cross cutting
class Rect;
class Circle;

struct Visitor {
    virtual void visit(Rect* r) {};
    virtual void visit(Circle* r) {};
};

// Typically the accept is pure, and not polymorphic
struct Shape {
    virtual void accept(Visitor* v)=0;
};

struct Rect : public Shape {
    virtual void accept(Visitor* v) {
        v->visit(this);
    }
};

struct Circle : public Shape {
    virtual void accept(Visitor* v) {
        v->visit(this);
    }
};

struct Classic : Visitor {
    virtual void visit(Rect* ) {}
    virtual void visit(Circle* ) {}
};

};