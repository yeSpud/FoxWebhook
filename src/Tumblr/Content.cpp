//
// Created by Spud on 4/24/2021.
//

#include "Content.hpp"

bool Content::entryHasString(const rapidjson::Value &entry, const char *value, std::string &buffer) {
	if (entry.HasMember(value)) {
		buffer = entry[value].GetString();
		return true;
	} else {
		return false;
	}
}

bool Content::entryHasInt(const rapidjson::Value &entry, const char *value, unsigned &buffer) {
	if (entry.HasMember(value)) {
		buffer = entry[value].GetUint();
		return true;
	} else {
		return false;
	}
}
