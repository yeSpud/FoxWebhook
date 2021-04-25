//
// Created by Spud on 4/24/2021.
//

#include "Image.hpp"

bool Image::entryHasString(const rapidjson::Value &entry, const char *value, std::string &buffer) {
	if (entry.HasMember(value)) {
		buffer = entry[value].GetString();
		return true;
	} else {
		return false;
	}
}

bool Image::entryHasInt(const rapidjson::Value &entry, const char *value, unsigned &buffer) {
	if (entry.HasMember(value)) {
		buffer = entry[value].GetUint();
		return true;
	} else {
		return false;
	}
}
