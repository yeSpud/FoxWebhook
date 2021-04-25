#include <iostream>
#include "src/FoxWebhook.hpp"

int main() {

	// Load the FoxWebhooks from the config file.
	std::vector<FoxWebhook> foxWebhooks;
	FoxWebhook::loadFromConfig(foxWebhooks);

	// Iterate through each FoxWebhook.
	for (FoxWebhook foxWebhook : foxWebhooks) {

		// For now just print the blog info.
		std::cout << foxWebhook.getTumblrAPI().getBlogInfo().text << std::endl;
	}

	return 0;
}