//
// Created by Spud on 4/23/2021.
//

#ifndef FOXWEBHOOK_FOXWEBHOOK_HPP
#define FOXWEBHOOK_FOXWEBHOOK_HPP

#include <fstream>
#include "DiscordWebhook.hpp"
#include "ErrorCodes.hpp"
#include "TumblrAPI.h"
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
	                                                                                   tumblrApi(std::move(tumblrAPI)),
	                                                                                   discordWebhook(std::move(
			                                                                                   discordWebhook)) {};

	/**
	 * TODO Documentation
	 */
	const std::string blog;

	/**
	 * TODO Documentation
	 */
	const TumblrAPI tumblrApi;

	/**
	 * TODO Documentation
	 */
	const DiscordWebhook discordWebhook;

	/**
	 * TODO Documentation
	 */
	Post previousPost;

	/**
	 * TODO Documentation
	 * @return
	 */
	static int loadFromConfig(std::vector<FoxWebhook> &foxWebhooks);

	/**
	 * TODO Documentation
	 * @param filePath
	 * @return
	 */
	static int loadFromConfig(const std::string &filePath, std::vector<FoxWebhook> &foxWebhooks);

private:

	/**
	 * TODO Documentation
	 * @param filePath
	 * @param json
	 * @return
	 */
	static bool readFromFile(const std::string &filePath, std::string &json) {

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

	/**
	 * TODO Documentation
	 * @param json
	 * @param webhooks
	 * @return
	 */
	static int parseJSON(const std::string &json, std::vector<FoxWebhook> &webhooks);

};

#endif //FOXWEBHOOK_FOXWEBHOOK_HPP
