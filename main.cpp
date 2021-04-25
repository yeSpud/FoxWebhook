#include <iostream>
#include "src/FoxWebhook.hpp"

int main() {

	// Load the FoxWebhooks from the config file.
	std::vector<FoxWebhook> foxWebhooks;
	FoxWebhook::loadFromConfig(foxWebhooks);

	// Iterate through each FoxWebhook.
	for (FoxWebhook foxWebhook : foxWebhooks) {

		// For now just print the most recent post url.
		std::cout << foxWebhook.getTumblrAPI().getMostRecentPost().getPost_url() << std::endl;
	}

	return 0;
}