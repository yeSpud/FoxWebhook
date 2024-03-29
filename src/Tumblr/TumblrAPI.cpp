//
// Created by Spud on 4/23/2021.
//

#include "TumblrAPI.hpp"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

cpr::Response
TumblrAPI::sendRequest(const std::string &endpoint, bool authRequired, const std::string &optionalParameters = "") {

	// Format the URL to go to for retrieving data.
	std::string url = "api.tumblr.com/v2/" + endpoint;
	if (authRequired) {
		url += "?api_key=" + token;
	}
	url += optionalParameters;

	// Get the response from the URL.
	cpr::Response response;
	response = cpr::Get(cpr::Url{url});
	return response;
}

std::vector<Post> TumblrAPI::getPosts(unsigned int number) { // TODO Comments

	std::vector<Post> posts;
	rapidjson::Document document;

	// Get the json from the network request.
	cpr::Response json = sendRequest("blog/" + blogURL + "/posts", true, "&npf=true&limit=" + std::to_string(number));
	document.Parse(json.text.c_str());

	// Get the response section of the request (if it has it).
	if (document.HasMember("response")) {
		rapidjson::GenericObject response = document["response"].GetObj();

		// Gets the posts array from the response.
		if (response.HasMember("posts")) {

			for (const auto &entry : response["posts"].GetArray()) {
				if (entry.IsObject()) {
					rapidjson::StringBuffer buffer;
					rapidjson::Writer <rapidjson::StringBuffer> writer(buffer);
					entry.Accept(writer);
					Post post = Post(buffer.GetString());
					posts.push_back(post);
				}
			}
		}
	}

	return posts;
}

Blog TumblrAPI::getBlogInfo() { // TODO Comments
	rapidjson::Document document;
	Blog blog;

	cpr::Response json = sendRequest("blog/" + blogURL + "/info", true);
	document.Parse(json.text.c_str());

	if (document.HasMember("response")) {
		rapidjson::GenericObject response = document["response"].GetObj();

		if (response.HasMember("blog")) {
			rapidjson::StringBuffer buffer;
			rapidjson::Writer <rapidjson::StringBuffer> writer(buffer);
			response["blog"].Accept(writer);
			blog = Blog(buffer.GetString());
		}
	}

	return blog;
}

cpr::Response TumblrAPI::getBlogAvatar() {
	return sendRequest("blog/" + blogURL + "/avatar", false);
}
