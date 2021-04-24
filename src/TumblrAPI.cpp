//
// Created by Spud on 4/23/2021.
//

#include "TumblrAPI.hpp"
#include <iostream>

cpr::Response
TumblrAPI::sendRequest(const std::string &endpoint, bool authRequired, const std::string &optionalParameters = "") {

	// Format the URL to go to for retrieving data.
	std::string url = "api.tumblr.com/v2/" + endpoint;
	if (authRequired) {
		url += "?api_key=" + token;
	}
	url += optionalParameters;
	std::cout << url << std::endl;

	// Get the response from the URL.
	cpr::Response response;
	response = cpr::Get(cpr::Url{url});
	return response;
}

cpr::Response TumblrAPI::getPosts(unsigned int number) {
	return sendRequest("blog/" + blogURL + "/posts", true, "&npf=true&limit=" + std::to_string(number));
}

cpr::Response TumblrAPI::getBlogInfo() {
	return sendRequest("blog/" + blogURL + "/info", true);
}

cpr::Response TumblrAPI::getBlogAvatar() {
	return sendRequest("blog/" + blogURL + "/avatar", false);
}
