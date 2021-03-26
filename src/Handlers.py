class BlogHandler:
    """
    This class contains all the methods for interfacing with the desired Tumblr blog
    and determining of they've posted anything new.
    """

    # Since this is the only place we are using the Tumblr API, only import the library here.
    from src.TumblrAPI import Posts, Blog, NPFTumblrRestClient
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

    API: NPFTumblrRestClient = None
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
        Constructor for the blog handler.
        All that is needed for creation is the blog url, and the tumblr API token.
        :param blog_url: The url of the tumblr blog that will be queried for new posts.
        :param token: The tumblr API token used to authenticate with the tumblr API.
        """

        # Create an api object for querying the tumblr api.
        self.API = self.NPFTumblrRestClient(consumer_key=token)

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
        :return: Returns True if there is a new post from the blog, or False if there is no new post.
        """

        try:
            # Get the most recent post.
            post: BlogHandler.Posts = self.get_most_recent_post()

        # If there is any exception thrown log the error and return False (no new post).
        except Exception as e:
            Logger.log_error(e)
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
        :return: Returns the most recent Post from the blog.
        """

        # Try just calling the get_number_of_posts function but pass 1 as the argument.
        # Then just return the first item in the list.
        try:
            return self.get_number_of_posts(1)[0]

        # If there is any exception raised its likely caused by the fact that there was no post returned,
        # and so the list has a size of 0. Because of this, getting the first entry in the list will cause an exception.
        # If this happens simply log the error, and then change the type of exception to an index error.
        except Exception as e:
            Logger.log_error(e)
            raise IndexError("List appears to be empty, so there may have been an issue getting the posts.")

    def get_number_of_posts(self, number: int) -> List[Posts]:
        """
        Get the most recent (specified number of) posts.
        :param number: The number of posts to get from the blog. Must be at least 1 or higher.
        :return: A list of Post objects retrieved from the blog.
        """

        # Create a list of posts to be returned.
        posts: BlogHandler.List[BlogHandler.Posts] = []

        # Create a list of posts from the blog (with a limit).
        all_posts: list = self.API.posts(blogname=self.blog_url, type="photo", limit=number, npf=True).get("posts")

        # Create a post object using each of the posts from the blog, and then add them to the returned posts list.
        for entry in all_posts:

            # Try to create and add a new Post object to the posts list.
            try:
                post: BlogHandler.Posts = BlogHandler.Posts(entry)
                posts.append(post)

            # If there is any exception just log it.
            except Exception as e:
                Logger.log_error(e)

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
        If there is an issue getting the post url then a message will be posted instead.
        :param blog: The Blog object which corresponds to the tumblr blog that has posted a new... post.
        :param post: The new Post object from the blog.
        :return: Nothing is returned.
        """

        # Create a new embed to store all the information that we want to provide the channel.
        embed = self.DiscordEmbed()

        # Abuse the author portion to use it as the title for the embed, as well as using it for the icon avatar.
        embed.set_author(name=f"{blog.title} has a new post!", url=post.post_url, icon_url=blog.avatar[0].get("url"))

        # Make sure the post has a photo attached. If there isn't send a message instead.
        if len(post.content) < 1:
            Logger.write_to_file("Cannot get image from post!")
            self.post_message(f"{blog.title} has a new post, but I can't seem to access the image :(")
            return

        # Get the first content block that is of image type.
        content_block: dict = {}
        for block in post.content:
            if block.get("type") == "image":
                content_block = block
                break

        # Make sure the content block is the right size.
        if len(content_block) == 0:
            Logger.write_to_file("Unable to retrieve photo from content block!")
            self.post_message(f"{blog.title} has a new post, but I can't seem to access the image :(")
            return

        # Get all the photo urls from the content block.
        photos: list = content_block.get("media")

        # Get the first photo (usually the biggest).
        photo: dict = photos[0]

        # Set the large image area to be that of the post from the blog.
        embed.set_image(url=photo.get("url"), width=photo.get("width"), height=photo.get("height"))

        # Set the timestamp in the footer - just because.
        embed.set_timestamp()

        # Add the embed to the webhook
        self.webhook.add_embed(embed)
        Logger.write_to_file("Sending embed to channel...")

        # Send it to the discord channel
        self.execute()

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
        self.execute()

    def execute(self):
        """
        Sends the embed or message to the discord channel.
        :return: Nothing is returned.
        """

        # Setup the requests variable here before the gauntlet of try-excepts.
        from requests import Response
        response: Response

        try:
            # Try sending the message to the discord channel.
            response = self.webhook.execute()[0]
        except Exception as error:
            # If there was any exception thrown log it and return early.
            Logger.log_error(error)
            return

        try:
            # The library that is used does something weird.
            # Rather than just attaching the embed, sending off the request, and then disposing of it,
            # it keeps the embed around for future use.
            # Because of this, we will add the embed to the request, execute the request,
            # and then remove the embed (should be at index 0) and remove the content.
            self.webhook.set_content("")
            if len(self.webhook.embeds > 0):
                self.webhook.remove_embed(0)

        except Exception as error:
            Logger.log_error(error)

        try:
            # If the status code from the response is not good (200 - 204), log the code (and response) to the file.
            if response.status_code not in [200, 204]:
                Logger.write_to_file("Response code from embed was " + str(response.status_code))
        except Exception as responseException:
            Logger.log_error(responseException)


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
                Logger.write_to_file("Unable to add FoxWebhook")
                Logger.log_error(f)

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
    def log_error(error: Exception):
        """
        If an error occurs this method will log the error to the log file.
        This will also include the file and line number at which the error occurred.
        :param error: The error to be logged to the file.
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
        output: str = f"ERROR: {type(error).__name__} - {str(error)} ({file_name}:{exc_tb.tb_lineno})"

        # Determine the size of the border that will surrounded the message.
        border: str = '=' * len(output)

        # Write the output to the log file surround by the border.
        Logger.write_to_file(f"\n\n{border}\n{output}\n{border}\n\n")
