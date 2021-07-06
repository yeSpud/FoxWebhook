//
// Created by Spud on 4/25/2021.
//

#ifndef FOXWEBHOOK_BLOG_HPP
#define FOXWEBHOOK_BLOG_HPP

#include "Content.hpp"
#include <vector>

class Blog {

public:

	/**
	 * TODO Documentation
	 */
	Blog() = default;;

	static Blog generateBlog(const char* json);

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] bool getAsk() const { return ask; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] bool getAsk_anon() const { return ask_anon; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getAsk_page_title() { return ask_page_title; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] bool getAsks_allow_media() const { return asks_allow_media; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::vector<Content::Image> getAvatars() { return avatar; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] bool getCan_chat() const { return can_chat; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] bool getCan_subscribe() const { return can_subscribe; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getDescription() { return description; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] bool getIs_nsfw() const { return is_nsfw; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getName() { return name; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] uint64_t getPosts() const { return posts; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] bool getShared_likes() const { return shared_likes; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] bool getSubscribed() const { return subscribed; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getTitle() { return title; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] uint64_t getTotal_posts() const { return total_posts; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] uint64_t getUpdated() const { return updated; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getURL() { return url; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getUUID() { return uuid; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] bool getIs_optout_ads() const { return is_optout_ads; };

private:

	/**
	 * TODO Documentation
	 * @param json
	 */
	explicit Blog(const char* json);

	/**
	 * TODO Documentation
	 */
	bool ask{};

	/**
	 * TODO Documentation
	 */
	bool ask_anon{};

	/**
	 * TODO Documentation
	 */
	std::string ask_page_title;

	/**
	 * TODO Documentation
	 */
	bool asks_allow_media{};

	/**
	 * TODO Documentation
	 */
	std::vector<Content::Image> avatar;

	/**
	 * TODO Documentation
	 */
	bool can_chat{};

	/**
	 * TODO Documentation
	 */
	bool can_subscribe{};

	/**
	 * TODO Documentation
	 */
	std::string description;

	/**
	 * TODO Documentation
	 */
	bool is_nsfw{};

	/**
	 * TODO Documentation
	 */
	std::string name;

	/**
	 * TODO Documentation
	 */
	uint64_t posts{};

	/**
	 * TODO Documentation
	 */
	bool shared_likes{};

	/**
	 * TODO Documentation
	 */
	bool subscribed{};

	// auto theme

	/**
	 * TODO Documentation
	 */
	std::string title;

	/**
	 * TODO Documentation
	 */
	uint64_t total_posts{};

	/**
	 * TODO Documentation
	 */
	uint64_t updated{};

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
	bool is_optout_ads{};

};


#endif //FOXWEBHOOK_BLOG_HPP
