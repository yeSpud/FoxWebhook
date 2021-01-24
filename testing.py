from typing import List
from src.Handlers import ConfigHandler, DiscordHandler, BlogHandler
from src.FoxWebhook import FoxWebhook
from src.UpdateMethods import UpdateMethods

webhooks: List[FoxWebhook] = []

# Try loading from the configuration file. If anything goes wrong, print the error, and exit with a status code of -2.
try:
    config: ConfigHandler = ConfigHandler()

    # Set the webhooks to be what was loaded from the config file.
    webhooks = config.get_web_hooks()
except Exception as e:
    print(e)
    exit(-1)

foo = UpdateMethods.init(webhooks)

for entry in foo:
    blog: BlogHandler = entry.get("blog")
    posts: List[BlogHandler.Posts] = blog.get_number_of_posts(1)
    posts.reverse()
    discord: DiscordHandler = entry.get("discord")
    discord.post_message("I got rate limited D:")
    for post in posts:
        discord.post_new_update(blog.blog, post)
