#include <iostream>
#include "property.hpp"
#include <map>

using namespace std;
// void* getPtr(PropertyBase* property) {

// }
int main()
{
    // test map
    map<string, PropertyBase*> property_set;
    property_set["enable"] = new SwitchProperty(PropertyBase::SWITCH_PROPERTY, true, string("enable_shit"));;
    property_set["int_range"] = new RangeProperty<int>(PropertyBase::I_RANGE_PROPERTY, 10, 1, 100, string("int_range_property"));
    property_set["float_range"] = new RangeProperty<float>(PropertyBase::F_RANGE_PROPERTY, 0.5f, 0.f, 10.0f, string("float_range_property"));
    
	bool* val = ((SwitchProperty*)property_set["enable"])->GetPtr();
	*val = false;
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