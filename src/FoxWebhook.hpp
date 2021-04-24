//
// Created by Spud on 4/23/2021.
//

#ifndef FOXWEBHOOK_FOXWEBHOOK_HPP
#define FOXWEBHOOK_FOXWEBHOOK_HPP

#include <string>
#include <utility>
#include <vector>
#include "rapidjson/document.h"

class FoxWebhook {

public:

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getWebhookURL() {return webhookURL;}

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getBlogURL() {return blogURL;}

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getAuth() {return authToken;}

	/**
	 * TODO Documentation
	 * @return
	 */
	static std::vector<FoxWebhook> loadFromConfig() { return FoxWebhook::loadFromConfig("../config.json"); };

	/**
	 * TODO Documentation
	 * @param filePath
	 * @return
	 */
	static std::vector<FoxWebhook> loadFromConfig(const std::string &filePath);

private:

	/**
	 * TODO Documentation
	 */
	const std::string webhookURL;

	/**
	 * TODO Documentation
	 */
	const std::string blogURL;

	/**
	 * TODO Documentation
	 */
	const std::string authToken;

	/**
	 * TODO Documentation
	 * @param filePath
	 * @param json
	 * @return
	 */
	static bool readFromFile(const std::string &filePath, std::string &json);

	/**
	 * TODO Documentation
	 * @param json
	 * @param webhooks
	 * @return
	 */
	static int parseJSON(const std::string &json, std::vector<FoxWebhook> &webhooks);

	/**
	 * TODO Documentation
	 * @param entry
	 * @param entryValue
	 * @return
	 */
	static std::string parseEntry(const rapidjson::Value &entry, const std::string &entryValue);

	/**
	 * TODO Documentation
	 * @param webhookURL
	 * @param blogURL
	 * @param authToken
	 */
	FoxWebhook(std::string webhookURL, std::string blogURL, std::string authToken) : webhookURL(std::move(webhookURL)),
	                                                                                 blogURL(std::move(blogURL)),
	                                                                                 authToken(std::move(authToken)) {};
};


#endif //FOXWEBHOOK_FOXWEBHOOK_HPP
