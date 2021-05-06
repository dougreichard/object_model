namespace obj
{
    
    ////////////////////////////////////
    // Enum
    //   This allows you to use a symbol as a value
    //   This is useful to have a value that is an 'int' in code
    // template <typename T, const Symbol &name>
    // struct TEnum : TValue<T, name>
    // {
    //     TValue() : Value(name) {}
    //     TValue(const T &value) : Value(name), _value(value) {}
    //     T _value;
    //     inline operator const T &() const { return _value; }
    //     inline operator T &() { return _value; }
    //     inline TValue &operator=(const TValue &value)
    //     {
    //         _value = value;
    //         return *this;
    //     }
    //     inline TValue &operator=(const T &value)
    //     {
    //         _value = value;
    //         return *this;
    //     }
    //     virtual std::shared_ptr<Value> make_shared()
    //     {
    //         return std::shared_ptr<Value>(new TValue(*this));
    //     }
    //     virtual ValuePtr clone()
    //     {
    //         return ValuePtr(new TValue(*this), false);
    //     }
    //     void accept(IVisitor *v)
    //     {
    //         IVisit<TValue> *visit_me = dynamic_cast<IVisit<TValue> *>(v);
    //         if (visit_me)
    //             visit_me->visit((TValue &)*this);
    //     }
    // };

   
} // namespace obj
