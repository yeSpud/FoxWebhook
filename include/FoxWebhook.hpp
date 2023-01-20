//
// Created by Spud on 4/23/2021.
//

#ifndef FOXWEBHOOK_FOXWEBHOOK_HPP
#define FOXWEBHOOK_FOXWEBHOOK_HPP

#include <fstream>
#include "DiscordWebhook.hpp"
#include "ErrorCodes.hpp"
#include "spdlog/spdlog.h"

class FoxWebhook {

public:

	/**
	 * TODO Documentation
	 * @param blog
	 * @param tumblrAPI
	 * @param discordWebhook
	 */
	FoxWebhook(std::string blog, std::string key, DiscordWebhook discordWebhook) : blog(std::move(blog)),
	key(std::move(key)),discordWebhook(std::move(discordWebhook)) {};

	/**
	 * The blog's url.
	 */
	const std::string blog;

	/**
	 * The key used by the relevant api.
	 */
	const std::string key;

	/**
	 * The discord webhook object used to send messages to the discord channel.
	 */
	DiscordWebhook discordWebhook;

	/**
	 * TODO Documentation
	 * @param foxWebhooks
	 * @return
	 */
	static int loadFromConfig(std::vector<FoxWebhook> &foxWebhooks);

	/**
	 * TODO Documentation
	 * @param filePath
	 * @param foxWebhooks
	 * @return
	 */
	static int loadFromConfig(const std::string &filePath, std::vector<FoxWebhook> &foxWebhooks);

private:

	inline static const char* KEYS = "Keys";

	inline static const char* WEBHOOKS = "Webhooks";

	inline static const char* RETRIEVE_FROM = "Retrieve-From";

	inline static const char* SEND_TO = "Send-To";

	inline static const char* KEY = "Service-Key";

	inline static const char* SERVICE_TUMBLR = "Tumblr";

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
	 * @param document
	 * @return
	 */
	static std::unordered_map<std::string, std::string> loadKeys(const rapidjson::Document &document);

	/**
	 *
	 * @param jsonObject
	 * @param keysMap
	 * @param retrieveFrom
	 * @param sendTo
	 * @param apiKey
	 * @return
	 */
	static int loadFoxWebhook(const rapidjson::GenericObject<false, rapidjson::Value> &jsonObject,
							  const std::unordered_map<std::string,std::string> &keysMap,std::string &retrieveFrom,
							  std::string &sendTo, std::string &apiKey);
};

#endif //FOXWEBHOOK_FOXWEBHOOK_HPP
