//
// Created by Spud on 4/23/2021.
//

#ifndef FOXWEBHOOK_FOXWEBHOOK_HPP
#define FOXWEBHOOK_FOXWEBHOOK_HPP

#include <fstream>
#include "DiscordWebhook.hpp"
#include "ErrorCodes.hpp"
#include "TumblrAPI.hpp"
#include "post.hpp"
#include "spdlog/spdlog.h"

/**
 * TODO Documentation
 */
class FoxWebhook {

public:

	/**
	 * TODO Documentation
	 * @param blog
	 * @param tumblrAPI
	 * @param discordWebhook
	 */
	FoxWebhook(std::string blog, TumblrAPI tumblrAPI, DiscordWebhook discordWebhook) : blog(std::move(blog)),
	tumblrApi(std::move(tumblrAPI)), discordWebhook(std::move(discordWebhook)) {};

	/**
	 * TODO Documentation
	 */
	const std::string blog;

	/**
	 * TODO Documentation
	 */
	TumblrAPI tumblrApi;

	/**
	 * TODO Documentation
	 */
	DiscordWebhook discordWebhook;

	/**
	 * TODO Documentation
	 */
	std::shared_ptr<Post> previousPost = nullptr;

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
	 * TODO Documentation
	 * @param jsonObject
	 * @param keysMap
	 * @param foxWebhook
	 * @return
	 */
	static int loadFoxWebhook(const JSON_OBJECT &jsonObject, const std::unordered_map<std::string, std::string> &keysMap, FoxWebhook** foxWebhook);
};

#endif //FOXWEBHOOK_FOXWEBHOOK_HPP
