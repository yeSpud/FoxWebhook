//
// Created by Spud on 4/24/2021.
//

#include "DiscordWebhook.hpp"
#include <iostream>

cpr::Response DiscordWebhook::sendWebhook() {

	message = "Foo";

	std::string messageJson, embedJson, json;

	if (!message.empty()) {
		messageJson = R"({"Content":")" + message + "\"";
	}

	if (!embed.empty()) {
		embedJson = "\"embeds\":[" + embed + "]";
	}

	json = messageJson;
	if (!embedJson.empty()) {
		json += ("," + embedJson);
	}
	json += "}";

	std::cout << json << std::endl;

	//cpr::Body body =  cpr::Body{R"({"Content":"Test","embeds":[{"title":"Programmatic test"}]})"};
	cpr::Body body =  cpr::Body{json};

	cpr::Header header = cpr::Header{{"Content-Type", "application/json"}};

	//cpr::Response response = cpr::Post(cpr::Url{webhookURL}, body, header, cpr::Ssl(cpr::ssl::MaxTLSVersion{}));

	//cpr::Part embed = cpr::Part("embeds", R"([{"title":"Embed test"}])", "application/json");

	//cpr::Multipart Content = cpr::Multipart{{"Content", message}, embed};

	cpr::Response response = cpr::Post(cpr::Url{webhookURL},body, header, cpr::VerifySsl(false));

	message = "";

	embed = "";

	return response;

}

cpr::Response DiscordWebhook::sendMessage(const std::string& message) {

	this->message = message;

	return sendWebhook();
}

cpr::Response DiscordWebhook::sendEmbed(const std::string& blogname, const std::string& postURL, const std::string& blogAvatar, const std::string& postImageURL) {
	// "embeds":[{"author":{"name":"BLOGNAME has a new post!","url":"POSTURL","icon_url":"BLOGAVATAR"},"image":{"url":"POSTIMAGE"}}]
	embed = "{\"author\":{\"name\":\""+blogname+" has a new post!\",\"url\":\"" + postURL + "\",\"icon_url\":\"" + blogAvatar + "\"},\"image\":{\"url\":\""+postImageURL+"\"}}";

	return sendWebhook();
}


