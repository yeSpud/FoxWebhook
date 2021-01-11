from typing import List
from src.FoxWebhook import FoxWebhook
from src.Handlers import BlogHandler, DiscordHandler


class UpdateMethods:

    @staticmethod
    def init(fox_webhooks: List[FoxWebhook]) -> List[dict]:
        """
        TODO Documentation
        :param fox_webhooks:
        :return:
        """

        return_list: List[dict] = []
        for fox_webhook in fox_webhooks:
            # This is going to be where we do our initialization for each blog.
            # Create a new blog object by using the BlogHandler class.
            blog: BlogHandler = BlogHandler(blog_url=fox_webhook.blogURL, token=fox_webhook.token)

            discord_channel: DiscordHandler = DiscordHandler(discord_webhook_url=fox_webhook.webhookURL)

            # Append the newly created blog to the list of blogs.
            return_list.append({"blog": blog, "discord": discord_channel})

        return return_list

    @staticmethod
    def loop(items: List[dict]):
        """
        TODO Documentation
        :return:
        """

        from src.Handlers import Logger
        from time import sleep

        while True:
            try:
                for item in items:
                    blog: BlogHandler = item.get("blog")
                    if blog.has_new_post():
                        Logger.write_to_file("New post found! Posting to channel...")

                        discord_channel: DiscordHandler = item.get("discord")
                        discord_channel.post_new_update(blog.blog, blog.previousPost)

                sleep(60)
            except Exception as e:
                Logger.log_error(str(e))
                break

        raise StopIteration("Exited while loop due to exception!")
