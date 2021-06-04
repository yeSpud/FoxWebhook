//
// Created by Spud on 4/23/2021.
//

#include "FoxWebhook.hpp"
#include "ErrorCodes.hpp"
#include <spdlog/spdlog.h>

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

int FoxWebhook::parseJSON(const std::string &json, std::vector<FoxWebhook> &webhooks) {

	// Get our logger.
	std::shared_ptr<spdlog::logger> logger = spdlog::get("Logger");

	// Create a document object to parse the json.
	rapidjson::Document document;

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
	const rapidjson::Value &entries = document["Webhooks"];

	// Make the entries is a json array.
	if (!entries.IsArray()) {
		logger->warn("'Webhooks' is not an array");
		return ErrorCodes::WEBHOOK_NOT_ARRAY;
	}

	// Iterate though each entry in the webhooks json array.
	for (rapidjson::SizeType i = 0; i < entries.Size(); i++) {

		// If the entry is not an object, skip it.
		if (!entries[i].IsObject()) {
			logger->warn("Entry is not an object");
			continue;
		}

		// Get the current entry object.
		auto entry = entries[i].GetObj();

		// Try parsing the 3 main variables that we care about (webhook, blog, auth).
		std::string webhook, blog, auth;
		blog = parseEntry(entry, "BlogURL");
		auth = parseEntry(entry, "Auth");
		webhook = parseEntry(entry, "WebhookURL");

		// Create a new TumblrAPI object from the blog and auth variables.
		if (auth.empty() || blog.empty()) {
			logger->warn("Cannot create Tumblr API class");
			continue;
		}
		TumblrAPI tumblrApi = TumblrAPI(auth, blog);

		// Create a new discordWebhook object from the webhook url.
		if (webhook.empty()) {
			logger->warn("Cannot create discord webhook class");
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
		spdlog::get("Logger")->warn("Missing entry '" + entryValue + "'");
		return "";
	}
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
