//
// Created by Spud on 4/25/2021.
//

#ifndef FOXWEBHOOK_BLOG_HPP
#define FOXWEBHOOK_BLOG_HPP

#include <string>

class Blog {

private:

	bool ask;

	bool ask_anon;

	std::string ask_page_title;

	bool asks_allow_media;

	std::vector<Image> avatar;

};


#endif //FOXWEBHOOK_BLOG_HPP
