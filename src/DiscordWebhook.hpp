//
// Created by Spud on 4/24/2021.
//

#ifndef FOXWEBHOOK_DISCORDWEBHOOK_HPP
#define FOXWEBHOOK_DISCORDWEBHOOK_HPP

#include <cpr/cpr.h>
#include "rapidjson/document.h"

/**
 * TODO Documentation
 */
class DiscordWebhook {

private:

	/**
	 * TODO Documentation
	 */
	const std::string webhookURL;

	/**
	 * TODO Documentation
	 * @param json
	 * @return
	 */
	cpr::Response sendWebhook(const std::string &json);

	/**
	 * TODO Documentation
	 * @param message
	 * @param blogname
	 * @param postURL
	 * @param blogAvatar
	 * @param postImageURL
	 * @return
	 */
	static std::string formatJson(const std::string &message, const std::string &blogname,
	                              const std::string &postURL, const std::string &blogAvatar,
	                              const std::string &postImageURL);

	/**
	 * TODO Documentation
	 * @param key
	 * @param value
	 * @param object
	 * @param allocator
	 */
	static void setStringKeyValueObject(const std::string &key, const std::string &value, rapidjson::Value &object,
	                                    rapidjson::MemoryPoolAllocator<> &allocator);

public:

	/**
	 * TODO Documentation
	 * @param webhookURL
	 */
	explicit DiscordWebhook(std::string webhookURL) : webhookURL(std::move(webhookURL)) {};

	/**
	 * TODO Documentation
	 * @param message
	 * @return
	 */
	cpr::Response sendMessage(const std::string &message);

	/**
	 * TODO Documentation
	 * @param blogname
	 * @param postURL
	 * @param blogAvatar
	 * @param postImageURL
	 * @return
	 */
	cpr::Response sendEmbed(const std::string &blogname, const std::string &postURL, const std::string &blogAvatar,
	                        const std::string &postImageURL);

};


#endif //FOXWEBHOOK_DISCORDWEBHOOK_HPP
