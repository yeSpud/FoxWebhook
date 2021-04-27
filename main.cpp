#include "src/FoxWebhook.hpp"
#include <iostream>

// Add sleep function based on OS
#ifdef _WIN32

#include <windows.h> // Windows sleep
#define sleep(seconds){Sleep(seconds * 1000);}

#else

#include <unistd.h> // UNIX sleep

#endif

/**
 * TODO Documentation
 * @param foxWebhook
 */
void checkForNewPost(FoxWebhook f) {

	TumblrAPI t = f.getTumblrAPI();
	Post p = t.getMostRecentPost();

	if (p != f.previousPost) {
		Blog b = t.getBlogInfo();

		f.getDiscordWebhook().sendEmbed(b.getTitle(), p.getPost_url(), b.getAvatars()[0].getUrl(),
		                                p.getContent()[0].getUrl());

		f.previousPost = std::move(p);

	}
}

int main() {

	// Load the FoxWebhooks from the config file.
	std::vector<FoxWebhook> foxWebhooks;
	FoxWebhook::loadFromConfig(foxWebhooks);

	// Initialize each fox webhook's previous posts.
	for (FoxWebhook &foxWebhook : foxWebhooks) {
		Post post = foxWebhook.getTumblrAPI().getMostRecentPost();
		foxWebhook.previousPost = std::move(post);
	}

	while (true) {

		try {
			// Iterate through each FoxWebhook and check for a new post.
			for (const FoxWebhook &foxWebhook : foxWebhooks) {
				checkForNewPost(foxWebhook);
			}

			// Wait for 45 seconds before looping...
			sleep(45);
			std::cout << "Looping..."<<std::endl;
		} catch (const std::exception &exception) {

			// Log any exceptions and break from the loop.
			std::cout << "Exception raised: " << exception.what() << std::endl;
			break;
		}
	}

	return 0;
}
