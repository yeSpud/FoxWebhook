//
// Created by Spud on 4/24/2021.
//

#include "Post.hpp"
#include <iostream>

Post::Post(const std::string &postJson) {

	// Ingest the string as a json DOM
	rapidjson::Document document;
	document.Parse(postJson.c_str());

	type = getString(document, "type", "blocks");
	original_type = getString(document, "original_type", "regular");
	blog_name = getString(document, "blog_name", "");

	id = getInt(document, "id");
	id_string = getString(document, "id_string", "0");
	post_url = getString(document, "post_url", "");
	slug = getString(document, "slug", "");
	date = getString(document, "date", "");
	timestamp = getInt(document, "timestamp");
	state = getString(document, "state", "published");
	reblog_key = getString(document, "reblog_key", "");

	short_url = getString(document, "short_url", "");
	summary = getString(document, "summary", "");
	should_open_in_legacy = getBool(document, "should_open_in_legacy");
	note_count = getInt(document, "note_count");

	// TODO Image
	if (document.HasMember("content")) {
		for (const auto& contentEntry : document["content"].GetArray()) {
			if (contentEntry.HasMember("media")) {
				for (const auto& mediaEntry : contentEntry["media"].GetArray()) {
					if (mediaEntry.IsObject()) {
						std::string media_key, url;
						unsigned int width, height;

						if (!Image::entryHasString(mediaEntry, "media_key", media_key)) {
							continue;
						}

						if (!Image::entryHasInt(mediaEntry, "width", width)) {
							continue;
						}

						if (!Image::entryHasInt(mediaEntry, "height", height)) {
							continue;
						}

						if (!Image::entryHasString(mediaEntry, "url", url)) {
							continue;
						}

						bool has_original_dimensions = mediaEntry.HasMember("has_original_dimensions");

						Image c = Image(media_key, width, height, url, has_original_dimensions);
						content.push_back(c);
					}
				}
			}
		}
	}

	can_like = getBool(document, "can_like");
	can_reblog = getBool(document, "can_reblog");
	can_send_in_message = getBool(document, "can_send_in_message");
	can_reply = getBool(document, "can_reply");
	display_avatar = getBool(document, "display_avatar");

	//std::cout << postJson << std::endl;

}

std::string Post::getString(rapidjson::Document &entry, const std::string &value, std::string defaultValue) {
	if (entry.HasMember(value.c_str())) {
		return entry[value.c_str()].GetString();
	} else {
		return defaultValue;
	}
}

uint64_t Post::getInt(rapidjson::Document &entry, const std::string &value) {
	if (entry.HasMember(value.c_str())) {
		return entry[value.c_str()].GetUint64();
	} else {
		return 0;
	}
}

bool Post::getBool(rapidjson::Document &entry, const std::string &value) {
	if (entry.HasMember(value.c_str())) {
		return entry[value.c_str()].GetBool();
	} else {
		return false;
	}
}
