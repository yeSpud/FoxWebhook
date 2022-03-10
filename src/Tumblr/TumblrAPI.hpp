//
// Created by Spud on 4/23/2021.
//

#ifndef FOXWEBHOOK_TUMBLRAPI_HPP
#define FOXWEBHOOK_TUMBLRAPI_HPP

#include <cpr/cpr.h>
#include <spdlog/spdlog.h>
#include "Content.hpp"

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
	cpr::Response sendRequest(const std::string &endpoint, bool authRequired, const std::string &optionalParams = "");

	/**
	 * TODO Documentation & comments
	 * @param entry
	 * @param value
	 * @return
	 */
	static bool getBool(auto &entry, const char *value) {
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
	static std::string getString(auto &entry, const char *value) {
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
	static uint64_t getNumber(auto &entry, const char *value) {
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

	struct Post {

		/**
		 * TODO Documentation
		 */
		std::string type;

		/**
		 * TODO Documentation
		 */
		std::string original_type;

		/**
		 * TODO Documentation
		 */
		std::string blog_name;

		// const auto blog;

		/**
		 * TODO Documentation
		 */
		uint64_t id = 0;

		/**
		 * TODO Documentation
		 */
		std::string id_string;

		/**
		 * TODO Documentation
		 */
		std::string post_url;

		/**
		 * TODO Documentation
		 */
		std::string slug;

		/**
		 * TODO Documentation
		 */
		std::string date;

		/**
		 * TODO Documentation
		 */
		uint64_t timestamp = 0;

		/**
		 * TODO Documentation
		 */
		std::string state;

		/**
		 * TODO Documentation
		 */
		std::string reblog_key;

		// const auto tags;

		/**
		 * TODO Documentation
		 */
		std::string short_url;

		/**
		 * TODO Documentation
		 */
		std::string summary;

		/**
		 * TODO Documentation
		 */
		bool should_open_in_legacy;

		/**
		 * TODO Documentation
		 */
		uint64_t note_count = 0;

		/**
		 * TODO Documentation
		 */
		std::vector<Content::Image> content;

		// const auto layout;

		// const auto trail;

		/**
		 * TODO Documentation
		 */
		bool can_like;

		/**
		 * TODO Documentation
		 */
		bool can_reblog;

		/**
		 * TODO Documentation
		 */
		bool can_send_in_message;

		/**
		 * TODO Documentation
		 */
		bool can_reply;

		/**
		 * TODO Documentation
		 */
		bool display_avatar;


		/**
		 * TODO Documentation
		 * @param p
		 * @return
		 */
		bool operator!=(const Post &p) const {
			bool idCheck = id == p.id;
			bool idStringCheck = id_string == p.id_string;
			bool urlCheck = post_url == p.post_url;
			return !idCheck && !idStringCheck && !urlCheck;
		}

		/**
		 * TODO Documentation
		 * @param p
		 * @return
		 */
		bool operator==(const Post &p) const {
			bool idCheck = id == p.id;
			bool idStringCheck = id_string == p.id_string;
			bool urlCheck = post_url == p.post_url;
			return idCheck && idStringCheck && urlCheck;
		}
	};

	/**
	 * TODO Documentation
	 * @param json
	 * @return
	 */
	static Blog generateBlog(const char *json);

	static std::vector<Post> generatePosts(const char *json);

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
