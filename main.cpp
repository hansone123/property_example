#include <iostream>
#include <map>
#include <fstream>
#include "property.hpp"
#include "json/json.h"

using namespace std;
// void* getPtr(PropertyBase* property) {

// }

map<string, PropertyBase*> loadConfig(char* filepath) {
	Json::Value data;
	std::ifstream ifs;
	ifs.open(filepath);
	Json::CharReaderBuilder builder;
	builder["collectComments"] = true;
	JSONCPP_STRING errs;
	map<string, PropertyBase*> properties;
	if (!parseFromStream(builder, ifs, &data, &errs)) {
		std::cout << errs << std::endl;
		return properties;
	}
	for (int i=0; i<data.size(); i++) {
		if (data[i]["type"].asString().compare("bool") == 0) {
			properties[data[i]["name"].asString()] = new SwitchProperty(PropertyBase::SWITCH_PROPERTY, data[i]["val"].asBool(), 
														data[i]["name"].asString(), string("desc"));
			// printf("[%s] %d\n",data[i]["name"].asString().c_str(), data[i]["val"].asBool());
		}
		else if (data[i]["type"].asString().compare("float") == 0) {
			properties[data[i]["name"].asString()] = new RangeProperty<float>(PropertyBase::F_RANGE_PROPERTY, data[i]["val"].asFloat(), 
														data[i]["min"].asFloat(), data[i]["max"].asFloat(), data[i]["name"].asString(), string("desc"));
			// printf("[%s] %d\n",data[i]["name"].asString().c_str(), data[i]["val"].asInt());
		}
		else if (data[i]["type"].asString().compare("int") == 0) {
			properties[data[i]["name"].asString()] = new RangeProperty<int>(PropertyBase::I_RANGE_PROPERTY, data[i]["val"].asInt(), 
														data[i]["min"].asInt(), data[i]["max"].asInt(), data[i]["name"].asString(), string("desc"));
			// printf("[%s] %f\n",data[i]["name"].asString().c_str(), data[i]["val"].asFloat());
			
		}
	}
	return properties;
}
int main(void) 
{
	const char* src_dir = "./config/demokit_config.json"; 
	map<string, PropertyBase*> properties = loadConfig((char*)src_dir);
	for (auto it = properties.begin(); it != properties.end(); it++) 
	{
		switch(it->second->getPropertyType()) {
			case PropertyBase::SWITCH_PROPERTY:
				cout << it->second->GetName() << " : " << *(bool*)it->second->GetPtr()<< endl;
				break;
        	case PropertyBase::I_RANGE_PROPERTY:
				cout << it->second->GetName() << " : " << *(int*)it->second->GetPtr()<< endl;
				break;
        	case PropertyBase::F_RANGE_PROPERTY:
				cout << it->second->GetName() << " : " << *(float*)it->second->GetPtr()<< endl;
				break;

		}
	}
	return 1;
}