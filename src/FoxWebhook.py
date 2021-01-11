from secrets import compare_digest


class FoxWebhook:
    """
    This class will hold the information gathered from the config file
    (as well as the discordHandler for when the blogHandler is setup).
    """

    webhookURL: str = None
    """
    String used to store the url of the discord webhook that will be used to post updates to a specified channel.
    """

    blogURL: str = None
    """
    String used to store the url of the corresponding blog that will be queried for new posts every so often.
    """

    token: str = None
    """
    String used to store the authentication token for accessing the Tumblr API. 
    Make sure this is not exposed to the public!
    """

    def __init__(self, webhook, blog, token):
        """
        Constructor for the FoxWebhook class. The class takes 3 arguments:

        * the webhook (str),
        * the blog url (str),
        * and the auth token (str).

        From there it checks to make sure that the provided strings are loosely valid.
        If they are not a FoxWebhookException is raised.
        :param webhook: the discord webhook url
        :param blog: the (tumblr) blog url
        :param token: tumblr authentication token for using their API
        """

        # Try assigning the arguments to the class variables.
        self.webhookURL = webhook
        self.blogURL = blog
        self.token = token

        # Make sure none of our variables are null (and are string type).
        if self.webhookURL is None or type(self.webhookURL) != str or self.webhookURL == "":
            raise FoxWebhookException("Webhook provided is incorrect type!")

        if self.blogURL is None or type(self.blogURL) != str or self.blogURL == "":
            raise FoxWebhookException("Webhook provided is incorrect type!")

        if self.token is None or type(self.token) != str or compare_digest(self.token, ""):
            raise FoxWebhookException("Token provided is incorrect type!")


class FoxWebhookException:
    """
    Custom error class for being raised when there is an issue with FoxWebhooks.
    """

    def __init__(self, message):
        super().__init__(message)
