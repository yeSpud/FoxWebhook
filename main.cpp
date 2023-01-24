#include "FoxWebhook.hpp"
#include "spdlog/sinks/basic_file_sink.h"

// Add sleep function based on OS
#ifdef _WIN32

#include <windows.h> // Windows sleep

#define sleep(seconds){Sleep(seconds * 1000);}

#else

#include <unistd.h> // UNIX sleep

#endif

/**
 * Pointer for the logger of the script.
 */
std::shared_ptr <spdlog::logger> logger;

void checkForNewPost(FoxWebhook &foxWebhook) {

	// Get the most recent post from the blog. Start by getting the json.
	cpr::Response response = cpr::Get(cpr::Url{"https://api.tumblr.com/v2/blog/", foxWebhook.blog, "/posts?api_key=",
											   foxWebhook.key, "&npf=true&limit=1"});

	// Check the response code for the post. If it isn't 200 be sure to log as an error and return now.
	rapidjson::Document returnedJson;
	returnedJson.Parse(response.text);
	if (response.status_code >= 400) {
		logger->warn("Unable to get post!\nResponse code: {0}.\nResponse text: {1}\nError message: {2}", response.status_code,
		             response.text, response.error.message);
		return;
	}

	rapidjson::GenericObject<false, rapidjson::Value> responseJson = returnedJson.GetObj()["response"].GetObj();
	rapidjson::GenericObject<false, rapidjson::Value> post = responseJson["posts"].GetArray()[0].GetObj();

	// Compare the posts.
	logger->debug(fmt::format("Comparing post id {} to post id {}", post["id_string"].GetString(),
							  foxWebhook.previousPost["id_string"].GetString()));
	if (post["id"].GetInt64() == foxWebhook.previousPost["id"].GetInt64()) {
		logger->debug("No new post found");
		return;
	}
	logger->info(fmt::format("New post found! {}", post["post_url"].GetString()));

	rapidjson::GenericObject<false, rapidjson::Value> blog = responseJson["blog"].GetObj();
	std::string avatarUrl = blog["avatar"].GetArray()[0].GetObj()["url"].GetString();
	std::string postContentImage = post["content"].GetArray()[0].GetObj()["media"].GetArray()[0].GetObj()["url"].GetString();

	// Send the embed.
	logger->debug("Sending post to discord channel");
	foxWebhook.discordWebhook.sendEmbed(blog["title"].GetString(), post["post_url"].GetString(), avatarUrl, postContentImage);

	// And finally reset the previous post to the current post.
	logger->debug(fmt::format("Setting previous post to {}", post["id_string"].GetString()));
	foxWebhook.previousPost = post;

	logger->info("Returning to main function");
}

int main() {

	// Setup the logger.
	logger = spdlog::basic_logger_st("Logger", "log.txt");
	logger->flush_on(spdlog::level::debug);
	spdlog::set_default_logger(logger);
	spdlog::flush_on(spdlog::level::debug);
	logger->info("Starting up script...");

	// Load the FoxWebhooks from the config file.
	std::vector <FoxWebhook> foxWebhooks;
	int status = FoxWebhook::loadFromConfig(foxWebhooks);

	// If the status from the config was not 0, return the status.
	if (status != 0) {
		logger->error(fmt::format("Unable to load FoxWebhooks: {}", status));
		return status;
	}
	logger->info(fmt::format("Loaded {} FoxWebhook(s) successfully", foxWebhooks.size()));

	while (true) {
		try {

			// Iterate through each FoxWebhook and check for a new post.
			for (FoxWebhook &foxWebhook : foxWebhooks) {
				checkForNewPost(foxWebhook);
			}

			// Wait for 90 seconds before looping...
			sleep(90);
		} catch (const std::exception &exception) {

			// Log any exceptions and break from the loop.
			logger->error(fmt::format("Error occurred! {}", exception.what()));
			logger->flush();
			break;
		}
	}

	logger->warn("Exiting early!");
	return 0;
}
