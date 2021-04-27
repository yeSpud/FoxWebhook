//
// Created by Spud on 4/24/2021.
//

#ifndef FOXWEBHOOK_IMAGE_HPP
#define FOXWEBHOOK_IMAGE_HPP

#include <string>
#include <utility>
#include <rapidjson/document.h>

class Image {

public:

	/**
	 * TODO Documentation
	 * @param media_key
	 * @param width
	 * @param height
	 * @param url
	 * @param has_original_dimensions
	 */
	Image(std::string media_key, unsigned int width, unsigned int height, std::string url,
	      bool has_original_dimensions) : media_key(std::move(media_key)), type("image"), width(width),
	                                      height(height), url(std::move(url)),
	                                      has_original_dimensions(has_original_dimensions) {};

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getType() { return type; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getMedia_key() { return media_key; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] unsigned int getWidth() const { return width; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] unsigned int getHeight() const { return height; };

	/**
	 * TODO Documentation
	 * @return
	 */
	std::string getUrl() { return url; };

	/**
	 * TODO Documentation
	 * @return
	 */
	[[nodiscard]] bool getHas_original_dimensions() const { return has_original_dimensions; };

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

private:

	/**
	 * TODO Documentation
	 */
	const std::string type;

	/**
	 * TODO Documentation
	 */
	const std::string media_key;

	/**
	 * TODO Documentation
	 */
	const unsigned int width;

	/**
	 * TODO Documentation
	 */
	const unsigned int height;

	/**
	 * TODO Documentation
	 */
	const std::string url;

	/**
	 * TODO Documentation
	 */
	bool has_original_dimensions;

};


#endif //FOXWEBHOOK_IMAGE_HPP
