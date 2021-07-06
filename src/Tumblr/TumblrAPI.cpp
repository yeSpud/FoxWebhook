//
// Created by Spud on 4/23/2021.
//

#include "TumblrAPI.hpp"

TumblrAPI::Blog TumblrAPI::generateBlog(const char *json) { // TODO Comments

	Blog blog;

	rapidjson::Document document;

	document.Parse(json);

	if (document.HasMember("response")) {
		auto response = document["response"].GetObj();

		if (response.HasMember("blog")) {

			auto blogjson = response["blog"].GetObj();

			blog.ask = TumblrAPI::getBool(blogjson, "ask");
			blog.ask_anon = TumblrAPI::getBool(blogjson, "ask_anon");
			blog.ask_page_title = TumblrAPI::getString(blogjson, "ask_page_title");
			blog.asks_allow_media = TumblrAPI::getBool(blogjson, "asks_allow_media");

			// Avatars
			if (blogjson.HasMember("avatar")) {
				for (const auto &avatarEntry : blogjson["avatar"].GetArray()) {
					if (avatarEntry.IsObject()) {
						std::string avatarurl;
						unsigned int width, height;

						if (!Content::entryHasString(avatarEntry, "url", avatarurl)) {
							continue;
						}

						if (!Content::entryHasInt(avatarEntry, "width", width)) {
							continue;
						}

						if (!Content::entryHasInt(avatarEntry, "height", height)) {
							continue;
						}

						Content::Image image;
						image.width = width;
						image.height = height;
						image.url = avatarurl;
						blog.avatar.push_back(image);
					}
				}
			}

			blog.can_chat = TumblrAPI::getBool(blogjson, "can_chat");
			blog.can_subscribe = TumblrAPI::getBool(blogjson, "can_subscribe");
			blog.description = TumblrAPI::getString(blogjson, "description");
			blog.is_nsfw = TumblrAPI::getBool(blogjson, "is_nsfw");
			blog.name = TumblrAPI::getString(blogjson, "name");
			blog.posts = TumblrAPI::getNumber(blogjson, "posts");
			blog.shared_likes = TumblrAPI::getBool(blogjson, "shared_likes");
			blog.subscribed = TumblrAPI::getBool(blogjson, "subscribed");

			blog.title = TumblrAPI::getString(blogjson, "title");
			blog.total_posts = TumblrAPI::getNumber(blogjson, "total_posts");
			blog.updated = TumblrAPI::getNumber(blogjson, "updated");
			blog.url = TumblrAPI::getString(blogjson, "url");
			blog.uuid = TumblrAPI::getString(blogjson, "uuid");
			blog.is_optout_ads = TumblrAPI::getBool(blogjson, "is_optout_ads");
		}
	}

	return blog;

}
