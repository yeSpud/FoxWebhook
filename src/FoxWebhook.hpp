//
// Created by Spud on 4/23/2021.
//

#ifndef FOXWEBHOOK_FOXWEBHOOK_HPP
#define FOXWEBHOOK_FOXWEBHOOK_HPP

#include "Tumblr/TumblrAPI.hpp"
#include "DiscordWebhook.hpp"

class FoxWebhook {

public:

	/**
	 * TODO Documentation
	 * @param tumblrApi
	 * @param discordWebhook
	 */
	FoxWebhook(TumblrAPI tumblrApi, DiscordWebhook discordWebhook):tumblrApi(std::move(tumblrApi)), discordWebhook(std::move(discordWebhook)) {};


	/**
	 * TODO Documentation
	 * @return
	 */
	TumblrAPI getTumblrAPI() { return tumblrApi; };

	/**
	 * TODO Documentation
	 * @return
	 */
	DiscordWebhook getDiscordWebhook() { return discordWebhook; };

	/**
	 * TODO Documentation
	 * @return
	 */
	//Post getPreviousPost() { return previousPost;};
	Post previousPost = Post();
	//void setPreviousPost(Post post) {previousPost = std::move(post);};

	/**
	 * TODO Documentation
	 * @return
	 */
	static int loadFromConfig(std::vector<FoxWebhook> &foxWebhooks) {
		return FoxWebhook::loadFromConfig("../config.json", foxWebhooks);
	};

	/**
	 * TODO Documentation
	 * @param filePath
	 * @return
	 */
	static int loadFromConfig(const std::string &filePath, std::vector<FoxWebhook> &foxWebhooks);

private:

	/**
	 * TODO Documentation
	 */
	TumblrAPI tumblrApi;

	/**
	 * TODO Documentation
	 */
	DiscordWebhook discordWebhook;

	/**
	 * TODO Documentation.
	 */
	//Post previousPost = Post("");

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

};

#endif //FOXWEBHOOK_FOXWEBHOOK_HPP
