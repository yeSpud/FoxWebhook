class BlogHandler:
    """
    This class contains all the methods for interfacing with the desired Tumblr blog
    and determining of they've posted anything new.
    """

    # Since this is the only place we are using the Tumblr API, only import the library here.
    from pytumblr import TumblrRestClient
    from src.TumblrAPI import Posts, Blog
    from typing import List

    previousPost: Posts = None
    """
    The previous post by the blog (used for comparison).
    """

    blog: Blog = None
    """
    The Blog object from the TumblrAPI module.
    See the TumblrAPI module for documentation.
    """

    API: TumblrRestClient = None
    """
    Class used to interact with the Tumblr API.
    """

    blog_url: str = None
    """
    Url of the Tumblr blog.
    Be sure to hold onto it here as it is not kept by the API.
    """

    def __init__(self, blog_url: str, token: str):
        """
        Constructor for the blog handler. All that is needed for creation is the blog url, and the tumblr API token.
        :param blog_url: TODO
        :param token: TODO
        """

        # Create an api object for querying the tumblr api.
        self.API = self.TumblrRestClient(consumer_key=token)

        # Because the blog url is not maintained by the API we store it as one of the object's variables.
        self.blog_url = blog_url

        # Get the blog info.
        self.blog = BlogHandler.Blog(self.API.blog_info(self.blog_url))
        Logger.write_to_file("Successfully retrieved blog info from " + self.blog.title)

        # Get the most recent post from the blog and store it in memory.
        self.previousPost = self.get_most_recent_post()
        Logger.write_to_file("Set previous post to " + str(self.previousPost.id))

    def has_new_post(self) -> bool:
        """
        Checks for a new blog post by comparing the current one to the previous post stored in memory.
        If there is a new post this function will return True.
        If there isn't a new post then the function will return False.
        :return: TODO
        """

        try:
            # Get the most recent post.
            post: BlogHandler.Posts = self.get_most_recent_post()

        # TODO Documentation
        except Exception as e:
            Logger.log_error(str(e))
            return False

        # Check if the new post doesnt match the previous post.
        Logger.write_to_file(f"Comparing {post.id} to {self.previousPost.id}...")
        if post.id != self.previousPost.id:

            # Set the previous post to the current post, and return true.
            self.previousPost = post
            return True
        else:

            # No new post, so return false.
            return False

    def get_most_recent_post(self) -> Posts:
        """
        Gets the most recent post from the blog by querying the blog url with a limit of 1.
        :return: TODO
        """

        # TODO Comment
        try:
            # Just call the get_number_of_posts function but pass 1 as the argument.
            # Then just return the first item in the list.
            return self.get_number_of_posts(1)[0]

        except Exception as e:
            Logger.log_error(str(e))
            raise IndexError("List appears to be empty, so there may have been an issue getting the posts.")

    def get_number_of_posts(self, number: int) -> List[Posts]:  # TODO Test me
        """
        Get the most recent (specified number of) posts.
        :param number: TODO
        :return: TODO
        """

        # Create a list of posts to be returned.
        posts: BlogHandler.List[BlogHandler.Posts] = []

        # Create a list of posts from the blog (with a limit).
        all_posts: list = self.API.posts(self.blog_url, limit=number).get("posts")

        # Create a post object using each of the posts from the blog, and then add them to the returned posts list.
        for entry in all_posts:

            # TODO Comment
            try:
                post: BlogHandler.Posts = BlogHandler.Posts(entry)
                posts.append(post)

            # TODO Comment
            except Exception as e:
                Logger.log_error(str(e))

        # Return the retrieved posts.
        return posts


class DiscordHandler:
    """
    This class contains all the methods that will be used to post the new blog post to a discord channel via webhook.
    """

    # Since this is where we are sending the discord embeds from, only import the discord libs here.
    from discord_webhook import DiscordWebhook, DiscordEmbed
    from src.TumblrAPI import Blog, Posts

    webhook: DiscordWebhook = None
    """
    This webhook object is going to be used to store our webhook created in the constructor,
    and will be used elsewhere in the class.
    """

    def __init__(self, discord_webhook_url: str):
        """
        Constructor for our DiscordHandler.
        All that is required is the URL of the discord webhook.
        :param discord_webhook_url: The url for the discord webhook (usually leads to a discord channel).
        """

        # Create a new webhook object using our URL.
        self.webhook = self.DiscordWebhook(url=discord_webhook_url)

    def post_new_update(self, blog: Blog, post: Posts):
        """
        Posts an embed into the webhook's discord channel.
        Whats required is the title of the blog, the avatar of the blog, and finally the post itself.
        :param blog: TODO
        :param post: TODO
        :return: Nothing is returned.
        """

        # Create a new embed to store all the information that we want to provide the channel.
        embed = self.DiscordEmbed()

        # Abuse the author portion to use it as the title for the embed, as well as using it for the icon avatar.
        embed.set_author(name=f"{blog.title} has a new post!", url=post.post_url, icon_url=blog.avatar[0].get("url"))

        # Set the large image area to be that of the post from the blog.
        embed.set_image(url=post.photos[0].get("original_size").get("url"))

        # Set the timestamp in the footer - just because.
        embed.set_timestamp()

        # The library that we are using does something weird.
        # Rather than just attaching the embed, sending off the request, and then disposing of it,
        # it keeps the embed around for future use.
        # Because of this, we will add the embed to the request, execute the request,
        # and then remove the embed (should be at index 0).
        self.webhook.add_embed(embed)
        Logger.write_to_file("Sending embed to channel...")
        response_code = self.webhook.execute()[0]
        self.webhook.remove_embed(0)

        # If the status code from the response is not good (200 - 204), log the code (and response) to the file.
        if response_code.status_code not in [200, 204]:
            Logger.write_to_file("Response code from embed was " + response_code)

    def post_message(self, message: str):
        """
        Posts a simple text message to the webhook text channel.
        :param message: The message to send via the webhook to the discord channel.
        :return: Nothing is returned.
        """

        # Set the content of the webhook to the message.
        self.webhook.set_content(message)

        # Send the message to the channel.
        Logger.write_to_file("Sending message to channel...")
        response_code = self.webhook.execute()[0]

        # Be sure to reset the message content.
        self.webhook.set_content("")

        # If the status code from the response is not good (200 - 204), log the code (and response) to the file.
        if response_code.status_code not in [200, 204]:
            Logger.write_to_file("Response code from sent message was " + response_code)


class ConfigHandler:
    """
    Handler that manages the reading and parsing of the configuration file.
    """

    from src.FoxWebhook import FoxWebhook, FoxWebhookException
    from typing import List

    @staticmethod
    def read_configs() -> dict:
        """
        Reads the content of a set configuration file.
        If an error occurs during the reading phase an exception will be thrown.
        :return: Returns a dict of json that has been read from the config file.
        """

        # Check that the config file exits.
        config_file_path: str = "config.json"

        # Raise an exception if the file does not exit.
        import os.path
        if not os.path.isfile(config_file_path):
            raise FileNotFoundError(f"The config file at {os.path.abspath(config_file_path)} is missing or unreadable!")

        # Try opening the config file.
        try:
            Logger.write_to_file("Reading from config...")
            raw_config = open(config_file_path, 'r')
        except OSError:
            raise Exception("Could not open config file!")

        # Read the json from the config file.
        import json
        config_json: dict = json.load(raw_config)

        # Close the rawConfig file.
        raw_config.close()

        # Return our JSON.
        return config_json

    def get_web_hooks(self) -> List[FoxWebhook]:
        """
        Gets a list of FoxWebhooks from the config file.
        If there is any issue trying to read from the config file, an exception will be thrown.
        :return: Returns a list of FoxWebhooks that have been read from the config file.
        """

        # Create an array to store our webhooks from the config file
        fox_web_hooks: ConfigHandler.List[ConfigHandler.FoxWebhook] = []

        # Get the configs
        config = self.read_configs()

        # Iterate though our entries from the config file
        for entry in config["Webhooks"]:

            Logger.write_to_file("Processing webhook from file...")

            # If the webhook URL at this point is empty move onto the other entries
            web_hook_url = entry.get("WebhookURL")
            if web_hook_url is None or web_hook_url == "":
                continue

            # If the blog URL at this point is empty move onto other entries
            blog_url = entry.get("BlogURL")
            if blog_url is None or blog_url == "":
                continue

            # If the blog auth at this point is empty move onto other entries
            blog_auth = entry.get("Auth")
            if blog_auth is None or blog_auth == "":
                continue

            # Try appending the FoxWebHook to our return array
            try:
                fwh = self.FoxWebhook(web_hook_url, blog_url, blog_auth)
                fox_web_hooks.append(fwh)
            except self.FoxWebhookException as f:
                # Print what went wrong :(
                Logger.log_error("Unable to add FoxWebhook")
                Logger.log_error(str(f))

        # Return our FoxWebHooks
        return fox_web_hooks


class Logger:
    """Handler that manages all logging for the scripts. Mostly static methods."""

    file: str = None
    """
    File name of the log file. 
    This is set as None until the file has been created.
    """

    previous_message: str = None
    """
    Stores the previous message written to the log file.
    This is just the message - it does not include the message's timestamp.
    """

    @staticmethod
    def create_new_log_file():
        """
        Creates a new log file following the format of 'time-date'-log.txt that will be written to.
        :return: Nothing is returned.
        """

        # If we are not in verbose mode return now (essentially canceling the creation)
        if Logger.is_not_verbose_mode():
            return

        from datetime import datetime

        # %H-%M-%S-%b-%d-%Y = Hours - Minutes - Seconds - Day - Month - Year
        now = datetime.now().strftime("%H-%M-%S-%b-%d-%Y")
        Logger.file = now + "-log.txt"

        # Log success to the newly created file
        Logger.write_to_file("Created log file")

    @staticmethod
    def write_to_file(message: str):
        """
        Write the given message to the log file.
        A timestamp will automatically be included with the message,
        so the timestamp can be omitted in the provided message.
        :param message: The message to be written to the log file.
        :return: Nothing is returned.
        """

        # If we are not in verbose mode exit early
        if Logger.is_not_verbose_mode():
            return

        # Check if the message is equal to a previous message. Return early if it is.
        if message == Logger.previous_message:
            return

        # Open the log file stream in append mode in order to write to the file.
        f = open(Logger.file, 'a')

        from datetime import datetime
        # Get the current date and time of the message.
        # Formatted as: Hour:Minute:Second - Day-Month-Year.
        now = datetime.now().strftime("%H:%M:%S - %b-%d-%Y")

        # Write the timestamp and the message to the log file, and print the message.
        f.write(f"{now}:\t{message}\n")
        print(message)

        # Flush and close the log file.
        f.flush()
        f.close()

        # Finally update the previous message.
        Logger.previous_message = message

    @staticmethod
    def is_not_verbose_mode() -> bool:
        """
        Returns whether the script has not been executed in verbose mode (run without -v as the argument).
        :return: True or False depending on if the script is being run in verbose mode.
        """

        try:
            from sys import argv

            # Try returning if the argument provided is not in verbose mode (-v).
            return argv[1] != "-v"

        # If an index error has occurred, default to true.
        except IndexError:
            return True

    @staticmethod
    def log_error(error: str):
        """
        If an error occurs this method will log the error to the log file.
        This will also include the file and line number at which the error occurred.
        :param error: The error to be logged to the file (must be passed as a string).
        :return: Nothing returned.
        """

        # If we are not in verbose mode return now.
        if Logger.is_not_verbose_mode():
            return

        from sys import exc_info
        from os import path

        # Determine the file that the error occurred in, and at what line.
        # This information will be appended to the error message.
        _, _, exc_tb = exc_info()
        file_name = path.split(exc_tb.tb_frame.f_code.co_filename)[1]
        output: str = f"ERROR: {error} ({file_name}:{exc_tb.tb_lineno})"

        # Determine the size of the border that will surrounded the message.
        border: str = '=' * len(output)

        # Write the output to the log file surround by the border.
        Logger.write_to_file(f"\n\n{border}\n{output}\n{border}\n\n")
