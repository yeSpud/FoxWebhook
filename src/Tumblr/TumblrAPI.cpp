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

std::vector<TumblrAPI::Post> TumblrAPI::generatePosts(const char *json) { // TODO Comments

	std::vector<Post> posts;
	rapidjson::Document document;

	document.Parse(json);

	// Get the response section of the request (if it has it).
	if (document.HasMember("response")) {
		auto response = document["response"].GetObj();

		// Gets the posts array from the response.
		if (response.HasMember("posts")) {

			for (const auto &entry : response["posts"].GetArray()) {
				if (entry.IsObject()) {

					auto postjson = entry.GetObj();
					Post post;

					post.type = TumblrAPI::getString(postjson, "type");
					post.original_type = TumblrAPI::getString(postjson, "original_type");
					post.blog_name = TumblrAPI::getString(postjson, "blog_name");

					post.id = TumblrAPI::getNumber(postjson, "id");
					post.id_string = TumblrAPI::getString(postjson, "id_string");
					post.post_url = TumblrAPI::getString(postjson, "post_url");
					post.slug = TumblrAPI::getString(postjson, "slug");
					post.date = TumblrAPI::getString(postjson, "date");
					post.timestamp = TumblrAPI::getNumber(postjson, "timestamp");
					post.state = TumblrAPI::getString(postjson, "state");
					post.reblog_key = TumblrAPI::getString(postjson, "reblog_key");

					post.short_url = TumblrAPI::getString(postjson, "short_url");
					post.summary = TumblrAPI::getString(postjson, "summary");
					post.should_open_in_legacy = TumblrAPI::getBool(postjson, "should_open_in_legacy");
					post.note_count = TumblrAPI::getNumber(postjson, "note_count");

					// Image
					if (postjson.HasMember("content")) {
						for (const auto &contentEntry : postjson["content"].GetArray()) {
							if (contentEntry.HasMember("media")) {
								for (const auto &mediaEntry : contentEntry["media"].GetArray()) {
									if (mediaEntry.IsObject()) {
										std::string media_key, media_type, url;
										unsigned int width, height;

										if (!Content::entryHasString(mediaEntry, "media_key", media_key)) {
											continue;
										}

										if (!Content::entryHasString(mediaEntry, "type", media_type)) {
											continue;
										}

										if (!Content::entryHasInt(mediaEntry, "width", width)) {
											continue;
										}

										if (!Content::entryHasInt(mediaEntry, "height", height)) {
											continue;
										}

										if (!Content::entryHasString(mediaEntry, "url", url)) {
											continue;
										}

										bool has_original_dimensions = mediaEntry.HasMember("has_original_dimensions");

										Content::Image image;
										image.media_key = media_key;
										image.type = media_type;
										image.width = width;
										image.height = height;
										image.url = url;
										image.has_original_dimensions = has_original_dimensions;
										post.content.push_back(image);
									}
								}
							}
						}
					}

					post.can_like = TumblrAPI::getBool(postjson, "can_like");
					post.can_reblog = TumblrAPI::getBool(postjson, "can_reblog");
					post.can_send_in_message = TumblrAPI::getBool(postjson, "can_send_in_message");
					post.can_reply = TumblrAPI::getBool(postjson, "can_reply");
					post.display_avatar = TumblrAPI::getBool(postjson, "display_avatar");

					posts.push_back(post);
				}
			}
		}
	}

	return posts;

}
