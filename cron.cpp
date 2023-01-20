//
// Created by Spud on 1/18/23.
//

#include <iostream>
#include "src/DiscordWebhook.hpp"
#include "TumblrAPI.hpp"
#include "post.hpp"
#include "npf/content/image.hpp"

int main(int argc, char** argv) {

	if (argc != 4) {
		std::cerr << "Usage: ./cron <webhook url> <blog url> <api key>" << std::endl;
		return -1;
	}

	DiscordWebhook webhook = DiscordWebhook(argv[1]);
	TumblrAPI tumblrApi = TumblrAPI(argv[3]);

	cpr::Response postResponse = tumblrApi.getPostsJson(argv[2], 1);

	if (postResponse.status_code != 200) {
		std::cerr << "Unable to get most recent post: " << postResponse.status_code << std::endl;
		return 1;
	}

	std::shared_ptr<Post> post = Post::generatePosts(postResponse.text.c_str())[0];

	if (post == nullptr) {

		// Treat as no new post and exit early.
		return 0;
	}

	std::ifstream infile("most_recent.url");
	if (infile) {

		// File exists - read from infile.
		std::string mostRecentUrl;
		infile >> mostRecentUrl;
		infile.close();

		if (mostRecentUrl == post->post_url) {

			// Post has not changed, so just return.
			return 0;
		}
	}

	// Overwrite file content (or create a new one if it didnt exist).
	std::ofstream outfile("most_recent.url", std::ios::trunc);
	if (!outfile) {
		std::cerr << "Unable to open file stream" << std::endl;
		return -2;
	}

	outfile << post->post_url << std::endl;
	outfile.close();

	// Get the relevant blog information for the post.
	cpr::Response blogResponse = tumblrApi.getBlogInfoJson(argv[2]);
	if (blogResponse.status_code != 200) {
		std::cerr << "Unable to retrieve blog information: " << blogResponse.text << std::endl;
		return 2;
	}
	Blog blog = Blog::generateBlog(blogResponse.text.c_str());
	std::shared_ptr<Image> postImage = std::dynamic_pointer_cast<Image>(post->content[0]);
	cpr::Response response = webhook.sendEmbed(blog.title, post->post_url, blog.avatars[0].url, postImage->media[0].url);

	if (response.status_code != 200) {
		std::cerr << "Could not send embed: " << response.text << std::endl;
		return -3;
	}

	return 0;
}