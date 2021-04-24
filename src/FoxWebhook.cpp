//
// Created by Spud on 4/23/2021.
//

#include "FoxWebhook.hpp"
#include <fstream>
#include <iostream>

bool FoxWebhook::readFromFile(const std::string &filePath, std::string &json) {

	// Try to open the file at the filePath location.
	std::fstream file;
	file.open(filePath, std::ios::in);

	// Check if we were successfully able to open the file at this point.
	if (!file.is_open()) {

		// Print that we were unable to open the file successfully, and return false (error).
		std::cout << "Unable to open file at " << filePath << std::endl;
		return false;
	}

	// Load the content of the file into the string.
	json.assign(std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>());
	//std::cout << "Content: " << json << std::endl;

	// Return success.
	return true;
}

int FoxWebhook::parseJSON(const std::string& json, std::vector<FoxWebhook>& webhooks) {

	// Create a document object to parse the json.
	rapidjson::Document document;

	// Parse the json string into the document.
	document.Parse(json.c_str());

	// Check that the parsed json is a valid object.
	if (!document.IsObject()) {
		std::cout << "Config is in an invalid format (not a json object)" << std::endl;
		return -1;
	}

	// Check that the json object has an element named "Webhooks".
	if (!document.HasMember("Webhooks")) {
		std::cout << "Config is missing 'Webhooks' array" << std::endl;
		return -2;
	}

	// Try to get the json array of webhook information.
	const rapidjson::Value& entries = document["Webhooks"];

	// Make the entries is a json array.
	if (!entries.IsArray()) {
		std::cout << "'Webhooks' is not an array" << std::endl;
		return -3;
	}

	// Iterate though each entry in the webhooks json array.
	for (rapidjson::SizeType i = 0; i < entries.Size(); i++) {

		// If the entry is not an object, skip it.
		if (!entries[i].IsObject()) {

			std::cout << "Entry is not an object" << std::endl;
			continue;
		}

		auto entry = entries[i].GetObject();

		// Try parsing the 3 main variables that we care about (webhook, blog, auth).
		std::string webhook, blog, auth;

		webhook = parseEntry(entry, "WebhookURL");
		blog = parseEntry(entry, "BlogURL");
		auth = parseEntry(entry, "Auth");

		// Create a new FoxWebhook object with these values.
		FoxWebhook foxWebhook(webhook, blog, auth);

		// Add the webhook to the vector.
		webhooks.push_back(foxWebhook);
	}

	// Return success.
	return 0;
}

std::string FoxWebhook::parseEntry(const rapidjson::Value& entry, const std::string& entryValue) {
	if (entry.HasMember(entryValue.c_str())) {
		return entry[entryValue.c_str()].GetString();
	} else {
		std::cout << "Missing entry '" << entryValue << '\'' << std::endl;
		return "";
	}
}

std::vector<FoxWebhook> FoxWebhook::loadFromConfig(const std::string& filePath) {

	// Create a buffer vector to store our webhooks into.
	std::vector<FoxWebhook> webhooks;

	// Create a buffer string object to have the json stored into.
	std::string json;

	// Try reading the content of the file into the string.
	// If there was an error, return the empty vector.
	if (!FoxWebhook::readFromFile(filePath, json)) {
		std::cout << "Unable to read config file" << std::endl;
		return webhooks;
	}

	// Try parsing the config file into our buffer of webhooks.
	if (FoxWebhook::parseJSON(json, webhooks) != 0) {

		// Because we were unable to parse the webhooks, clear the vector and return it empty (as the entries could be bad).
		std::cout << "Unable to parse webhooks" << std::endl;
		webhooks.clear();
		return webhooks;
	}

	// Return our vector of webhooks.
	return webhooks;
}
