//
// Created by Spud on 1/18/23.
//

#include "spdlog/sinks/basic_file_sink.h"
#include "FoxWebhook.hpp"

int main(int argc, char** argv) {

	std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_st("Logger", "cron-log.txt");
	logger->flush_on(spdlog::level::info);
	spdlog::set_default_logger(logger);
	spdlog::flush_on(spdlog::level::info);

	std::vector<FoxWebhook> foxWebhooks;
	if (argc == 1) {
		int status = FoxWebhook::loadFromConfig(foxWebhooks);
		if (status != 0) {
			logger->error(fmt::format("Unable to load fox webhooks: {}", status));
			return -2;
		}
	} else if (argc == 4) {
		FoxWebhook foxWebhook = FoxWebhook(argv[2], argv[3], argv[1]);
		foxWebhooks.push_back(foxWebhook);
	} else {
		logger->error("Usage: ./cron");
		logger->error("Usage: ./cron <webhook url> <blog url> <api key>");
		return -1;
	}

	for (FoxWebhook foxWebhook : foxWebhooks) {
		cpr::Response postResponse = cpr::Get(cpr::Url{"https://api.tumblr.com/v2/blog/", foxWebhook.blog, "/posts?api_key=",
													   foxWebhook.key, "&npf=true&limit=1"});
		if (postResponse.status_code >= 400) {
			logger->error("Unable to get most recent post ({0}): {1}", postResponse.status_code, postResponse.text);
			return 1;
		}

		/*
		 * Example of response from post:
		 * {
		 *  "meta": {...},
		 *  "response": {
		 *      "blog": {...},
		 *      "posts": [...],
		 *      "total_posts": 714,
		 *      "_links": {...}
		 *  }
		 * }
		 *
		 * Here we want to get the first item in the posts array.
		 */
		rapidjson::Document returnedJson;
		returnedJson.Parse(postResponse.text);
		rapidjson::GenericObject responseJson = returnedJson["response"].GetObj();

		/*
		 * Example of post array:
		 * "posts": [
		 *  {
		 *      "type": "blocks",
		 *      "original_type": "regular",
		 *      "is_blocks_post_format": true,
		 *      "blog_name": "foxes-in-love",
		 *      "blog": {...},
		 *      "id": 706888475865137200,
		 *      "id_string": "706888475865137152",
		 *      "post_url": "https://foxes-in-love.tumblr.com/post/706888475865137152",
		 *      "slug": "",
		 *      "date": "2023-01-19 15:16:23 GMT",
		 *      "timestamp": 1674141383,
		 *      "state": "published",
		 *      "reblog_key": "QLmjK0Su",
		 *      "tags": [],
		 *      "short_url": "https://tmblr.co/Zn_LPXdFNrBJma00",
		 *      "summary": "",
		 *      "should_open_in_legacy": false,
		 *      "recommended_source": null,
		 *      "recommended_color": null,
		 *      "note_count": 3591,
		 *      "content": [...],
		 *      "layout": [],
		 *      "trail": [],
		 *      "can_like": false,
		 *      "interactability_reblog": "everyone",
		 *      "can_reblog": false,
		 *      "can_send_in_message": true,
		 *      "can_reply": false,
		 *      "display_avatar": true
	     *   }
	     *  ]
	     *
	     * Here we want to compare the post_url - if its different pull from the content array.
		 */
		rapidjson::GenericObject post = responseJson["posts"].GetArray()[0].GetObj();
		std::string postUrl = post["post_url"].GetString();

		std::ifstream infile("most-recent-" + foxWebhook.blog + ".url");
		if (infile) {

			// File exists - read from infile.
			std::string mostRecentUrl;
			infile >> mostRecentUrl;
			infile.close();

			if (mostRecentUrl == postUrl) {

				// Post has not changed, so just return.
				return 0;
			}
		}

		// Get the relevant blog information from the post.
		/*
		 * Example of blog data from response json:
		 * "blog": {
		 *  "ask": false,
		 *  "ask_anon": false,
		 *  "ask_page_title": "Ask me anything",
		 *  "asks_allow_media": true,
		 *  "avatar": [...],
		 *  "can_chat": false,
		 *  "can_subscribe": false,
		 *  "description": "",
		 *  "is_nsfw": false,
		 *  "name": "foxes-in-love",
		 *  "posts": 714,
		 *  "share_likes": false,
		 *  "subscribed": false,
		 *  "theme": {...},
		 *  "title": "Foxes in Love",
		 *  "total_posts": 714,
		 *  "updated": 1674141384,
		 *  "url": "https://foxes-in-love.tumblr.com/",
		 *  "uuid": "t:bF2JMCXfv_Agp-5ZfJQ75A"
		 * }
		 *
		 * Here we want the blog title and avatar url.
		 */
		rapidjson::GenericObject blog = responseJson["blog"].GetObj();
		std::string avatarUrl = blog["avatar"].GetArray()[0].GetObj()["url"].GetString();
		std::string postContentImage = post["content"].GetArray()[0].GetObj()["media"].GetArray()[0].GetObj()["url"].GetString();

		Embed embed;
		embed.author.name = std::string(blog["title"].GetString()) + " has a new post";
		embed.author.url = postUrl;
		embed.author.icon_url = avatarUrl;
		embed.image.url = postContentImage;

		foxWebhook.discordWebhook.appendEmbed(embed);
		cpr::Response response = foxWebhook.discordWebhook.send();
		if (response.status_code >= 400) {
			logger->error(fmt::format("Could not send embed ({0}): {1}", response.status_code, response.text));
			return 2;
		}

		// Overwrite file content (or create a new one if it didn't exist).
		std::ofstream outfile("most-recent-" + foxWebhook.blog + ".url", std::ios::trunc);
		if (!outfile) {
			logger->error("Unable to open file stream");
			return -3;
		}

		outfile << postUrl << std::endl;
		outfile.close();
	}

	return 0;
}