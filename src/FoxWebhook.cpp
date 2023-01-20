//
// Created by Spud on 4/23/2021.
//

#include "FoxWebhook.hpp"
#include "rapidjson/document.h"

int FoxWebhook::parseJSON(const std::string &json, std::vector<FoxWebhook> &webhooks) {

	// Get the logger.
	std::shared_ptr<spdlog::logger> logger = spdlog::get("Logger");

	// Create a document object to parse the json.
	rapidjson::Document document;

	// Parse the json string into the document.
	document.Parse(json.c_str());

	// Check that the parsed json is a valid object.
	if (!document.IsObject()) {
		logger->error("Config is in an invalid format (not a json object).");
		return ErrorCodes::INVALID_JSON_FORMAT;
	}

	// Load API keys for services used by the webhooks.
	std::unordered_map<std::string, std::string> keys = loadKeys(document);

	// Check for missing keys.
	if (keys.empty()) {
		logger->error("No API keys in config file.");
		return ErrorCodes::MISSING_KEYS;
	}

	// Check that the json object has an element named "Webhooks".
	if (!document.HasMember(WEBHOOKS)) {
		logger->error("Config is missing 'Webhooks' array.");
		return ErrorCodes::MISSING_WEBHOOK;
	}

	// Make sure the json object named "Webhooks" is a json array type.
	if (!document[WEBHOOKS].IsArray()) {
		logger->error("Webhook is not an array type.");
		return ErrorCodes::WEBHOOK_NOT_ARRAY;
	}

	// Try to get the json array of webhook information.
	rapidjson::GenericArray<false, rapidjson::Value> entries = document[WEBHOOKS].GetArray();

	// Iterate though each entry in the webhooks json array.
	for (rapidjson::SizeType i = 0; i < entries.Size(); i++) {

		// If the entry is not an object, skip it.
		if (!entries[i].IsObject()) {
			logger->warn("Entry is not an object.");
			continue;
		}

		// Get the current webhook entry object.
		rapidjson::GenericObject<false, rapidjson::Value> entry = entries[i].GetObj();

		// Get the fox webhook from the json object.
		std::string retrieveFrom, sendTo, apiKey;
		int webhookStatus = loadFoxWebhook(entry, keys, retrieveFrom, sendTo, apiKey);

		// Make sure the webhook was loaded successfully before adding it to the vector.
		switch(webhookStatus){
			case 0: {

				DiscordWebhook discordWebhook = DiscordWebhook(sendTo);

				// Initialize the webhook.
				FoxWebhook foxWebhook = FoxWebhook(retrieveFrom, apiKey, discordWebhook);
				webhooks.push_back(foxWebhook);
				break;
			}
			case ErrorCodes::MISSING_RETRIEVE_FROM: {
				// Log missing retrieve from.
				logger->warn(fmt::format("Missing {} value.", RETRIEVE_FROM));
				break;
			}
			case ErrorCodes::MISSING_SEND_TO: {
				// Log missing send to.
				logger->warn(fmt::format("Missing {} value.", SEND_TO));
				break;
			}
			case ErrorCodes::MISSING_KEY: {
				// Log missing key.
				logger->warn(fmt::format("Missing {} value.", KEY));
				break;
			}
			default: {
				// Log general error.
				logger->warn(fmt::format("Unable to load webhook. Returned value {}", webhookStatus));
				break;
			}
		}
	}

	// Return success.
	return 0;
}

std::unordered_map<std::string, std::string> FoxWebhook::loadKeys(const rapidjson::Document &document) {

	// Get the logger.
	std::shared_ptr<spdlog::logger> logger = spdlog::get("Logger");

	// Create a hash map to store the keys for various services.
	std::unordered_map<std::string, std::string> hashmap;

	// Make sure the document has the key member.
	if (!document.HasMember(KEYS)) {
		logger->warn("Config is missing 'Keys' for services");
		return hashmap;
	}

	// Load the keys from the object.
	rapidjson::GenericObject<true, rapidjson::GenericValue<rapidjson::UTF8<>>::ValueType> keys = document[KEYS].GetObj();

	// Get the tumblr API key.
	if (keys.HasMember(SERVICE_TUMBLR)) {
		logger->info("Loaded tumblr API key");
		hashmap[SERVICE_TUMBLR] = keys[SERVICE_TUMBLR].GetString();
	}

	// TODO Add check for other services.

	// Return the hashmap.
	return hashmap;
}

int FoxWebhook::loadFoxWebhook(const rapidjson::GenericObject<false, rapidjson::Value>& jsonObject, const std::unordered_map<std::string, std::string> &keysMap,
							   std::string &retrieveFrom, std::string &sendTo, std::string &apiKey) {

	// Check if the json object has the retrieve from entry.
	if (!jsonObject.HasMember(RETRIEVE_FROM)) {
		return ErrorCodes::MISSING_RETRIEVE_FROM;
	}

	// Make sure that the retrieve from entry is a string type.
	if (!jsonObject[RETRIEVE_FROM].IsString()) {
		return ErrorCodes::MISSING_RETRIEVE_FROM;
	}

	// Get the retrieve from entry.
	retrieveFrom = jsonObject[RETRIEVE_FROM].GetString();

	// Check if the json object has the send-to entry.
	if (!jsonObject.HasMember(SEND_TO)) {
		return ErrorCodes::MISSING_SEND_TO;
	}

	// Make sure that the send-to entry is a string type.
	if (!jsonObject[SEND_TO].IsString()) {
		return ErrorCodes::MISSING_SEND_TO;
	}

	// Get the send-to entry.
	sendTo = jsonObject[SEND_TO].GetString();

	// Check if the json object as a key value.
	if (!jsonObject.HasMember(KEY)) {
		return ErrorCodes::MISSING_KEY;
	}

	// Check if the key is a string type.
	if (!jsonObject[KEY].IsString()) {
		return ErrorCodes::MISSING_KEY;
	}

	// Check if the key is in the hash map. TODO Move this away from auto.
	auto mapIndex = keysMap.find(jsonObject[KEY].GetString());
	if (mapIndex == keysMap.end()) {
		return ErrorCodes::MISSING_KEY;
	}

	// Get the key value.
	apiKey = mapIndex->second;

	// Return success.
	return 0;
}

int FoxWebhook::loadFromConfig(const std::string &filePath, std::vector<FoxWebhook> &foxWebhooks) {

	// Create a buffer string object to have the json stored into.
	std::string json;

	// Try reading the Image of the file into the string.
	if (!FoxWebhook::readFromFile(filePath, json)) {
		spdlog::get("Logger")->warn("Unable to read config file");
		return ErrorCodes::CANNOT_READ;
	}

	// Try parsing the config file into our buffer of webhooks.
	return FoxWebhook::parseJSON(json, foxWebhooks);
}

int FoxWebhook::loadFromConfig(std::vector<FoxWebhook> &foxWebhooks) {

	// Get the logger for our script.
	std::shared_ptr<spdlog::logger> logger = spdlog::get("Logger");

	// Get the load status from the file.
	int status;

	// If the first read was successful return.
	logger->info("Attempting to read config from parent directory...");
	status = FoxWebhook::loadFromConfig("../config.json", foxWebhooks);
	if (status == 0) {
		return 0;
	}

	// If we were unsuccessful, try reading from a different (but still common) location.
	logger->info("Attempting to read config from current directory...");
	status = FoxWebhook::loadFromConfig("config.json", foxWebhooks);
	if (status == 0) {
		return 0;
	}

	// If we've made it this far, return an error.
	return status;
}

bool FoxWebhook::readFromFile(const std::string &filePath, std::string &json) {

	// Try to open the file at the filePath location.
	std::fstream file;
	file.open(filePath, std::ios::in);

	// Check if we were successfully able to open the file at this point.
	if (!file.is_open()) {

		// Log that we were unable to open the file successfully, and return false (error).
		spdlog::get("Logger")->error("Unable to open file at " + filePath);
		return false;
	}

	// Load the content of the file into the string.
	json.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

	// Return success.
	return true;
}