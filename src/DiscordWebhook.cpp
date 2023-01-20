//
// Created by Spud on 4/24/2021.
//

#include "DiscordWebhook.hpp"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

cpr::Response DiscordWebhook::sendWebhook(const std::string &json) { // TODO Comments

	cpr::Body body = cpr::Body{json};
	cpr::Header header = cpr::Header{{"Content-Type", "application/json"}};
	cpr::Url url = cpr::Url{webhookURL};
	//cpr::SslOptions sslOpts = cpr::Ssl(cpr::ssl::TLSv1_3{});
	cpr::VerifySsl verifySsl = cpr::VerifySsl{false};
	cpr::Response response = cpr::Post(url, body, header, verifySsl);

	return response;

}

cpr::Response DiscordWebhook::sendMessage(const std::string &message) { // TODO Comments

	std::string json = formatJson(message, "", "", "", "");

	return sendWebhook(json);
}

cpr::Response
DiscordWebhook::sendEmbed(const std::string &blogname, const std::string &postURL, const std::string &blogAvatar,
                          const std::string &postImageURL) { // TODO Comments

	std::string json = formatJson("", blogname, postURL, blogAvatar, postImageURL);

	return sendWebhook(json);
}

std::string DiscordWebhook::formatJson(const std::string &message, const std::string &blogname,
                                       const std::string &postURL, const std::string &blogAvatar,
                                       const std::string &postImageURL) { // TODO Comments

	rapidjson::Document document;
	document.SetObject();

	if (!message.empty()) {
		setStringKeyValueObject("content", message, document, document.GetAllocator());
	}

	if (!blogname.empty() && !postURL.empty() && !blogAvatar.empty() && !postImageURL.empty()) {
		rapidjson::Value embedArray(rapidjson::kArrayType);
		rapidjson::Value embedObject(rapidjson::kObjectType);

		rapidjson::Value authorObject(rapidjson::kObjectType);
		setStringKeyValueObject("name", blogname + " has a new post!", authorObject, document.GetAllocator());
		setStringKeyValueObject("url", postURL, authorObject, document.GetAllocator());
		setStringKeyValueObject("icon_url", blogAvatar, authorObject, document.GetAllocator());
		embedObject.AddMember("author", authorObject, document.GetAllocator());

		rapidjson::Value imageObject(rapidjson::kObjectType);
		setStringKeyValueObject("url", postImageURL, imageObject, document.GetAllocator());
		embedObject.AddMember("image", imageObject, document.GetAllocator());

		embedArray.PushBack(embedObject, document.GetAllocator());
		document.AddMember("embeds", embedArray, document.GetAllocator());
	}

	rapidjson::StringBuffer buffer;
	rapidjson::Writer <rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);


	return buffer.GetString();

}

void DiscordWebhook::setStringKeyValueObject(const std::string &key, const std::string &value, rapidjson::Value &object,
                                             rapidjson::MemoryPoolAllocator<> &allocator) { // TODO Comments
	rapidjson::Value keyString(rapidjson::kStringType);
	keyString.SetString(key.c_str(), allocator);
	rapidjson::Value valueString(rapidjson::kStringType);
	valueString.SetString(value.c_str(), allocator);
	object.AddMember(keyString, valueString, allocator);
}


