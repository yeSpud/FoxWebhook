//
// Created by Spud on 4/25/2021.
//

#ifndef FOXWEBHOOK_BLOG_HPP
#define FOXWEBHOOK_BLOG_HPP

#include <string>
#include "Image.hpp"
#include <vector>

class Blog {

public:

	Blog(){};

	explicit Blog(const std::string &blogJson);

	bool getAsk() { return ask; };

	bool getAsk_anon() { return ask_anon; };

	std::string getAsk_page_title() { return ask_page_title; };

	bool getAsks_allow_media() { return asks_allow_media; };

	std::vector<Image> getAvatars() { return avatar; };

	bool getCan_chat() { return can_chat; };

	bool getCan_subscribe() { return can_subscribe; };

	std::string getDescription() { return description; };

	bool getIs_nsfw() { return is_nsfw; };

	std::string getName() { return name; };

	uint64_t getPosts() { return posts; };

	bool getShared_likes() { return shared_likes; };

	bool getSubscribed() { return subscribed; };

	std::string getTitle() { return title; };

	uint64_t getTotal_posts() { return total_posts; };

	uint64_t getUpdated() { return updated; };

	std::string getURL() { return url; };

	std::string getUUID() { return uuid; };

	bool getIs_optout_ads() { return is_optout_ads; };

private:

	bool ask{};

	bool ask_anon{};

	std::string ask_page_title;

	bool asks_allow_media{};

	std::vector<Image> avatar;

	bool can_chat{};

	bool can_subscribe{};

	std::string description;

	bool is_nsfw{};

	std::string name;

	uint64_t posts{};

	bool shared_likes{};

	bool subscribed{};

	// auto theme

	std::string title;

	uint64_t total_posts{};

	uint64_t updated{};

	std::string url;

	std::string uuid;

	bool is_optout_ads{};

};


#endif //FOXWEBHOOK_BLOG_HPP
