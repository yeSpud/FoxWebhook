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

	struct Theme {
		// TODO Used by blog info
	};

	struct Text {
		// TODO
	};

	struct Image {

		/**
		 * TODO Documentation
		 */
		std::string type;

		/**
		 * TODO Documentation
		 */
		std::string media_key;

		/**
		 * TODO Documentation
		 */
		unsigned int width;

		/**
		 * TODO Documentaiton
		 */
		unsigned int height;

		/**
		 * TODO Documentation
		 */
		std::string url;

		/**
		 * TODO Documentation
		 */
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
