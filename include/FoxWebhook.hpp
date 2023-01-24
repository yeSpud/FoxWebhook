//
// Created by Spud on 4/23/2021.
//

#ifndef FOXWEBHOOK_FOXWEBHOOK_HPP
#define FOXWEBHOOK_FOXWEBHOOK_HPP

#include <fstream>
#include <utility>
#include "DiscordWebhook.hpp"
#include "spdlog/spdlog.h"

#define FOXWEBHOOK_KEYS "Keys"
#define FOXWEBHOOK_WEBHOOKS "Webhooks"
#define FOXWEBHOOK_RETRIEVE_FROM "Retrieve-From"
#define FOXWEBHOOK_SEND_TO "Send-To"
#define FOXWEBHOOK_KEY "Service-Key"

/**
 * CURRENTLY SUPPORTED SERVICES
 */
#define SERVICE_TUMBLR "Tumblr"

class FoxWebhook {

public:

	/**
	 * Manually create a FoxWebhook object with the blog string, the api key, and the url for the webhook.
	 */
	FoxWebhook(std::string blog, std::string key, std::string webhookUrl) : blog(std::move(blog)), key(std::move(key)),
	                                                                        discordWebhook(DiscordWebhook(std::move(webhookUrl))) {};

	/**
	 * Manually create a FoxWebhook object with the blog string, the api key, and a DiscordWebhook object.
	 */
	FoxWebhook(std::string blog, std::string key, DiscordWebhook webhook) : blog(std::move(blog)), key(std::move(key)),
	                                                                        discordWebhook(std::move(webhook)) {};

	FoxWebhook() = delete;

	FoxWebhook(const FoxWebhook &copy) : blog(copy.blog), key(copy.key), discordWebhook(copy.discordWebhook) {

		// RTFM: https://rapidjson.org/md_doc_tutorial.html#DeepCopyValue
		rapidjson::Document d;
		rapidjson::Document::AllocatorType &a = d.GetAllocator();
		previousPost.CopyFrom(copy.previousPost, a);
	}
	FoxWebhook &operator=(const FoxWebhook &rhs) = delete;


	FoxWebhook(FoxWebhook &&rhs) noexcept: blog(rhs.blog), key(rhs.key), discordWebhook(rhs.discordWebhook) {
		previousPost.Swap(rhs.previousPost);
	}
	FoxWebhook &operator=(FoxWebhook &&rhs) = delete;

	~FoxWebhook() = default;

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
	const DiscordWebhook discordWebhook;

	/**
	 * The previous post corresponding to this entry.
	 */
	rapidjson::Value previousPost;

	/**
	 * Loads FoxWebhooks from the config file.
	 * The config file is assumed to be in either the parent directory, or the current working directory if that should fail.
	 */
	static int loadFromConfig(std::vector<FoxWebhook> &foxWebhooks);

	/**
	 * Loads FoxWebhooks from a config file specified by a file path.
	 */
	static int loadFromConfig(const std::string &filePath, std::vector<FoxWebhook> &foxWebhooks);

};

#endif //FOXWEBHOOK_FOXWEBHOOK_HPP
