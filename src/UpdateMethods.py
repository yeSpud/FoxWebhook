from typing import List
from src.FoxWebhook import FoxWebhook
from src.Handlers import BlogHandler, DiscordHandler


class UpdateMethods:

    @staticmethod
    def init(fox_webhooks: List[FoxWebhook]) -> List[dict]:
        """
        Initializes the script for checking blogs by determine the blog to query,
        and where to send new posts by parsing the FoxWebhooks,
        and then inserting the BlogHandler and DiscordHandler into a list.

        The BlogHandler is under the key 'blog', and the DiscordHandler is under the key 'discord'.

        :param fox_webhooks: A list of FoxWebhooks.
        :return: Returns the list of dictionaries containing the BlogHandler and DiscordHandler.
        """

        # Create an empty list that will contain a dictionary which will house both the BlogHandler and DiscordHandler.
        return_list: List[dict] = []
        for fox_webhook in fox_webhooks:

            # This is going to be where we do our initialization for each blog.
            # Create a new blog object by using the BlogHandler class.
            blog: BlogHandler = BlogHandler(blog_url=fox_webhook.blogURL, token=fox_webhook.token)

            # Create a new discord handler using the webhook url.
            discord_channel: DiscordHandler = DiscordHandler(discord_webhook_url=fox_webhook.webhookURL)

            # Append the newly created blog and discord handler to our empty list.
            return_list.append({"blog": blog, "discord": discord_channel})

        return return_list

    @staticmethod
    def loop(items: List[dict]):
        """
        Loop method of the script that queries each blog in the FoxWebhook,
        and sends new posts to their respective discord channels.
        :return: Nothing is returned.
        """

        # As this is the only place that uses the logger and sleep method, import it here.
        from src.Handlers import Logger
        from time import sleep

        while True:

            # Try iterating through all the items in the list to query the blogs for new posts,
            # and post them in their respective channels.
            try:
                for item in items:

                    # Get the blog handler to check for a new post from the blog.
                    blog: BlogHandler = item.get("blog")
                    if blog.has_new_post():
                        Logger.write_to_file("New post found! Posting to channel...")

                        # Get the discord handler and use it to post the new post from the blog.
                        discord_channel: DiscordHandler = item.get("discord")
                        discord_channel.post_new_update(blog.blog, blog.previousPost)

                # Once all items have been iterated through just sleep for 60 seconds.
                sleep(60)

            # If there is any exception during the loop be sure to log it and break out of the loop.
            except Exception as e:
                Logger.log_error(e)
                break

        # If we are able to make it out of the loop throw a new exception.
        raise StopIteration("Exited while loop due to exception!")
