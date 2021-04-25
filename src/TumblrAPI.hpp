//
// Created by Spud on 4/23/2021.
//

#ifndef FOXWEBHOOK_TUMBLRAPI_HPP
#define FOXWEBHOOK_TUMBLRAPI_HPP

#include <string>
#include <utility>
#include <cpr/cpr.h>

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
	cpr::Response sendRequest(const std::string &endpoint, bool authRequired, const std::string& optionalParameters);

public:

	/**
	 * TODO Documentation
	 * @param auth
	 * @param blogURL
	 */
	explicit TumblrAPI(std::string auth, std::string blogURL):token(std::move(auth)), blogURL(std::move(blogURL)){};

	/**
	 * TODO Documentation
	 * @param number
	 * @return
	 */
	cpr::Response getPosts(unsigned int number);

	/**
	 * TODO Documentation
	 * @return
	 */
	cpr::Response getMostRecentPost(){return getPosts(1);};

	/**
	 * TODO Documentation
	 * @return
	 */
	cpr::Response getBlogInfo();

	/**
	 * TODO Documentation
	 * @return
	 */
	cpr::Response getBlogAvatar();

};


#endif //FOXWEBHOOK_TUMBLRAPI_HPP