from typing import List
from src.Handlers import ConfigHandler
from src.FoxWebhook import FoxWebhook
from src.UpdateMethods import UpdateMethods

webhooks: List[FoxWebhook] = []

# Try loading from the configuration file. If anything goes wrong, print the error, and exit with a status code of -2.
try:
    config: ConfigHandler = ConfigHandler()

    # Set the webhooks to be what was loaded from the config file.
    webhooks = config.get_web_hooks()
    print(webhooks)
except Exception as e:
    print(e)
    exit(-1)

foo = UpdateMethods.init(webhooks)

for entry in foo:
    blog = entry.get("blog")
    post = blog.get_most_recent_post()
    d = entry.get("discord")
    d.post_new_update(blog.blog, post)
