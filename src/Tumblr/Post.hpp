//
// Created by Spud on 4/24/2021.
//

#ifndef FOXWEBHOOK_POST_HPP
#define FOXWEBHOOK_POST_HPP

#include "Image.hpp"
#include <vector>

class Post {

public:

	explicit Post(const std::string &postJson);

	static std::string getString(rapidjson::Document &entry, const std::string &value, std::string defaultValue);

	static uint64_t getInt(rapidjson::Document &entry, const std::string &value);

	static bool getBool(rapidjson::Document &entry, const std::string &value);

	std::string getType() { return type; };

	std::string getOriginal_type() { return original_type; };

	std::string getBlog_name() { return blog_name; };

	uint64_t getId() { return id; };

	std::string getId_string() { return id_string; };

	std::string getPost_url() { return post_url; };

	std::string getSlug() { return slug; };

	std::string getDate() { return date; };

	uint64_t getTimestamp() { return timestamp; };

	std::string getState() { return state; };

	std::string getReblog_key() { return reblog_key; };

	std::string getShort_url() { return short_url; };

	std::string getSummary() { return summary; };

	bool getShould_open_in_legacy() { return should_open_in_legacy; };

	uint64_t getNote_count() { return note_count; };

	std::vector<Image> getContent() { return content; };

	bool getCan_like() { return can_like; };

	bool getCan_reblog() { return can_reblog; };

	bool getCan_send_in_message() { return can_send_in_message; };

	bool getCan_reply() { return can_reply; };

	bool getDisplay_avatar() { return display_avatar; };

private:

	std::string type;

	std::string original_type;

	std::string blog_name;

	// const auto blog;

	uint64_t id;

	std::string id_string;

	std::string post_url;

	std::string slug;

	std::string date;

	uint64_t timestamp;

	std::string state;

	std::string reblog_key;

	// const auto tags;

	std::string short_url;

	std::string summary;

	bool should_open_in_legacy;

	uint64_t note_count;

	std::vector<Image> content;

	// const auto layout;

	// const auto trail;

	bool can_like;

	bool can_reblog;

	bool can_send_in_message;

	bool can_reply;

	bool display_avatar;
};


#endif //FOXWEBHOOK_POST_HPP
