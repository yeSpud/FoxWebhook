//
// Created by Spud on 3/10/22.
//

#include <spdlog/sinks/basic_file_sink.h>
#include "src/FoxWebhook.hpp"
#include <iostream>
#include <npf/content/image.hpp>
#include <cpr/cpr.h>

// Add sleep function based on OS
#ifdef _WIN32

#include <windows.h> // Windows sleep

#define sleep(seconds){Sleep(seconds * 1000);}

#else

#include <unistd.h> // UNIX sleep
#include <iostream>

#endif

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
		logger->error(fmt::format("Unable to load FoxWebooks: {}", loadStatus));
		return loadStatus;
	}
	logger->info(fmt::format("Loaded {} FoxWebhooks(s) successfully", foxWebhooks.size()));

	// Iterate though each foxwebhook.
	for (FoxWebhook &foxWebhook : foxWebhooks) {

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
		logger->info(fmt::format("Retrieving {} posts...", input));

		// Fetch the json data from the tumblr api.
		cpr::Response json = foxWebhook.tumblrApi.getPostsJson(foxWebhook.blog, input);

		// Check the response ode for the post. If it isn't 200 be sure to log as an error and return now.
		if (json.status_code != 200) {

			logger->error("Unable to get initial post for blog!\nResponse code {0}.\n{1}", json.status_code, json.text);
			return ErrorCodes::CANNOT_GET_INITIAL_POST;
		}
		logger->info(fmt::format("Response json vvv\n{}\n", json.text));

	}

	logger->info("Exiting...");
	logger->flush();

	return 0;
}