//
// Created by Spud on 4/24/2021.
//

#ifndef FOXWEBHOOK_POST_HPP
#define FOXWEBHOOK_POST_HPP

#include "Image.hpp"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <vector>

class Post {

public:

	/**
	 * TODO Documentation
	 */
	Post() : type("blocks"), original_type("regular"), id(0), id_string("0"), timestamp(0), state("published"),
	         should_open_in_legacy(false), note_count(0), can_like(false), can_reblog(false),
	         can_send_in_message(false), can_reply(false), display_avatar(false) {};

	/**
	 * TODO Documentation
	 * @param json
	 * @return
	 */
	static std::vector<Post> generatePosts(const char* json);

	/**
	 * TODO Documentation
	 * @param entry
	 * @param value
	 * @param defaultValue
	 * @return
	 */
	static std::string getString(rapidjson::Document &entry, const std::string &value, std::string defaultValue);

	/**
	 * TODO Documentation
	 * @param entry
	 * @param value
	 * @return
	 */
	static uint64_t getInt(rapidjson::Document &entry, const std::string &value);

	/**
	 * TODO Documentation
	 * @param entry
	 * @param value
	 * @return
	 */
	static bool getBool(rapidjson::Document &entry, const std::string &value);

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getType() { return type; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getOriginal_type() { return original_type; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getBlog_name() { return blog_name; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] uint64_t getId() const { return id; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getId_string() { return id_string; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getPost_url() { return post_url; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getSlug() { return slug; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getDate() { return date; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] uint64_t getTimestamp() const { return timestamp; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getState() { return state; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getReblog_key() { return reblog_key; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getShort_url() { return short_url; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getSummary() { return summary; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] bool getShould_open_in_legacy() const { return should_open_in_legacy; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] uint64_t getNote_count() const { return note_count; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::vector<Image> getContent() { return content; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] bool getCan_like() const { return can_like; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] bool getCan_reblog() const { return can_reblog; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] bool getCan_send_in_message() const { return can_send_in_message; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] bool getCan_reply() const { return can_reply; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] bool getDisplay_avatar() const { return display_avatar; };

	/**
	 * TODO Documentation
	 * @param p
	 * @return
	 */
	bool operator!=(const Post &p);

private:

	/**
	 * TODO Documentation
	 * @param postJson
	 */
	explicit Post(const char* json);

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
	uint64_t id;

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
	uint64_t timestamp;

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
	uint64_t note_count;

	/**
	 * TODO Documentation
	 */
	std::vector<Image> content;

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
};


#endif //FOXWEBHOOK_POST_HPP
