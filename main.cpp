#include <spdlog/sinks/basic_file_sink.h>
#include "src/FoxWebhook.hpp"
#include <npf/content/image.hpp>
#include <cpr/cpr.h>

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

/**
 * TODO Documentation
 * @param foxWebhook
 * @return
 */
std::shared_ptr<Post> getMostRecentPost(FoxWebhook foxWebhook) {

	// Get the most recent post from the blog. Start by getting the json.
	cpr::Response response = foxWebhook.tumblrApi.getPostsJson(foxWebhook.blog, 1);

	// Check the response ode for the post. If it isn't 200 be sure to log as an error and return now.
	if (response.status_code != 200) {

		logger->error("Unable to get post!\nResponse code {0}.\n{1}", response.status_code, response.text);
		return nullptr;
	}

	// Get the posts as a vector for now (for debugging).
	std::vector<std::shared_ptr<Post>> posts = Post::generatePosts(response.text.c_str());

	// Return the pointer to the first post in the vector.
	return posts[0];
}

/**
 * TODO Documentation
 * @param foxWebhook
 */
void checkForNewPost(FoxWebhook &foxWebhook) {

	// Get the most recent post from the tumblr blog.
	std::shared_ptr<Post> post = getMostRecentPost(foxWebhook);

	// Make sure the retrieved post isn't null.
	if (post == nullptr) {

		// Log that there was an issue retrieving the post and return early.
		logger->warn("Unable to retrieve most recent post during loop");
		return;
	}

	// Compare the posts.
	logger->debug(fmt::format("Comparing post id {} to post id {}", post->id_string, foxWebhook.previousPost->id_string));
	if (post->id_string == foxWebhook.previousPost->id_string) {

		// Return early if there are no new posts.
		logger->debug("No new post found");
		return;
	}

	// Log that a new post was found.
	logger->info("New post found! " + post->post_url);

	// Get the blog json from the tumblr api.
	cpr::Response blogResponse = foxWebhook.tumblrApi.getBlogInfoJson(foxWebhook.blog);

	// Verify that the response for retrieving the blog json was successful (will return with 200).
	if (blogResponse.status_code != 200) {

		// Log that we could not get the blog info and return early.
		// DO NOT SET THE PREVIOUS POST (that way this will be rerun).
		logger->warn("Unable to retrieve blog info during loop");
		return;
	}

	// Get the content of the post.
	std::shared_ptr<Content> postContent = post->content[0];

	// Check if the post content is an Image type.
	if (postContent->type != "image") {
		logger->warn("New post is not an image post!");
		return;
	}

	// Get the post image to send.
	std::shared_ptr<Image> image = std::dynamic_pointer_cast<Image>(postContent);
	Media media = image->media[0];

	// Generate the blog from the blog json.
	Blog blog = Blog::generateBlog(blogResponse.text.c_str());

	// Send the embed.
	logger->debug("Sending post to discord channel");
	foxWebhook.discordWebhook.sendEmbed(blog.title, post->post_url, blog.avatars[0].url, media.url);

	// And finally reset the previous post to the current post.
	logger->debug(fmt::format("Setting previous post to {}", post->id_string));
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

	// Initialize each fox webhook's previous posts.
	for (FoxWebhook &foxWebhook : foxWebhooks) {

		// Get hte most recent post from the blog.
		std::shared_ptr<Post> post = getMostRecentPost(foxWebhook);

		// Make sure the post isn't null.
		if (post == nullptr) {

			logger->error("Failed to retrieve initial post!");
			return ErrorCodes::CANNOT_GET_INITIAL_POST;
		}

		// Set the previous post for the webhook to the returned post.
		foxWebhook.previousPost = post;
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
