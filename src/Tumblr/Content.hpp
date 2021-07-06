//
// Created by Spud on 7/6/21.
//

#ifndef FOXWEBHOOK_CONTENT_HPP
#define FOXWEBHOOK_CONTENT_HPP

#include <string>
#include <utility>
#include <rapidjson/document.h>


class Content {

public:

	struct Text {
		// TODO
	};

	struct Image {

		std::string type;
		std::string media_key;
		unsigned int width;
		unsigned int height;
		std::string url;
		bool has_original_dimensions = false;

	};

	struct Link {
		// TODO
	};

	struct Audio {
		// TODO
	};

	struct Video {
		// TODO
	};

	/**
	 * TODO Documentation
	 * @param entry
	 * @param value
	 * @param buffer
	 * @return
	 */
	static bool entryHasString(const rapidjson::Value &entry, const char *value, std::string &buffer);

	/**
	 * TODO Documentation
	 * @param entry
	 * @param value
	 * @param buffer
	 * @return
	 */
	static bool entryHasInt(const rapidjson::Value &entry, const char *value, unsigned int &buffer);

};


#endif //FOXWEBHOOK_CONTENT_HPP
