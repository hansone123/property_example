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
    PropertyBase(Type inType) {
        type = inType;
    }
    enum Type getPropertyType() {
        return type;
    }
private:
    Type type;
    
};
template <class T>
class Property : public PropertyBase{
public:
    static_assert( sizeof(int) == sizeof(T) || sizeof(float) == sizeof(T) || sizeof(bool) == sizeof(T), "T type is not the specified DataType including int, float, bool");
    
    Property(PropertyBase::Type in_type, T in_def, string in_name) : PropertyBase(in_type) {
        name = in_name;
        val = in_def;
        def = in_def;
    }
    ~Property() {};
    void Set(T in_val) {
        if (in_val == val)
            return;
        // TODO : notify listener
        notifyListerner();
        val = in_val;
    }
    T& GetPtr() {
        // TODO : notify listener
        notifyListerner();
        return val;
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
    string GetValType() {
        return string(typeid(T).name());
    }
    virtual void info() {
        cout << this->GetName() << " " << this->GetValType() << " " << this->GetDef() << " " << this->GetVal() << endl;
    }
protected:
    string name;
    T val;
    T def;
    void notifyListerner() {
        cout << name << ": notify listerner" << endl;
    }
};
class SwitchProperty : public Property<bool>{
public:
    SwitchProperty(Type in_type, bool in_def, string in_name) : Property(in_type, in_def, in_name) {
    }
    ~SwitchProperty() {}
private:
};

template <class T>
class RangeProperty : public Property<T>{
public:
    static_assert( sizeof(int) == sizeof(T) || sizeof(float) == sizeof(T), "T type is not the specified DataType including int and float");
    RangeProperty(PropertyBase::Type in_type, T in_def, T in_min, T in_max,  string in_name) : Property<T>(in_type, in_def, in_name) {
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
    void info() override {
        cout << this->GetName() << " " << this->GetValType() << " " << this->GetDef() << " " << this->GetVal() << " " << this->GetMin() << "" << this->GetMax() << endl;
    }
private:
    T min;
    T max;
};
