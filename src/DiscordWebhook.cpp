//
// Created by Spud on 4/24/2021.
//

#include "DiscordWebhook.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

cpr::Response DiscordWebhook::sendWebhook(const std::string &json) const {

	cpr::Body body = cpr::Body{json};
	cpr::Header header = cpr::Header{{"Content-Type", "application/json"}};
	cpr::Url url = cpr::Url{webhookURL};
	//cpr::SslOptions sslOpts = cpr::Ssl(cpr::ssl::TLSv1_3{});
	cpr::VerifySsl verifySsl = cpr::VerifySsl{false};
	cpr::Response response = cpr::Post(url, body, header, verifySsl);

	return response;

}

cpr::Response DiscordWebhook::send() {

	rapidjson::Document document;
	document.SetObject();

	rapidjson::MemoryPoolAllocator allocator = document.GetAllocator();

	if (!content.empty()) {
		document.AddMember("content", content, allocator);
	} else {
		document.AddMember("content", rapidjson::kNullType, allocator);
	}

	// TODO Add support for username override

	// TODO Add support for avatar override

	// TODO Add support for TTS

	if (!embeds.empty()) {
		rapidjson::Value embedsJson(rapidjson::kArrayType);
		for (const Embed& embed : embeds) {
			rapidjson::Value embedObjectJson(rapidjson::kObjectType);

			if (!embed.title.empty()) {
				embedObjectJson.AddMember("title", embed.title, allocator);
			}

			embedObjectJson.AddMember("type", "rich", allocator);

			if (!embed.description.empty()) {
				embedObjectJson.AddMember("description", embed.description, allocator);
			}

			if (!embed.url.empty()) {
				embedObjectJson.AddMember("url", embed.url, allocator);
			}

			if (!embed.timestamp.empty()) {
				embedObjectJson.AddMember("timestamp", embed.timestamp, allocator);
			}

			if (embed.color != 0) {
				embedObjectJson.AddMember("color", embed.color, allocator);
			}

			if (!embed.footer.text.empty()) {
				rapidjson::Value footer(rapidjson::kObjectType);

				footer.AddMember("text", embed.footer.text, allocator);

				if (!embed.footer.icon_url.empty()) {
					footer.AddMember("icon_url", embed.footer.icon_url, allocator);
				}

				if (!embed.footer.proxy_icon_url.empty()) {
					footer.AddMember("proxy_icon_url", embed.footer.proxy_icon_url, allocator);
				}

				embedObjectJson.AddMember("footer", footer, allocator);
			}

			if (!embed.image.url.empty()) {
				rapidjson::Value image(rapidjson::kObjectType);

				image.AddMember("url", embed.image.url, allocator);

				if (!embed.image.proxy_url.empty()) {
					image.AddMember("proxy_url", embed.image.proxy_url, allocator);
				}

				if (embed.image.height != 0) {
					image.AddMember("height", embed.image.height, allocator);
				}

				if (embed.image.width != 0) {
					image.AddMember("width", embed.image.width, allocator);
				}

				embedObjectJson.AddMember("image", image, allocator);
			}

			if (!embed.thumbnail.url.empty()) {
				rapidjson::Value thumbnail(rapidjson::kObjectType);

				thumbnail.AddMember("url", embed.thumbnail.url, allocator);

				if (!embed.thumbnail.proxy_url.empty()) {
					thumbnail.AddMember("proxy_url", embed.thumbnail.proxy_url, allocator);
				}

				if (embed.thumbnail.height != 0) {
					thumbnail.AddMember("height", embed.thumbnail.height, allocator);
				}

				if (embed.thumbnail.width != 0) {
					thumbnail.AddMember("width", embed.thumbnail.width, allocator);
				}

				embedObjectJson.AddMember("thumbnail", thumbnail, allocator);
			}


			// TODO Add video

			// TODO Add provider

			if (!embed.author.name.empty()) {
				rapidjson::Value author(rapidjson::kObjectType);

				author.AddMember("name", embed.author.name, allocator);

				if (!embed.author.url.empty()) {
					author.AddMember("url", embed.author.url, allocator);
				}

				if (!embed.author.icon_url.empty()) {
					author.AddMember("icon_url", embed.author.icon_url, allocator);
				}

				if (!embed.author.proxy_icon_url.empty()) {
					author.AddMember("proxy_icon_url", embed.author.proxy_icon_url, allocator);
				}

				embedObjectJson.AddMember("author", author, allocator);
			}

			if (!embed.fields.empty()) {
				rapidjson::Value fields(rapidjson::kArrayType);

				for (const Embed::Field& field : embed.fields) {

					if (field.name.empty() || field.value.empty()) {
						continue;
					}

					rapidjson::Value fieldJson(rapidjson::kObjectType);

					fieldJson.AddMember("name", field.name, allocator);
					fieldJson.AddMember("value", field.value, allocator);
					fieldJson.AddMember("inline", field.Inline, allocator);
					fields.PushBack(fieldJson, allocator);
				}

				embedObjectJson.AddMember("fields", fields, allocator);
			}

			embedsJson.PushBack(embedObjectJson, allocator);
		}

		document.AddMember("embeds", embedsJson, allocator);
	}

	// TODO Add support for mentions

	// TODO Add support for message components

	// TODO Add support for files

	// TODO Add support for payload

	// TODO Add support for attachments.

	// TODO Add support for flags

	// TODO Add support for threads

	rapidjson::StringBuffer buffer;
	rapidjson::Writer <rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);

	return sendWebhook(buffer.GetString()); // TODO Clear message and embeds once sent?
}
