//
// Created by Spud on 4/24/2021.
//

#include <iostream>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "DiscordWebhook.hpp"

cpr::Response DiscordWebhook::sendWebhook(const std::string &json) {

	//std::cout << json << std::endl;

	cpr::Body body = cpr::Body{json};
	cpr::Header header = cpr::Header{{"Content-Type", "application/json"}};
	cpr::Response response = cpr::Post(cpr::Url{webhookURL}, body, header, cpr::VerifySsl(false));

	return response;

}

cpr::Response DiscordWebhook::sendMessage(const std::string &message) {

	std::string json = formatJson(message, "", "", "", "");

	return sendWebhook(json);
}

cpr::Response
DiscordWebhook::sendEmbed(const std::string &blogname, const std::string &postURL, const std::string &blogAvatar,
                          const std::string &postImageURL) {

	std::string json = formatJson("", blogname, postURL, blogAvatar, postImageURL);

	return sendWebhook(json);
}

std::string DiscordWebhook::formatJson(const std::string &message, const std::string &blogname,
                                       const std::string &postURL, const std::string &blogAvatar,
                                       const std::string &postImageURL) {

	rapidjson::Document document;
	document.SetObject();

	if (!message.empty()) {
		rapidjson::Value messageText(rapidjson::kStringType);
		messageText.SetString(message.c_str(), document.GetAllocator());
		document.AddMember("content", messageText, document.GetAllocator());
	}

	if (!blogname.empty() && !postURL.empty() && !blogAvatar.empty() && !postImageURL.empty()) {
		rapidjson::Value embedArray(rapidjson::kArrayType);
		rapidjson::Value embedObject(rapidjson::kObjectType);

		rapidjson::Value authorObject(rapidjson::kObjectType);
		rapidjson::Value name(rapidjson::kStringType);
		name.SetString(blogname.c_str(), document.GetAllocator());
		authorObject.AddMember("name", name, document.GetAllocator());
		rapidjson::Value blogposturl(rapidjson::kStringType);
		blogposturl.SetString(postURL.c_str(), document.GetAllocator());
		authorObject.AddMember("url", blogposturl, document.GetAllocator());
		rapidjson::Value blogiconurl(rapidjson::kStringType);
		blogiconurl.SetString(blogAvatar.c_str(), document.GetAllocator());
		authorObject.AddMember("icon_url", blogiconurl, document.GetAllocator());
		embedObject.AddMember("author", authorObject, document.GetAllocator());

		rapidjson::Value imageObject(rapidjson::kObjectType);
		rapidjson::Value imageurl(rapidjson::kStringType);
		imageurl.SetString(postImageURL.c_str(), document.GetAllocator());
		imageObject.AddMember("url", imageurl, document.GetAllocator());
		embedObject.AddMember("image", imageObject, document.GetAllocator());

		embedArray.PushBack(embedObject, document.GetAllocator());
		document.AddMember("embeds", embedArray, document.GetAllocator());
	}

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);


	return buffer.GetString();

}


