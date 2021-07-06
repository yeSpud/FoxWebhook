#include "src/FoxWebhook.hpp"
#include <spdlog/sinks/basic_file_sink.h>

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
std::shared_ptr<spdlog::logger> logger;

/**
 * TODO Documentation
 * @param f
 */
void checkForNewPost(FoxWebhook &f) {

	TumblrAPI t = f.getTumblrAPI();

	// Get the most recent post from the tumblr blog as json.
	cpr::Response postResponse = t.getPostsJson(1);

	// Check to see if the response was successful (will return with 200).
	if (postResponse.status_code != 200) {

		// Log that there was an issue retrieving the post and return early.
		logger->warn("Unable to retrieve most recent post during loop");
		return;
	}

	// Generate the post from the json.
	TumblrAPI::Post p = TumblrAPI::generatePosts(postResponse.text.c_str())[0];

	// Compare the posts.
	logger->debug(fmt::format("Comparing post id {} to post id {}", p.id_string, f.previousPost.id_string));
	if (p != f.previousPost) {

		// Log that a new post was found.
		logger->info("New post found! " + p.post_url);

		// Get the blog json from the tumblr api.
		cpr::Response blogResponse = t.getBlogInfoJson();

		// Verify that the response for retrieving the blog json was successful (will return with 200).
		if (blogResponse.status_code != 200) {

			// Log that we could not get the blog info and return early.
			// DO NOT SET THE PREVIOUS POST (that way this will be rerun).
			logger->warn("Unable to retrieve blog info during loop");
			return;
		}

		// Generate the blog from the blog json.
		TumblrAPI::Blog b = TumblrAPI::generateBlog(blogResponse.text.c_str());

		// Get the post image to send.
		std::string image = p.content[0].url;

		// Try overriding the image if a better one is found.
		for (const Content::Image& newImage : p.content) {
			if (newImage.has_original_dimensions) {
				image = newImage.url;
				break;
			}
		}

		// Send the embed.
		f.getDiscordWebhook().sendEmbed(b.title, p.post_url, b.avatar[0].url, image);

		// And finally reset the previous post to the current post.
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

		// Get the most recent post from the blog. Start by getting the json.
		cpr::Response response = foxWebhook.getTumblrAPI().getPostsJson(1);

		// Check the response ode for the post. If it isn't 200 be sure to log as an error and return now.
		if (response.status_code != 200) {

			logger->error("Unable to get initial post for blog!");
			return ErrorCodes::CANNOT_GET_INITIAL_POST;
		}

		// Get the post object since the status code was valid.
		TumblrAPI::Post post = TumblrAPI::generatePosts(response.text.c_str())[0];

		// Set the previous post for the webhook to the returned post.
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
