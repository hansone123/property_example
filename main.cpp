#include <iostream>
#include "property.hpp"


int main()
{
    switchProperty p1(string("switch_property"), true);
    cout << p1.GetName() << " " << p1.GetDef() << " " << p1.GetVal() << endl;
    rangeProperty<int> p2(string("int_range_property"), 10, 1, 100);
    cout << p2.GetName() << " " << p2.GetTypeName() << " " << p2.GetDef() << " " << p2.GetVal() << " " << p2.GetMin() << "" << p2.GetMax() << endl;
    p2.Set(101);
    p2.Update();
    cout << p2.GetName() << " " << p2.GetTypeName() << " " << p2.GetDef() << " " << p2.GetVal() << " " << p2.GetMin() << "" << p2.GetMax() << endl;

    rangeProperty<float> p3(string("float_range_property"), 0.5f, 0.f, 10.0f);
    cout << p3.GetName() << " " << p3.GetTypeName() << " " << p3.GetDef() << " " << p3.GetVal() << " " << p3.GetMin() << "" << p3.GetMax() << endl;
    p3.Set(10.1f);
    p3.Update();
    cout << p3.GetName() << " " << p3.GetTypeName() << " " << p3.GetDef() << " " << p3.GetVal() << " " << p3.GetMin() << "" << p3.GetMax() << endl;
    auto& val = p3.GetPtr();
    val = 0.1f;
    p3.Update();
    cout << p3.GetName() << " " << p3.GetTypeName() << " " << p3.GetDef() << " " << p3.GetVal() << " " << p3.GetMin() << "" << p3.GetMax() << endl;
    return 0;
}