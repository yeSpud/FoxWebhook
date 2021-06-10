#include "src/FoxWebhook.hpp"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

// Add sleep function based on OS
#ifdef _WIN32

#include <windows.h> // Windows sleep

#define sleep(seconds){Sleep(seconds * 1000);}

#else

#include <unistd.h> // UNIX sleep

#endif

/**
 * TODO Documentation
 */
std::shared_ptr<spdlog::logger> logger;

/**
 * TODO Documentation & comments
 * @param foxWebhook
 */
void checkForNewPost(FoxWebhook &f) {

	TumblrAPI t = f.getTumblrAPI();
	Post p = t.getMostRecentPost();

	logger->debug(fmt::format("Comparing post id {} to post id {}", p.getId_string(), f.previousPost.getId_string()));
	if (p != f.previousPost) {
		logger->info("New post found! " + p.getPost_url());
		Blog b = t.getBlogInfo();

		f.getDiscordWebhook().sendEmbed(b.getTitle(), p.getPost_url(), b.getAvatars()[0].getUrl(),
		                                p.getContent()[0].getUrl());

		f.previousPost = std::move(p);

	}
}

int main() {

	// Setup the logger.
	logger = spdlog::basic_logger_st("Logger", "log.txt");
	logger->flush_on(spdlog::level::info);
	spdlog::set_default_logger(logger);
	spdlog::flush_on(spdlog::level::info);
	logger->info("Starting up script...");

	// Load the FoxWebhooks from the config file.
	std::vector<FoxWebhook> foxWebhooks;
	int status = FoxWebhook::loadFromConfig(foxWebhooks);

	// If the status from the config was not 0, return the status.
	if (status != 0) {
		logger->error(fmt::format("Unable to load FoxWebhooks: {}", status));
		return status;
	}
	logger->info(fmt::format("Loaded {} FoxWebhook(s) successfully", foxWebhooks.size()));

	// Initialize each fox webhook's previous posts.
	for (FoxWebhook &foxWebhook : foxWebhooks) {
		Post post = foxWebhook.getTumblrAPI().getMostRecentPost();
		logger->info(fmt::format("Initializing with most recent post ID for {}: {}", post.getBlog_name(),
		                         post.getId_string()));

		foxWebhook.previousPost = std::move(post);
	}

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
