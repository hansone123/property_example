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
void saveConfig(char* filepath, map<string, PropertyBase*> config) {
	Json::Value data;
	int i=0;
	for (auto it = config.begin(); it != config.end(); it++, i++) 
	{
		data[i]["name"] = it->second->GetName();
		
		switch(it->second->getPropertyType()) {
			case PropertyBase::SWITCH_PROPERTY:
				data[i]["type"] = "bool";
				data[i]["val"] = *(bool*)it->second->GetPtr();
				break;
        	case PropertyBase::I_RANGE_PROPERTY:
				data[i]["type"] = "int";
				data[i]["val"] = *(int*)it->second->GetPtr();
				data[i]["min"] = ((RangeProperty<int>*)it->second)->GetMin();
				data[i]["max"] = ((RangeProperty<int>*)it->second)->GetMax();
				break;
        	case PropertyBase::F_RANGE_PROPERTY:
				data[i]["type"] = "float";
				data[i]["val"] = *(float*)it->second->GetPtr();
				data[i]["min"] = ((RangeProperty<float>*)it->second)->GetMin();
				data[i]["max"] = ((RangeProperty<float>*)it->second)->GetMax();
				break;

		}
		data[i]["desc"] = it->second->GetDesc();
	}

	Json::StreamWriterBuilder builder;
	const std::string json_file = Json::writeString(builder, data);
	std::cout << json_file << std::endl;
	ofstream file(filepath);
	file << json_file;
	cout << "config saved." << endl;
}
int main(void) 
{
	const char* src_dir = "./config/demokit_config2.json"; 
	const char* dist_dir = "./config/demokit_config3.json"; 
	map<string, PropertyBase*> config = loadConfig((char*)src_dir);
	saveConfig((char*)dist_dir, config);
	// for (auto it = config.begin(); it != config.end(); it++) 
	// {
	// 	switch(it->second->getPropertyType()) {
	// 		case PropertyBase::SWITCH_PROPERTY:
	// 			cout << it->second->GetName() << " : " << *(bool*)it->second->GetPtr()<< endl;
	// 			break;
    //     	case PropertyBase::I_RANGE_PROPERTY:
	// 			cout << it->second->GetName() << " : " << *(int*)it->second->GetPtr()<< endl;
	// 			break;
    //     	case PropertyBase::F_RANGE_PROPERTY:
	// 			cout << it->second->GetName() << " : " << *(float*)it->second->GetPtr()<< endl;
	// 			break;

	// 	}
	// }
	return 1;
}