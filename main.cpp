#include <iostream>
#include "src/FoxWebhook.hpp"

int main() {
	std::cout << "Hello, World!" << std::endl;

	std::vector<FoxWebhook> foxWebhooks = FoxWebhook::loadFromConfig();
	for (FoxWebhook foxWebhook : foxWebhooks) {
		std::cout << foxWebhook.getWebhookURL() << '\n' << foxWebhook.getBlogURL() << '\n' << foxWebhook.getAuth() << std::endl;
	}

	return 0;
}
