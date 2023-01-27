//
// Created by Spud on 4/24/2021.
//

#ifndef FOXWEBHOOK_DISCORDWEBHOOK_HPP
#define FOXWEBHOOK_DISCORDWEBHOOK_HPP

#include "cpr/cpr.h"

class Embed {

public:

	struct Footer {
		std::string text;
		std::string icon_url;
		std::string proxy_icon_url;
	};

	struct Image {
		std::string url;
		std::string proxy_url;
		unsigned int height = 0;
		unsigned int width = 0;
	};

	struct Provider {
		std::string name;
		std::string url;
	};

	struct Author {
		std::string name;
		std::string url;
		std::string icon_url;
		std::string proxy_icon_url;
	};

	struct Field {
		std::string name;
		std::string value;
		bool Inline;
	};

	std::string title;

	//std::string type = "rich";

	std::string description;

	std::string url;

	/**
	 * ISO8601 timestamp
	 */
	std::string timestamp;

	int color = 0;

	Footer footer;

	Image image;

	Image thumbnail;

	Image video;

	Provider provider;

	Author author;

	std::vector<Field> fields;
};

class DiscordWebhook {

private:

	/**
	 * The url of the webhook.
	 */
	const std::string webhookURL;

	std::string content;

	/**
	 * Embeds to be sent.
	 */
	std::vector<Embed> embeds;

	void mswap(DiscordWebhook& other) noexcept {
		std::swap(other.content, content);
		std::swap(other.embeds, embeds);
	}

	/**
	 * Sends a message to the channel determined by the json.
	 */
	[[nodiscard]] cpr::Response sendWebhook(const std::string &json) const;

public:

	/**
	 * Creates a discord webhook object from a given url.
	 */
	explicit DiscordWebhook(std::string webhookURL): webhookURL(std::move(webhookURL)) {};

	/**
	 * Creates a discord webhook object from just the ID and token.
	 */
	DiscordWebhook(const std::string& id, const std::string& token):webhookURL("https://discord.com/api/webhooks/" + id + "/" + token) {}

	DiscordWebhook(const DiscordWebhook& copy) = default;

	DiscordWebhook& operator=(const DiscordWebhook& rhs) {

		// Make a copy.
		DiscordWebhook copy(rhs);

		// Swap.
		mswap(copy);

		return *this;
	}

	DiscordWebhook(DiscordWebhook&& rhs) noexcept : webhookURL(rhs.webhookURL), content(rhs.content), embeds(rhs.embeds) {};

	DiscordWebhook& operator=(DiscordWebhook&& rhs) noexcept {

		// Swap.
		mswap(rhs);

		return *this;
	}

	/**
	 * Set the message to be sent to the channel.
	 */
	void setMessage(const std::string& message) {
		content = message;
	}

	/**
	 * Adds an embed object to be sent to the channel - multiple embeds (max 10) can be sent.
	 * @param embed
	 */
	void appendEmbed(const Embed& embed) {
		if (embeds.size() <= 10) {
			embeds.push_back(embed);
		}
	}

	/**
	 * Sends the message and any embeds to the channel.
	 */
	cpr::Response send();

};


#endif //FOXWEBHOOK_DISCORDWEBHOOK_HPP
