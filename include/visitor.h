#pragma once

namespace obj {

#if defined(USE_VISITOR) || defined(USE_VISITOR_POLY)
    #define USE_VISITOR_ANY 1
    struct IVisitor
    {
        virtual ~IVisitor() {}
    };
    /// Base for anything that can be visited
    struct IVisitable
    {
        virtual ~IVisitable() {}
        virtual void accept(IVisitor *v) {}
    };
      // Used to build visitors
    template <typename T>
    struct IVisit
    {
        virtual void visit(T &obj) = 0;
    };
    
#endif

#ifdef USE_VISITOR
#define MAKE_VISITABLE(T, ...)                                   \
    void accept(IVisitor *v)                                \
    {                                                       \
        IVisit<T> *visit_me = dynamic_cast<IVisit<T> *>(v); \
        if (visit_me)                                       \
            visit_me->visit((T &)*this);                    \
    }
#endif

#ifdef USE_VISITOR_POLY
#define MAKE_VISITABLE(T, B)                           \
    void accept(IVisitor *v)                                \
    {                                                       \
        IVisit<T> *visit_me = dynamic_cast<IVisit<T> *>(v); \
        if (visit_me)                                       \
            visit_me->visit((T &)*this);                    \
        else                                                \
            B::accept(v);                                   \
    }
#endif

    
#if defined(USE_VISITOR) || defined(USE_VISITOR_POLY)
    template <typename T, typename B = IVisitable>
    struct BVisitable : public B
    {
        void accept(IVisitor *v)
        {
            IVisit<T> *visit_me = dynamic_cast<IVisit<T> *>(v);
            if (visit_me)
                visit_me->visit((T &)*this);
#endif
    
#if defined(USE_VISITOR_POLY)
            else
                B::accept(v);
#endif        
        
#if defined(USE_VISITOR) || defined(USE_VISITOR_POLY)

        }
    };
#endif 

#if !defined(USE_VISITOR) && !defined(USE_VISITOR_POLY)
    #define MAKE_VISITABLE(T,B) 
#endif

}
