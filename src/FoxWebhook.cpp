//
// Created by Spud on 4/23/2021.
//

#include "FoxWebhook.hpp"
#include "rapidjson/document.h"

int FoxWebhook::parseJSON(const std::string &json, std::vector<FoxWebhook> &webhooks) {

	// Get our logger.
	std::shared_ptr<spdlog::logger> logger = spdlog::get("Logger");

	// Create a document object to parse the json.
	rapidjson::Document document;
	//rapidjson::Document document;

	// Parse the json string into the document.
	document.Parse(json.c_str());

	// Check that the parsed json is a valid object.
	if (!document.IsObject()) {
		logger->warn("Config is in an invalid format (not a json object)");
		return ErrorCodes::INVALID_JSON_FORMAT;
	}

	// Check that the json object has an element named "Webhooks".
	if (!document.HasMember("Webhooks")) {
		logger->warn("Config is missing 'Webhooks' array");
		return ErrorCodes::MISSING_WEBHOOK;
	}

	// Try to get the json array of webhook information.
	JSON_ARRAY entries = document["Webhooks"].GetArray();

	// Iterate though each entry in the webhooks json array.
	for (rapidjson::SizeType i = 0; i < entries.Size(); i++) {

		// If the entry is not an object, skip it.
		if (!entries[i].IsObject()) {
			logger->warn("Entry is not an object");
			continue;
		}

		// Get the current entry object.
		JSON_OBJECT entry = entries[i].GetObj();

		// Try parsing the 3 main variables that we care about (webhook, blog, auth).
		std::string webhook, blog, auth;
		blog = entry["BlogURL"].GetString();
		auth = entry["Auth"].GetString();
		webhook = entry["WebhookURL"].GetString();

		// Create a new TumblrAPI object from the blog and auth variables.
		if (auth.empty() || blog.empty()) {
			logger->warn("Cannot create Tumblr API class");
			continue;
		}
		TumblrAPI tumblrApi = TumblrAPI(auth);

		// Create a new discordWebhook object from the webhook url.
		if (webhook.empty()) {
			logger->warn("Cannot create discord webhook class");
			continue;
		}
		DiscordWebhook discordWebhook = DiscordWebhook(webhook);

		// Create a new FoxWebhook and add the webhook to the vector.
		FoxWebhook foxWebhook = FoxWebhook(blog, tumblrApi, discordWebhook);
		webhooks.push_back(foxWebhook);
	}

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
