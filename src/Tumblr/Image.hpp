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

	Image(std::string media_key, unsigned int width, unsigned int height, std::string url,
	        bool has_original_dimensions) : media_key(std::move(media_key)), type("image"), width(width),
	                                        height(height), url(std::move(url)),
	                                        has_original_dimensions(has_original_dimensions) {};

	std::string getType() { return type; };

	std::string getMedia_key() { return media_key; };

	unsigned int getWidth() { return width; };

	unsigned int getHeight() { return height; };

	std::string getUrl() { return url; };

	bool getHas_original_dimensions() { return has_original_dimensions; };

	static bool entryHasString(const rapidjson::Value &entry, const char *value, std::string &buffer);

	static bool entryHasInt(const rapidjson::Value &entry, const char *value, unsigned int &buffer);

private:

	const std::string type;

	const std::string media_key;

	const unsigned int width;

	const unsigned int height;

	const std::string url;

	bool has_original_dimensions;

};


#endif //FOXWEBHOOK_IMAGE_HPP
