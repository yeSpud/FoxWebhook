#include <iostream>
#include "src/FoxWebhook.hpp"
#include "src/TumblrAPI.hpp"
#include "src/DiscordWebhook.hpp"

int main() {

	// Load the FoxWebhooks from the config file.
	std::vector<FoxWebhook> foxWebhooks = FoxWebhook::loadFromConfig();

	// Get a vector to store the TumblrAPIs that we will be querying.
	std::vector<TumblrAPI> tumblrAPIs;
	std::vector<DiscordWebhook> discordWebhooks;

	for (FoxWebhook foxWebhook : foxWebhooks) {

		const std::string token = foxWebhook.getAuth();
		if (!token.empty()) {

			TumblrAPI tumblrRequest = TumblrAPI(token, foxWebhook.getBlogURL());
			tumblrAPIs.push_back(tumblrRequest);

			DiscordWebhook discordWebhook = DiscordWebhook(foxWebhook.getWebhookURL());
			discordWebhooks.push_back(discordWebhook);
		}
	}

	/*
	for (TumblrAPI request : tumblrAPIs) {
		std::cout << request.getMostRecentPost().text << std::endl;
	}
	 */

	for (DiscordWebhook webhook : discordWebhooks) {
		cpr::Response r = webhook.sendEmbed("Blog name", "https://www.google.com", "https://images-ext-2.discordapp.net/external/yEuQbTDhl2vBUz72K7mV6olN4IXRoYvkrvY208DJaYA/https/64.media.tumblr.com/29691e0fb00ec7e9b99780bf30daa9b8/cd277732d2cac806-30/s2048x3072/0eb8e2d885727bdb950940bfae0552ef80f65ea5.jpg", "https://images-ext-2.discordapp.net/external/yEuQbTDhl2vBUz72K7mV6olN4IXRoYvkrvY208DJaYA/https/64.media.tumblr.com/29691e0fb00ec7e9b99780bf30daa9b8/cd277732d2cac806-30/s2048x3072/0eb8e2d885727bdb950940bfae0552ef80f65ea5.jpg");
		std::cout << r.status_code << ':' << r.text << '\n' << r.error.message << std::endl;
	}

	return 0;
}