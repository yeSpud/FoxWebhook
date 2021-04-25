//
// Created by Spud on 4/24/2021.
//

#ifndef FOXWEBHOOK_CONTENT_HPP
#define FOXWEBHOOK_CONTENT_HPP

#include <string>
#include <utility>

class Content {

public:

	explicit Content(std::string type): type(std::move(type)){};

private:

	const std::string type;

};


#endif //FOXWEBHOOK_CONTENT_HPP
