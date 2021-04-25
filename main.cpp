#include <iostream>
#include "src/FoxWebhook.hpp"

int main() {

	// Load the FoxWebhooks from the config file.
	std::vector<FoxWebhook> foxWebhooks;
	FoxWebhook::loadFromConfig(foxWebhooks);

	// Iterate through each FoxWebhook.
	for (FoxWebhook foxWebhook : foxWebhooks) {


		Post post = foxWebhook.getTumblrAPI().getMostRecentPost();

		foxWebhook.getDiscordWebhook().sendEmbed(post.getBlog_name(), post.getPost_url(), post.getContent()[0].getUrl(), post.getContent()[0].getUrl());
	}

	return 0;
}