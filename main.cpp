#include "src/FoxWebhook.hpp"

void checkForNewPost(FoxWebhook foxWebhook);

int main() {

	// Load the FoxWebhooks from the config file.
	std::vector<FoxWebhook> foxWebhooks;
	FoxWebhook::loadFromConfig(foxWebhooks);

	// Initialize each fox webhook's previous posts.
	for (FoxWebhook &foxWebhook : foxWebhooks) {
		Post post = foxWebhook.getTumblrAPI().getMostRecentPost();
		foxWebhook.previousPost = std::move(post);
	}


	// Iterate through each FoxWebhook and check for a new post.
	for (const FoxWebhook &foxWebhook : foxWebhooks) {
		checkForNewPost(foxWebhook);
	}

	return 0;
}

/**
 * TODO Documentation
 * @param foxWebhook
 */
void checkForNewPost(FoxWebhook f) {

	TumblrAPI t = f.getTumblrAPI();
	Post p = t.getMostRecentPost();

	//if (p.getId() != f.getPreviousPost().getId()) {
	if (p != f.previousPost) {
		Blog b = t.getBlogInfo();

		f.getDiscordWebhook().sendEmbed(b.getTitle(), p.getPost_url(), b.getAvatars()[0].getUrl(),
		                                p.getContent()[0].getUrl());

		f.previousPost = std::move(p);

	}
}