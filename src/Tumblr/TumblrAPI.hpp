//
// Created by Spud on 4/23/2021.
//

#ifndef FOXWEBHOOK_TUMBLRAPI_HPP
#define FOXWEBHOOK_TUMBLRAPI_HPP

#include <cpr/cpr.h>
#include <spdlog/spdlog.h>
#include "Post.hpp"

class TumblrAPI {

private:

	/**
	 * TODO Documentation
	 */
	const std::string token;

	/**
	 * TODO Documentation
	 */
	const std::string blogURL;

	/**
	 * TODO Documentation
	 * @param endpoint
	 * @param authRequired
	 * @param optionalParams
	 * @return
	 */
	cpr::Response sendRequest(const std::string &endpoint, bool authRequired, const std::string &optionalParams = "") {

		// Get our logger.
		std::shared_ptr<spdlog::logger> logger = spdlog::get("Logger");

		// Format the URL to go to for retrieving data.
		std::string url = "api.tumblr.com/v2/" + endpoint;
		if (authRequired) {
			url += "?api_key=" + token;
		}
		url += optionalParams;

		// Get the response from the URL.
		logger->debug(fmt::format("Querying url: {}", url));
		cpr::Response response;
		response = cpr::Get(cpr::Url{url});

		// If check the response code.
		if (response.status_code != 200) {
			logger->warn(fmt::format("Status code {} - {}: {}", response.status_code, response.reason, response.text));
		}

		return response;
	}

	/**
	 * TODO Documentation & comments
	 * @param entry
	 * @param value
	 * @return
	 */
	static bool getBool(rapidjson::Value &entry, const char* value) {
		if (entry.HasMember(value)) {
			return entry[value].GetBool();
		} else {
			return false;
		}
	}

	/**
	 * TODO Documentation
	 * @param entry
	 * @param value
	 * @return
	 */
	static std::string getString(rapidjson::Value &entry, const char* value) {
		if (entry.HasMember(value)) {
			return entry[value].GetString();
		} else {
			return "";
		}
	}

	/**
	 * TODO Documentation
	 * @param entry
	 * @param value
	 * @return
	 */
	static uint64_t getNumber(rapidjson::Value &entry, const char* value) {
		if (entry.HasMember(value)) {
			return entry[value].GetUint64();
		} else {
			return 0;
		}
	}

public:

	struct Blog {

		/**
		 * TODO Documentation
		 */
		bool ask;

		/**
		 * TODO Documentation
		 */
		bool ask_anon;

		/**
		 * TODO Documentation
		 */
		std::string ask_page_title;

		/**
		 * TODO Documentation
		 */
		bool asks_allow_media;

		/**
		 * TODO Documentation
		 */
		std::vector<Content::Image> avatar;

		/**
		 * TODO Documentation
		 */
		bool can_chat;

		/**
		 * TODO Documentation
		 */
		bool can_subscribe;

		/**
		 * TODO Documentation
		 */
		std::string description;

		/**
		 * TODO Documentation
		 */
		bool is_nsfw;

		/**
		 * TODO Documentation
		 */
		std::string name;

		/**
		 * TODO Documentation
		 */
		uint64_t posts;

		/**
		 * TODO Documentation
		 */
		bool shared_likes;

		/**
		 * TODO Documentation
		 */
		bool subscribed;

		// auto theme

		/**
		 * TODO Documentation
		 */
		std::string title;

		/**
		 * TODO Documentation
		 */
		uint64_t total_posts;

		/**
		 * TODO Documentation
		 */
		uint64_t updated;

		/**
		 * TODO Documentation
		 */
		std::string url;

		/**
		 * TODO Documentation
		 */
		std::string uuid;

		/**
		 * TODO Documentation
		 */
		bool is_optout_ads;

	};

	/**
	 * TODO Documentation
	 * @param json
	 * @return
	 */
	static Blog generateBlog(const char* json);

	/**
	 * TODO Documentation
	 * @param auth
	 * @param blogURL
	 */
	explicit TumblrAPI(std::string auth, std::string blogURL) : token(std::move(auth)), blogURL(std::move(blogURL)) {};

	/**
	 * TODO Documentation
	 * @param number
	 * @return
	 */
	cpr::Response getPostsJson(const unsigned int number) {
		return sendRequest("blog/" + blogURL + "/posts", true, "&npf=true&limit=" + std::to_string(number));
	};

	/**
	 * TODO Documentation
	 * @return
	 */
	cpr::Response getBlogInfoJson() { return sendRequest("blog/" + blogURL + "/info", true); };

	/**
	 * TODO Documentation
	 * @return
	 */
	cpr::Response getBlogAvatarJson() { return sendRequest("blog/" + blogURL + "/avatar", false); };

};


#endif //FOXWEBHOOK_TUMBLRAPI_HPP
