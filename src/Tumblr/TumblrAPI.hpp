//
// Created by Spud on 4/23/2021.
//

#ifndef FOXWEBHOOK_TUMBLRAPI_HPP
#define FOXWEBHOOK_TUMBLRAPI_HPP

#include <cpr/cpr.h>
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
	 * @return
	 */
	cpr::Response sendRequest(const std::string &endpoint, bool authRequired, const std::string &optionalParameters);

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
	//[[deprecated]]
	//std::vector<Post> getPosts(unsigned int number);

	/**
	 * TODO Documentation
	 * @param number
	 * @return
	 */
	cpr::Response getPostsJson(const unsigned int number) {
		return sendRequest("blog/" + blogURL + "/posts",
		                   true, "&npf=true&limit=" + std::to_string(number));
	};

	/**
	 * TODO Documentation
	 * @return
	 */
	//[[deprecated]]
	//Blog getBlogInfo();

	/**
	 * TODO Documentation
	 * @return
	 */
	cpr::Response getBlogInfoJson() { return sendRequest("blog/" + blogURL + "/info", true, ""); };

	/**
	 * TODO Documentation
	 * @return
	 */
	cpr::Response getBlogAvatarJson() { return sendRequest("blog/" + blogURL + "/avatar", false, ""); };

};


#endif //FOXWEBHOOK_TUMBLRAPI_HPP
