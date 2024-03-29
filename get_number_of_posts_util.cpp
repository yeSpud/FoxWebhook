//
// Created by Spud on 3/10/22.
//

#include "spdlog/sinks/basic_file_sink.h"
#include "include/FoxWebhook.hpp"
#include <iostream>
#include "cpr/cpr.h"

/**
 * Logger used by script.
 */
std::shared_ptr<spdlog::logger> logger;

unsigned long getInput(const FoxWebhook& foxWebhook) {

	// Get the number of posts to get for each foxWebhook.
	unsigned long input = 0;
	std::cout << "How many posts should be retrieved from " << foxWebhook.blog << "? (max 10) "; // << std::endl; // no new lines.
	std::cin >> input;

	// Validate user input.
	while (std::cin.fail() || input > 10) {

		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid input." << std::endl;
		std::cout << "Retry: "; // << std::endl; // no new lines.
		std::cin >> input;
	}

	return input;
}

int main() {

	// Setup the logger.
	logger = spdlog::basic_logger_st("Logger", "util-log.txt");
	logger->flush_on(spdlog::level::debug);
	spdlog::set_default_logger(logger);
	spdlog::flush_on(spdlog::level::debug);
	logger->info("Starting up util script...");

	// Load the FoxWebhooks from the config file.
	std::vector <FoxWebhook> foxWebhooks;
	int loadStatus = FoxWebhook::loadFromConfig(foxWebhooks);

	// Check the status of the load.
	if (loadStatus != 0) {
		logger->error(fmt::format("Unable to load FoxWebhooks: {}", loadStatus));
		return loadStatus;
	}
	logger->info(fmt::format("Loaded {} FoxWebhooks(s) successfully", foxWebhooks.size()));

	// Iterate though each FoxWebhook.
	for (FoxWebhook &foxWebhook : foxWebhooks) {

		// Get the number of posts to retrieve.
		unsigned long input = getInput(foxWebhook);
		logger->info(fmt::format("Retrieving {} posts...", input));

		// Fetch the posts json data from the tumblr api.
		cpr::Response response = cpr::Get(cpr::Url({"https://api.tumblr.com/v2/blog/", foxWebhook.blog, "/posts?api_key=",
													foxWebhook.key, "&type=photo&npf=true&limit=", std::to_string(input)}));

		rapidjson::Document document;
		document.Parse(response.text);
		if (response.status_code >= 400) {
			logger->warn("Unable to get post(s) for blog!\nResponse code {0}.\n{1}", response.status_code, response.text);
			continue;
		}

		rapidjson::GenericObject postsJson = document["response"].GetObj();
		rapidjson::GenericObject blog = postsJson["blog"].GetObj();
		std::string avatarUrl = blog["avatar"].GetArray()[0].GetObj()["url"].GetString();

		rapidjson::GenericArray posts = postsJson["posts"].GetArray();
		std::reverse(posts.Begin(), posts.End());

		for (auto& post : posts) {

			Embed embed;
			embed.author.name = "Post by " + std::string(blog["title"].GetString());
			embed.author.url = post["post_url"].GetString();
			embed.author.icon_url = avatarUrl;

			rapidjson::GenericObject content = post["content"].GetArray()[0].GetObj();
			std::string contentType = content["type"].GetString();
			if (contentType == "image") {

				embed.image.url = content["media"].GetArray()[0].GetObj()["url"].GetString();
			} else {
				logger->warn("Unable to handle post type: " + contentType);
			}

			foxWebhook.discordWebhook.appendEmbed(embed);
		}

		cpr::Response discordResponse = foxWebhook.discordWebhook.send();
		if (discordResponse.status_code >= 400) {
			logger->warn("An error occurred sending posts via the webhook!\nResponse code {0}.\n{1}", discordResponse.status_code,
			             discordResponse.text);
		}
	}

	logger->info("Exiting...");
	logger->flush();

	return 0;
}