//
// Created by Spud on 4/23/2021.
//

#include "FoxWebhook.hpp"
#include "rapidjson/error/en.h"

std::unordered_map<std::string, std::string> loadKeys(const rapidjson::Value &json) {

	// Get the logger.
	std::shared_ptr<spdlog::logger> logger = spdlog::get("Logger");

	// Create a hash map to store the keys for various services.
	std::unordered_map<std::string, std::string> hashmap;

	// Make sure the document has the key member.
	if (!json.HasMember(FOXWEBHOOK_KEYS)) {
		logger->warn("Config is missing 'Keys' for services");
		return hashmap;
	}

	// Load the keys from the object.
	rapidjson::GenericObject keys = json[FOXWEBHOOK_KEYS].GetObj();

	// Get the tumblr API key.
	if (keys.HasMember(SERVICE_TUMBLR)) {
		hashmap[SERVICE_TUMBLR] = keys[SERVICE_TUMBLR].GetString();
		logger->info("Loaded tumblr API key");
	}

	// TODO Add check for other services.

	// Return the hashmap.
	return hashmap;
}

int parseJSON(const std::string &json, std::vector<FoxWebhook> &foxWebhooks) {

	// Get the logger.
	std::shared_ptr<spdlog::logger> logger = spdlog::get("Logger");

	// Create a document object to parse the json.
	rapidjson::Document document;
	document.Parse(json);

	if (document.HasParseError()) {
		logger->error(fmt::format("Unable to parse file: {}", rapidjson::GetParseError_En(document.GetParseError())));
		return 3;
	}

	// Load API keys for services used by the foxWebhooks.
	std::unordered_map<std::string, std::string> keys = loadKeys(document);

	// Check for missing keys.
	if (keys.empty()) {
		logger->error("No API keys were found in the config file.");
		return 4;
	}

	// Check that the json object has an element named "Webhooks".
	if (!document.HasMember(FOXWEBHOOK_WEBHOOKS)) {
		logger->error("Config is missing 'Webhooks' array.");
		return 5;
	}

	// Make sure the json object named "Webhooks" is a json array type.
	if (!document[FOXWEBHOOK_WEBHOOKS].IsArray()) {
		logger->error("Webhook is not an array type.");
		return 6;
	}

	// Try to get the json array of webhook information.
	rapidjson::GenericArray webhooks = document[FOXWEBHOOK_WEBHOOKS].GetArray();

	// Iterate though each entry in the foxWebhooks json array.
	for (auto &webhookEntry : webhooks) {

		// If the webhookEntry is not an object, skip it.
		if (!webhookEntry.IsObject()) {
			logger->warn("Entry is not an object.");
			continue;
		}

		std::string retrieveFrom;
		if (webhookEntry.HasMember(FOXWEBHOOK_RETRIEVE_FROM)) {
			if (webhookEntry[FOXWEBHOOK_RETRIEVE_FROM].IsString()) {
				retrieveFrom = webhookEntry[FOXWEBHOOK_RETRIEVE_FROM].GetString();
			}
		}

		std::string sendTo;
		if (webhookEntry.HasMember(FOXWEBHOOK_SEND_TO)) {
			if (webhookEntry[FOXWEBHOOK_SEND_TO].IsString()) {
				sendTo = webhookEntry[FOXWEBHOOK_SEND_TO].GetString();
			}
		}

		std::string keyToUse;
		if (webhookEntry.HasMember(FOXWEBHOOK_KEY)) {
			if (webhookEntry[FOXWEBHOOK_KEY].IsString()) {
				keyToUse = webhookEntry[FOXWEBHOOK_KEY].GetString();
			}
		}

		// Check if the key is in the hash map.
		auto keyLocation = keys.find(keyToUse);
		if (keyLocation == keys.end()) {
			logger->warn(fmt::format("Key to use for {} was not found", retrieveFrom));
			continue;
		}

		FoxWebhook foxWebhook = FoxWebhook(retrieveFrom, keyLocation->second, sendTo);
		foxWebhooks.push_back(foxWebhook);
	}

	return 0;
}

int FoxWebhook::loadFromConfig(const std::string &filePath, std::vector<FoxWebhook> &foxWebhooks) {

	std::shared_ptr<spdlog::logger> logger = spdlog::get("Logger");

	// Try to open the file at the filePath location.
	std::ifstream file(filePath);

	// Check if we were successfully able to open the file at this point.
	if (!file.is_open()) {

		// Log that we were unable to open the file successfully, and return false (error).
		logger->error("Unable to open file at " + filePath);
		return 1;
	}

	// Load the content of the file into the string.
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	// Try parsing the config file into our buffer of FoxWebhooks.
	try {
		return parseJSON(buffer.str(), foxWebhooks);
	} catch (const std::exception& exception) {
		logger->error(fmt::format("Error occurred in parsing the config file: {}", exception.what()));
		return 2;
	}
}

int FoxWebhook::loadFromConfig(std::vector<FoxWebhook> &foxWebhooks) {

	// Get the logger for our script.
	std::shared_ptr<spdlog::logger> logger = spdlog::get("Logger");

	// If the first read was successful return.
	logger->info("Attempting to read config from parent directory...");
	if (FoxWebhook::loadFromConfig("../config.json", foxWebhooks) == 0) {
		return 0;
	}

	// If we were unsuccessful, try reading from a different (but still common) location.
	logger->info("Attempting to read config from current directory...");
	if (FoxWebhook::loadFromConfig("config.json", foxWebhooks) == 0) {
		return 0;
	}

	// If we've made it this far, return an error.
	logger->error("Unable to load from config file.");
	return 3;
}