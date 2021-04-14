from typing import List
from src.Handlers import ConfigHandler, DiscordHandler, BlogHandler
from src.FoxWebhook import FoxWebhook
from src.TumblrAPI import Posts
from src.UpdateMethods import UpdateMethods
from time import sleep

webhooks: List[FoxWebhook] = []

# Try loading from the configuration file. If anything goes wrong, print the error, and exit with a status code of -2.
try:
    config: ConfigHandler = ConfigHandler()

    # Set the webhooks to be what was loaded from the config file.
    webhooks = config.get_web_hooks()
except Exception as e:
    print(e)
    exit(-1)

foo: List[dict] = UpdateMethods.init(webhooks)

for entry in foo:
    blog: BlogHandler = entry.get("blog")
    post: Posts = blog.get_most_recent_post()
    discord: DiscordHandler = entry.get("discord")
    discord.post_new_update(blog.blog, post)
    sleep(5)
    discord.post_new_update(blog.blog, post)
