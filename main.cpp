#include <iostream>
#include "src/FoxWebhook.hpp"

int main() {

	// Load the FoxWebhooks from the config file.
	std::vector<FoxWebhook> foxWebhooks;
	FoxWebhook::loadFromConfig(foxWebhooks);

	// Iterate through each FoxWebhook.
	for (FoxWebhook foxWebhook : foxWebhooks) {

		//std::cout << foxWebhook.getDiscordWebhook().sendMessage("Testing with just a message...").text << std::endl;

		Post post = foxWebhook.getTumblrAPI().getMostRecentPost();

		std::cout << foxWebhook.getTumblrAPI().getBlogInfo().text << std::endl;

		/*
		std::cout << foxWebhook.getDiscordWebhook().sendEmbed(post.getBlog_name(), post.getPost_url(),
		                                                      post.getContent()[0].getUrl(),
		                                                      post.getContent()[0].getUrl()).text << std::endl;
		                                                      */
	}

	return 0;
}