//
// Created by Spud on 4/23/2021.
//

#include "FoxWebhook.hpp"
#include "ErrorCodes.hpp"
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

	// Load the Image of the file into the string.
	json.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

	// Return success.
	return true;
}

int FoxWebhook::parseJSON(const std::string &json, std::vector<FoxWebhook> &webhooks) {

	// Create a document object to parse the json.
	rapidjson::Document document;

	// Parse the json string into the document.
	document.Parse(json.c_str());

	// Check that the parsed json is a valid object.
	if (!document.IsObject()) {
		std::cout << "Config is in an invalid format (not a json object)" << std::endl;
		return ErrorCodes::INVALID_JSON_FORMAT;
	}

	// Check that the json object has an element named "Webhooks".
	if (!document.HasMember("Webhooks")) {
		std::cout << "Config is missing 'Webhooks' array" << std::endl;
		return ErrorCodes::MISSING_WEBHOOK;
	}

	// Try to get the json array of webhook information.
	const rapidjson::Value &entries = document["Webhooks"];

	// Make the entries is a json array.
	if (!entries.IsArray()) {
		std::cout << "'Webhooks' is not an array" << std::endl;
		return ErrorCodes::WEBHOOK_NOT_ARRAY;
	}

	// Iterate though each entry in the webhooks json array.
	for (rapidjson::SizeType i = 0; i < entries.Size(); i++) {

		// If the entry is not an object, skip it.
		if (!entries[i].IsObject()) {
			std::cout << "Entry is not an object" << std::endl;
			continue;
		}

		// Get the current entry object.
		rapidjson::GenericObject entry = entries[i].GetObj();

		// Try parsing the 3 main variables that we care about (webhook, blog, auth).
		std::string webhook, blog, auth;
		blog = parseEntry(entry, "BlogURL");
		auth = parseEntry(entry, "Auth");
		webhook = parseEntry(entry, "WebhookURL");

		// Create a new TumblrAPI object from the blog and auth variables.
		if (auth.empty() || blog.empty()) {
			std::cout << "Cannot create Tumblr API class" << std::endl;
			continue;
		}
		TumblrAPI tumblrApi = TumblrAPI(auth, blog);

		// Create a new discordWebhook object from the webhook url.
		if (webhook.empty()) {
			std::cout << "Cannot create discord webhook class" << std::endl;
			continue;
		}
		DiscordWebhook discordWebhook = DiscordWebhook(webhook);


		// Create a new FoxWebhook and add the webhook to the vector.
		FoxWebhook foxWebhook = FoxWebhook(tumblrApi, discordWebhook);
		webhooks.push_back(foxWebhook);
	}

	// Return success.
	return 0;
}

std::string FoxWebhook::parseEntry(const rapidjson::Value &entry, const std::string &entryValue) { // TODO Comments
	if (entry.HasMember(entryValue.c_str())) {
		return entry[entryValue.c_str()].GetString();
	} else {
		std::cout << "Missing entry '" << entryValue << '\'' << std::endl;
		return "";
	}
}

int FoxWebhook::loadFromConfig(const std::string &filePath, std::vector<FoxWebhook> &foxWebhooks) {

	// Create a buffer string object to have the json stored into.
	std::string json;

	// Try reading the Image of the file into the string.
	if (!FoxWebhook::readFromFile(filePath, json)) {
		std::cout << "Unable to read config file" << std::endl;
		return ErrorCodes::CANNOT_READ;
	}

	// Try parsing the config file into our buffer of webhooks.
	return FoxWebhook::parseJSON(json, foxWebhooks);
}

int FoxWebhook::loadFromConfig(std::vector<FoxWebhook> &foxWebhooks) {

	// Get the load status from the file.
	int status;

	// If the first read was successful return.
	status = FoxWebhook::loadFromConfig("../config.json", foxWebhooks);
	if (status == 0) {
		return 0;
	}

	// If we were unsuccessful, try reading from a different (but still common) location.
	status = FoxWebhook::loadFromConfig("config.json", foxWebhooks);
	if (status == 0) {
		return 0;
	}

	// If we've made it this far, return an error.
	return status;
}
