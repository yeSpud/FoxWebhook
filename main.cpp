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

		Blog blog = foxWebhook.getTumblrAPI().getBlogInfo();

		std::cout << blog.getTitle() << std::endl;

		std::cout << foxWebhook.getDiscordWebhook().sendEmbed(blog.getTitle(), post.getPost_url(),
		                                                      blog.getAvatars()[0].getUrl(),
		                                                      post.getContent()[0].getUrl()).text << std::endl;

	}

	return 0;
}