#include <iostream>
#include <map>
#include <fstream>
#include "property.hpp"
#include "json/json.h"

using namespace std;

struct demokit_config_s{
	RangeProperty<int>* baseline;
	RangeProperty<int>* camera_mode;
	RangeProperty<int>* depth_color_z_ofst;
	RangeProperty<int>* depth_color_z_range;
	RangeProperty<int>* depth_h_range_min;
	RangeProperty<int>* depth_z_cut_ofst;
	RangeProperty<int>* depth_z_cut_range;
	SwitchProperty* enable_hole_fill;
	SwitchProperty* enable_ir;
	SwitchProperty* enable_screen_touch;
	RangeProperty<int>* h_range_max;
	RangeProperty<float>* imgui_distancebar_h;
	RangeProperty<float>* imgui_distancebar_w;
	RangeProperty<float>* imgui_fontSize;
	RangeProperty<int>* imgui_spot_size;
	RangeProperty<int>* pc_camera_z_ofst;
	RangeProperty<int>* pc_color_z_ofst;
	RangeProperty<int>* pc_color_z_range;
	SwitchProperty* pc_enable_filter;
	SwitchProperty* pc_enable_rotate;
	RangeProperty<int>* pc_filter_dlife;
	RangeProperty<int>* pc_filter_dthreshold;
	RangeProperty<int>* pc_point_size;
	RangeProperty<int>* pc_rotate_speed;
	RangeProperty<int>* pc_sample_level;
	RangeProperty<int>* pc_view_mode;
	RangeProperty<int>* pc_x_cut_ofst;
	RangeProperty<int>* pc_x_cut_range;
	RangeProperty<int>* pc_y_cut_ofst;
	RangeProperty<int>* pc_y_cut_range;
	RangeProperty<int>* pc_z_cut_ofst;
	RangeProperty<int>* pc_z_cut_range;
	RangeProperty<int>* resolution;

};
typedef demokit_config_s DemokitConfig;
DemokitConfig config;

map<string, PropertyBase*> loadConfigFile(char* filepath) {
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
														data[i]["name"].asString(), data[i]["desc"].asString());
			// printf("[%s] %d\n",data[i]["name"].asString().c_str(), data[i]["val"].asBool());
		}
		else if (data[i]["type"].asString().compare("float") == 0) {
			properties[data[i]["name"].asString()] = new RangeProperty<float>(PropertyBase::F_RANGE_PROPERTY, data[i]["val"].asFloat(), 
														data[i]["min"].asFloat(), data[i]["max"].asFloat(), data[i]["name"].asString(), data[i]["desc"].asString());
			// printf("[%s] %d\n",data[i]["name"].asString().c_str(), data[i]["val"].asInt());
		}
		else if (data[i]["type"].asString().compare("int") == 0) {
			properties[data[i]["name"].asString()] = new RangeProperty<int>(PropertyBase::I_RANGE_PROPERTY, data[i]["val"].asInt(), 
														data[i]["min"].asInt(), data[i]["max"].asInt(), data[i]["name"].asString(), data[i]["desc"].asString());
			// printf("[%s] %f\n",data[i]["name"].asString().c_str(), data[i]["val"].asFloat());
			
		}
	}
	return properties;
}
void saveConfigFile(char* filepath, map<string, PropertyBase*> config) {
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
	file.close();
	cout << "config saved." << endl;
}
void dumpConfigName(map<string, PropertyBase*> config) {
	ofstream file("dump_config_name.txt");
	char line[128];
	for (auto it = config.begin(); it != config.end(); it++) 
	{
		switch(it->second->getPropertyType()) {
			case PropertyBase::SWITCH_PROPERTY:
				sprintf(line, "config.%s = (SwitchProperty*)properties[\"%s\"];\n", it->second->GetName().c_str(), it->second->GetName().c_str());
				break;
        	case PropertyBase::I_RANGE_PROPERTY:
				sprintf(line, "config.%s = (RangeProperty<int>*)properties[\"%s\"];\n", it->second->GetName().c_str(), it->second->GetName().c_str());
				break;
        	case PropertyBase::F_RANGE_PROPERTY:
				sprintf(line, "config.%s = (RangeProperty<float>*)properties[\"%s\"];\n", it->second->GetName().c_str(), it->second->GetName().c_str());
				break;

		}
		file << line;
	}
	file.close();
}
void loadConfig(char* filepath) {
	map<string, PropertyBase*> properties = loadConfigFile(filepath);
	config.baseline = (RangeProperty<int>*)properties["baseline"];
	config.camera_mode = (RangeProperty<int>*)properties["camera_mode"];
	config.depth_color_z_ofst = (RangeProperty<int>*)properties["depth_color_z_ofst"];
	config.depth_color_z_range = (RangeProperty<int>*)properties["depth_color_z_range"];
	config.depth_h_range_min = (RangeProperty<int>*)properties["depth_h_range_min"];
	config.depth_z_cut_ofst = (RangeProperty<int>*)properties["depth_z_cut_ofst"];
	config.depth_z_cut_range = (RangeProperty<int>*)properties["depth_z_cut_range"];
	config.enable_hole_fill = (SwitchProperty*)properties["enable_hole_fill"];
	config.enable_ir = (SwitchProperty*)properties["enable_ir"];
	config.enable_screen_touch = (SwitchProperty*)properties["enable_screen_touch"];
	config.h_range_max = (RangeProperty<int>*)properties["h_range_max"];
	config.imgui_distancebar_h = (RangeProperty<float>*)properties["imgui_distancebar_h"];
	config.imgui_distancebar_w = (RangeProperty<float>*)properties["imgui_distancebar_w"];
	config.imgui_fontSize = (RangeProperty<float>*)properties["imgui_fontSize"];
	config.imgui_spot_size = (RangeProperty<int>*)properties["imgui_spot_size"];
	config.pc_camera_z_ofst = (RangeProperty<int>*)properties["pc_camera_z_ofst"];
	config.pc_color_z_ofst = (RangeProperty<int>*)properties["pc_color_z_ofst"];
	config.pc_color_z_range = (RangeProperty<int>*)properties["pc_color_z_range"];
	config.pc_enable_filter = (SwitchProperty*)properties["pc_enable_filter"];
	config.pc_enable_rotate = (SwitchProperty*)properties["pc_enable_rotate"];
	config.pc_filter_dlife = (RangeProperty<int>*)properties["pc_filter_dlife"];
	config.pc_filter_dthreshold = (RangeProperty<int>*)properties["pc_filter_dthreshold"];
	config.pc_point_size = (RangeProperty<int>*)properties["pc_point_size"];
	config.pc_rotate_speed = (RangeProperty<int>*)properties["pc_rotate_speed"];
	config.pc_sample_level = (RangeProperty<int>*)properties["pc_sample_level"];
	config.pc_view_mode = (RangeProperty<int>*)properties["pc_view_mode"];
	config.pc_x_cut_ofst = (RangeProperty<int>*)properties["pc_x_cut_ofst"];
	config.pc_x_cut_range = (RangeProperty<int>*)properties["pc_x_cut_range"];
	config.pc_y_cut_ofst = (RangeProperty<int>*)properties["pc_y_cut_ofst"];
	config.pc_y_cut_range = (RangeProperty<int>*)properties["pc_y_cut_range"];
	config.pc_z_cut_ofst = (RangeProperty<int>*)properties["pc_z_cut_ofst"];
	config.pc_z_cut_range = (RangeProperty<int>*)properties["pc_z_cut_range"];
	config.resolution = (RangeProperty<int>*)properties["resolution"];
}
void saveConfig(char* filepath) {
	map<string, PropertyBase*> properties;
	properties["baseline"] = config.baseline;
	properties["camera_mode"] = config.camera_mode;
	properties["depth_color_z_ofst"] = config.depth_color_z_ofst;
	properties["depth_color_z_range"] = config.depth_color_z_range;
	properties["depth_h_range_min"] = config.depth_h_range_min;
	properties["depth_z_cut_ofst"] = config.depth_z_cut_ofst;
	properties["depth_z_cut_range"] = config.depth_z_cut_range;
	properties["enable_hole_fill"] = config.enable_hole_fill;
	properties["enable_ir"] = config.enable_ir;
	properties["enable_screen_touch"] = config.enable_screen_touch;
	properties["h_range_max"] = config.h_range_max;
	properties["imgui_distancebar_h"] = config.imgui_distancebar_h;
	properties["imgui_distancebar_w"] = config.imgui_distancebar_w;
	properties["imgui_fontSize"] = config.imgui_fontSize;
	properties["imgui_spot_size"] = config.imgui_spot_size;
	properties["pc_camera_z_ofst"] = config.pc_camera_z_ofst;
	properties["pc_color_z_ofst"] = config.pc_color_z_ofst;
	properties["pc_color_z_range"] = config.pc_color_z_range;
	properties["pc_enable_filter"] = config.pc_enable_filter;
	properties["pc_enable_rotate"] = config.pc_enable_rotate;
	properties["pc_filter_dlife"] = config.pc_filter_dlife;
	properties["pc_filter_dthreshold"] = config.pc_filter_dthreshold;
	properties["pc_point_size"] = config.pc_point_size;
	properties["pc_rotate_speed"] = config.pc_rotate_speed;
	properties["pc_sample_level"] = config.pc_sample_level;
	properties["pc_view_mode"] = config.pc_view_mode;
	properties["pc_x_cut_ofst"] = config.pc_x_cut_ofst;
	properties["pc_x_cut_range"] = config.pc_x_cut_range;
	properties["pc_y_cut_ofst"] = config.pc_y_cut_ofst;
	properties["pc_y_cut_range"] = config.pc_y_cut_range;
	properties["pc_z_cut_ofst"] = config.pc_z_cut_ofst;
	properties["pc_z_cut_range"] = config.pc_z_cut_range;
	properties["resolution"] = config.resolution;
	saveConfigFile(filepath, properties);
}
int main(void) 
{
	const char* src_dir = "./config/demokit_config.json"; 
	const char* dist_dir = "./config/demokit_config2.json"; 
	loadConfig((char*)src_dir);
	saveConfig((char*)dist_dir);
	
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