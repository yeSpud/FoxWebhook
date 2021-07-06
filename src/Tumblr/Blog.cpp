//
// Created by Spud on 4/25/2021.
//

#include "Blog.hpp"
#include "Post.hpp"

Blog Blog::generateBlog(const char *json) { // TODO Comments

	Blog blog;
	rapidjson::Document document;

	document.Parse(json);

	if (document.HasMember("response")) {
		auto response = document["response"].GetObj();

		if (response.HasMember("blog")) {
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			response["blog"].Accept(writer);
			blog = Blog(buffer.GetString());
		}
	}

	return blog;

}

Blog::Blog(const char* json) { // TODO Comments

	rapidjson::Document document;
	document.Parse(json);

	ask = Post::getBool(document, "ask");
	ask_anon = Post::getBool(document, "ask_anon");
	ask_page_title = Post::getString(document, "ask_page_title", "");
	asks_allow_media = Post::getBool(document, "asks_allow_media");

	// Avatars
	if (document.HasMember("avatar")) {
		for (const auto &avatarEntry : document["avatar"].GetArray()) {
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
				avatar.push_back(image);
			}
		}
	}

	can_chat = Post::getBool(document, "can_chat");
	can_subscribe = Post::getBool(document, "can_subscribe");
	description = Post::getString(document, "description", "");
	is_nsfw = Post::getBool(document, "is_nsfw");
	name = Post::getString(document, "name", "");
	posts = Post::getInt(document, "posts");
	shared_likes = Post::getBool(document, "shared_likes");
	subscribed = Post::getBool(document, "subscribed");

	title = Post::getString(document, "title", "");
	total_posts = Post::getInt(document, "total_posts");
	updated = Post::getInt(document, "updated");
	url = Post::getString(document, "url", "");
	uuid = Post::getString(document, "uuid", "");
	is_optout_ads = Post::getBool(document, "is_optout_ads");

}
