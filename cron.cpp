//
// Created by Spud on 1/18/23.
//

#include <iostream>
#include "include/DiscordWebhook.hpp"

#define APIKEY argv[3]
#define BLOG argv[2]
#define WEBHOOK argv[1]

int main(int argc, char** argv) {

	if (argc != 4) {
		std::cerr << "Usage: ./cron <webhook url> <blog url> <api key>" << std::endl;
		return -1;
	}

	cpr::Response postResponse = cpr::Get(cpr::Url{"https://api.tumblr.com/v2/blog/", BLOG, "/posts?api_key=", APIKEY, "&npf=true&limit=1"});
	if (postResponse.status_code != 200) {
		std::cerr << "Unable to get most recent post: " << postResponse.status_code << std::endl;
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
	rapidjson::GenericObject<false, rapidjson::Value> responseJson = returnedJson["response"].GetObj();

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
	rapidjson::GenericObject<false, rapidjson::Value> post = responseJson["posts"].GetArray()[0].GetObj();
	std::string postUrl = post["post_url"].GetString();

	std::ifstream infile("most_recent.url");
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

	// Overwrite file content (or create a new one if it didn't exist).
	std::ofstream outfile("most_recent.url", std::ios::trunc);
	if (!outfile) {
		std::cerr << "Unable to open file stream" << std::endl;
		return -2;
	}

	outfile << postUrl << std::endl;
	outfile.close();

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
	rapidjson::GenericObject<false, rapidjson::Value> blog = responseJson["blog"].GetObj();
	std::string avatarUrl = blog["avatar"].GetArray()[0].GetObj()["url"].GetString();
	std::string postContentImage = post["content"].GetArray()[0].GetObj()["media"].GetArray()[0].GetObj()["url"].GetString();

	DiscordWebhook webhook = DiscordWebhook(WEBHOOK);
	cpr::Response response = webhook.sendEmbed(blog["title"].GetString(), postUrl, avatarUrl, postContentImage);
	if (response.status_code != 200) {
		std::cerr << "Could not send embed: " << response.text << std::endl;
		return 2;
	}

	return 0;
}