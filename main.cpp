#include <iostream>
#include "src/FoxWebhook.hpp"
#include "src/TumblrAPI.hpp"

int main() {

	// Load the FoxWebhooks from the config file.
	std::vector<FoxWebhook> foxWebhooks = FoxWebhook::loadFromConfig();

	// Get a vector to store the TumblrAPIs that we will be querying.
	std::vector<TumblrAPI> tumblrAPIs;

	for (FoxWebhook foxWebhook : foxWebhooks) {

		const std::string token = foxWebhook.getAuth();
		if (!token.empty()) {

			TumblrAPI tumblrRequest = TumblrAPI(token, foxWebhook.getBlogURL());
			tumblrAPIs.push_back(tumblrRequest);
		}
	}

	for (TumblrAPI request : tumblrAPIs) {
		std::cout << request.getMostRecentPost().text << std::endl;
	}

	return 0;
}