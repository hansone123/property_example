#include <string>
#include <iostream>

using namespace std;
template <class T>
class Property{
public:
    static_assert( sizeof(int) == sizeof(T) || sizeof(float) == sizeof(T) || sizeof(bool) == sizeof(T),"T type is not the specified DataType including int and float");

    Property(string in_name, T in_def) {
        name = in_name;
        val = in_def;
        def = in_def;
    }
    ~Property() {};
    bool Set(T in_next) {
        next = in_next;
    }
    T& GetPtr() {
        return next;
    }
    T GetVal() {
        return val;
    }
    T GetDef() {
        return def;
    }
    string GetName() {
        return name;
    }
    string GetTypeName() {
        return string(typeid(T).name());
    }
    bool Update() {
        val = next;
    }
protected:
    string name;
    T val;
    T def;
    T next;
private:
    auto& getValue();
};
class switchProperty : public Property<bool>{
public:
    switchProperty(string in_name, bool in_def) : Property(in_name, in_def) {
    };
    ~switchProperty() {};
    
private:

};

template <class T>
class rangeProperty : public Property<T>{
public:
    static_assert( sizeof(int) == sizeof(T) || sizeof(float) == sizeof(T),"T type is not the specified DataType including int and float");
    rangeProperty(string in_name, T in_def, T in_min, T in_max) : Property<T>(in_name, in_def) {
        min = in_min;
        max = in_max;
    };
    ~rangeProperty(){};
    bool Set(T in_val)  {
        in_val = in_val > max ? max : in_val;
        in_val = in_val < min ? min : in_val;
        Property<T>::Set(in_val);
    }
    T GetMin() {
        return min;
    }
    T GetMax() {
        return max;
    }
private:
    T min;
    T max;
};
