//
// Created by Spud on 3/10/22.
//

#include <spdlog/sinks/basic_file_sink.h>
#include "src/FoxWebhook.hpp"
#include <iostream>
#include <cpr/cpr.h>
#include <npf/content/image.hpp>

// Add sleep function based on OS
#ifdef _WIN32

#include <windows.h> // Windows sleep

#define sleep(seconds){Sleep(seconds * 1000);}

#else

#include <unistd.h> // UNIX sleep

#endif

unsigned long getInput() {

	// Get the number of posts to get for each foxWebhook.
	unsigned long input = 0;
	std::cout << "How many posts should be retrieved? "; // << std::endl; - no new lines.
	std::cin >> input;

	// Validate user input.
	while (std::cin.fail()) {

		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid input." << std::endl;
		std::cout << "Retry: "; // << std::endl; - no new lines.
		std::cin >> input;
	}

	return input;
}

int main() {

	// Setup the logger.
	std::shared_ptr <spdlog::logger> logger = spdlog::basic_logger_st("Logger", "util-log.txt");
	logger->flush_on(spdlog::level::debug);
	spdlog::set_default_logger(logger);
	spdlog::flush_on(spdlog::level::debug);
	logger->debug("Finished setting up logger");
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

	// Iterate though each foxwebhook.
	for (FoxWebhook &foxWebhook : foxWebhooks) {

		// Get the blog json from the tumblr api.
		cpr::Response blogJson = foxWebhook.tumblrApi.getBlogInfoJson(foxWebhook.blog);

		// Verify that the response for retrieving the blog json was successful (will return with 200).
		if (blogJson.status_code != 200) {

			// Log that we could not get the blog info and return early.
			// DO NOT SET THE PREVIOUS POST (that way this will be rerun).
			logger->warn("Unable to retrieve blog info!\nResponse code {0}.\n{1}", blogJson.status_code, blogJson.text);
			return ErrorCodes::INVALID_JSON_FORMAT;
		}
		logger->info(fmt::format("Blog response json vvv\n{}\n", blogJson.text));

		// Create an actual blog object.
		Blog blog = Blog::generateBlog(blogJson.text.c_str());

		// Get the number of posts to retrieve.
		unsigned long input = getInput();
		logger->info(fmt::format("Retrieving {} posts...", input));

		// Fetch the posts json data from the tumblr api.
		cpr::Response postsJson = foxWebhook.tumblrApi.getPostsJson(foxWebhook.blog, input);

		// Check the response ode for the post. If it isn't 200 be sure to log as an error and return now.
		if (postsJson.status_code != 200) {

			logger->error("Unable to get initial post for blog!\nResponse code {0}.\n{1}", postsJson.status_code, postsJson.text);
			return ErrorCodes::INVALID_JSON_FORMAT;
		}
		logger->info(fmt::format("Posts response json vvv\n{}\n", postsJson.text));

		// Generate posts from the response postsJson.
		std::vector<std::shared_ptr<Post>> posts = Post::generatePosts(postsJson.text.c_str());

		std::reverse(posts.begin(), posts.end());
		for (std::shared_ptr<Post> post : posts) {

			// TODO post to discord.

			// Sleep for 5 seconds.
			sleep(5);
		}
	}

	logger->info("Exiting...");
	logger->flush();

	return 0;
}