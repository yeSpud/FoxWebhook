//
// Created by Spud on 4/23/2021.
//

#ifndef FOXWEBHOOK_TUMBLRAPI_HPP
#define FOXWEBHOOK_TUMBLRAPI_HPP

#include <cpr/cpr.h>
#include <spdlog/spdlog.h>
#include "Blog.hpp"
#include "Post.hpp"

class TumblrAPI {

private:

	/**
	 * TODO Documentation
	 */
	const std::string token;

	/**
	 * TODO Documentation
	 */
	const std::string blogURL;

	/**
	 * TODO Documentation
	 * @param endpoint
	 * @param authRequired
	 * @param optionalParams
	 * @return
	 */
	cpr::Response sendRequest(const std::string &endpoint, bool authRequired, const std::string &optionalParams = "") {

		// Get our logger.
		std::shared_ptr<spdlog::logger> logger = spdlog::get("Logger");

		// Format the URL to go to for retrieving data.
		std::string url = "api.tumblr.com/v2/" + endpoint;
		if (authRequired) {
			url += "?api_key=" + token;
		}
		url += optionalParams;

		// Get the response from the URL.
		logger->debug(fmt::format("Querying url: {}", url));
		cpr::Response response;
		response = cpr::Get(cpr::Url{url});

		// If check the response code.
		if (response.status_code != 200) {
			logger->warn(fmt::format("Status code {} - {}: {}", response.status_code, response.reason, response.text));
		}

		return response;
	}

public:

	/**
	 * TODO Documentation
	 * @param auth
	 * @param blogURL
	 */
	explicit TumblrAPI(std::string auth, std::string blogURL) : token(std::move(auth)), blogURL(std::move(blogURL)) {};

	/**
	 * TODO Documentation
	 * @param number
	 * @return
	 */
	cpr::Response getPostsJson(const unsigned int number) {
		return sendRequest("blog/" + blogURL + "/posts", true, "&npf=true&limit=" + std::to_string(number));
	};

	/**
	 * TODO Documentation
	 * @return
	 */
	cpr::Response getBlogInfoJson() { return sendRequest("blog/" + blogURL + "/info", true); };

	/**
	 * TODO Documentation
	 * @return
	 */
	cpr::Response getBlogAvatarJson() { return sendRequest("blog/" + blogURL + "/avatar", false); };

};


#endif //FOXWEBHOOK_TUMBLRAPI_HPP
