#include <iostream>
#include "property.hpp"
#include <map>

using namespace std;

int main()
{
    SwitchProperty* p1 = new SwitchProperty(PropertyBase::SWITCH_PROPERTY, true, string("enable_shit"));
    // p1->info();
    RangeProperty<int>* p2 = new RangeProperty<int>(PropertyBase::I_RANGE_PROPERTY, 10, 1, 100, string("int_range_property"));
    // p2->info();
    p2->Set(101);
    // p2->info();

    RangeProperty<float>* p3 = new RangeProperty<float>(PropertyBase::F_RANGE_PROPERTY, 0.5f, 0.f, 10.0f, string("float_range_property"));
    // p3->info();
    p3->Set(10.1f);
    
    auto& val = p3->GetPtr();
    val = 0.1f;
    // p3->info();
    
    // test map
    map<string, PropertyBase*> property_set;
    property_set[p1->GetName()] = p1;
    property_set[p2->GetName()] = p2;
    property_set[p3->GetName()] = p3;
    
    if (!property_set.empty()) {
		for (auto i = property_set.begin(); i != property_set.end(); ++i) {
			if (i->second) {
                PropertyBase* property = i->second;
				switch (property->getPropertyType()) {
                    case PropertyBase::I_RANGE_PROPERTY:
                        ((RangeProperty<int>*) property)->info();
                        break;
                    case PropertyBase::F_RANGE_PROPERTY:
                        ((RangeProperty<float>*) property)->info();
                        break;
                    case PropertyBase::SWITCH_PROPERTY:
                        ((SwitchProperty*) property)->info();
                        break;

                }
			}
		}
	}  
    return 0;
}