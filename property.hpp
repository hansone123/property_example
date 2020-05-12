#include <string>
#include <iostream>

using namespace std;

class PropertyBase{
public:
    enum Type{
        SWITCH_PROPERTY=0,
        I_RANGE_PROPERTY,
        F_RANGE_PROPERTY,
        PROPERTY_TYPENUM
    };
    // enum dataType{
    //     INT_PROPERTY=0,
    //     FLOAT_PROPERTY,
    //     BOOL_PROPERTY
       
    // }
    PropertyBase(Type type, string name, string desc) : type(type), name(name), desc(desc){}
    enum Type getPropertyType() {
        return type;
    }
	virtual void* GetPtr() = 0;
	string GetName() {
        return name;
    }
	string GetDesc() {
		return desc;
	}
private:
    Type type;
	string desc;
    string name;
};
template <class T>
class Property : public PropertyBase{
public:
    static_assert( sizeof(int) == sizeof(T) || sizeof(float) == sizeof(T) || sizeof(bool) == sizeof(T), "T type is not the specified DataType including int, float, bool");
    
    Property(PropertyBase::Type in_type, T in_def, string name, string desc) : PropertyBase(in_type, name, desc) {
        val = in_def;
        def = in_def;
    }
    ~Property() {};
    void Set(T in_val) {
        if (in_val == val)
            return;
        // TODO : notify listener
        val = in_val;
    }
    void* GetPtr() {
        // TODO : notify listener
        return (void*)&val;
    }
    
    T GetVal() {
        return val;
    }
    T GetDef() {
        return def;
    }
    
    string GetValType() {
        return string(typeid(T).name());
    }
protected:
    T val;
    T def;
};
class SwitchProperty : public Property<bool>{
public:
    SwitchProperty(Type in_type, bool in_def, string in_name, string desc) : Property(in_type, in_def, in_name, desc) {
    }
    ~SwitchProperty() {}
private:
};

template <class T>
class RangeProperty : public Property<T>{
public:
    static_assert( sizeof(int) == sizeof(T) || sizeof(float) == sizeof(T), "T type is not the specified DataType including int and float");
    RangeProperty(PropertyBase::Type in_type, T in_def, T in_min, T in_max,  string in_name, string desc) : Property<T>(in_type, in_def, in_name, desc) {
        min = in_min;
        max = in_max;
    }
    ~RangeProperty() {};
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
