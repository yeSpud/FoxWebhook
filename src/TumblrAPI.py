from typing import List
from pytumblr import TumblrRestClient, helpers


class NPFTumblrRestClient(TumblrRestClient):
    """
    Used to fix the tumblr rest client from not supporting npf in its release.
    Maybe once they release the next version it'll be fixed :)
    """

    @helpers.validate_blogname
    def posts(self, blogname, type=None, **kwargs):
        """
        Gets a list of posts from a particular blog

        :param blogname: a string, the blogname you want to look up posts
        :param id: an int, the id of the post you are looking for on the blog
        :param tag: a string, the tag you are looking for on posts
        :param limit: an int, the number of results you want
        :param offset: an int, the offset of the posts you want to start at.
        :param before: an int, the timestamp for posts you want before.
        :param filter: the post format you want returned: HTML, text or raw.
        :param type: the type of posts you want returned, e.g. video. If omitted returns all post types.
        :param npf: Whether or not to use the Neue Post Format specification

        :returns: a dict created from the JSON response
        """
        if type is None:
            url = '/v2/blog/{}/posts'.format(blogname)
        else:
            url = '/v2/blog/{}/posts/{}'.format(blogname, type)
        return self.send_api_request("get", url, kwargs,
                                     ['id', 'tag', 'limit', 'offset', 'before', 'reblog_info', 'notes_info', 'filter',
                                      'api_key', 'npf'], True)


class Posts:
    """
    Object corresponding to a Tumblr post.
    """

    blog_name: str = None
    """
    The short name used to uniquely identify a blog.
    Default is None.
    """

    id: int = 0
    """
    The post's unique ID.
    Default is 0.
    """

    id_string: str = None
    """
    The post's unique ID as a String.
    For clients that don't support 64-bit integers.
    Default is None.
    """

    post_url: str = None
    """
    The location of the post.
    Default is None.
    """

    Type: str = None
    """
    The type of post.
    Default is None.
    """

    timestamp: int = 0
    """
    The time of the post, in seconds since the epoch.
    Default is 0.
    """

    date: str = None
    """
    The GMT date and time of the post, as a string.
    Default is None.
    """

    format: str = None
    """
    The post format: html or markdown.
    Default is None.
    """

    reblog_key: str = None
    """
    The key used to reblog this post.
    Default is None.
    """

    tags: list = []
    """
    Tags applied to the post.
    Default is an empty list.
    """

    bookmarklet: bool = False
    """
    Indicates whether the post was created via the Tumblr bookmarklet.
    Exists only if true.
    Default is False.
    """

    mobile: bool = False
    """
    Indicates whether the post was created via mobile/email publishing.
    Exists only if true.
    Default is False.
    """

    source_url: str = None
    """
    The URL for the source of the content (for quotes, reblogs, etc.).
    Exists only if there's a content source.
    Default is None.
    """

    source_title: str = None
    """
    The title of the source site.
    Exists only if there's a content source.
    Default is None.
    """

    liked: bool = False
    """
    Indicates if a user has already liked a post or not.
    Exists only if the request is fully authenticated with OAuth.
    Default is False.
    """

    state: str = None
    """
    Indicates the current state of the post.
    States are published, queued, draft and private.
    Default is None.
    """

    total_posts: int = 0
    """
    The total number of post available for this request, useful for paginating through results.
    Default is 0.
    """

    content: List[dict] = []

    def __init__(self, post_blob: dict):
        """
        Constructor for the Post object.
        :param post_blob: The json dict containing all the information of the post.
        """

        self.blog_name = post_blob.get("blog_name")
        self.id = post_blob.get("id")
        self.id_string = post_blob.get("id_string")
        self.post_url = post_blob.get("post_url")
        self.type = post_blob.get("type")
        self.timestamp = post_blob.get("timestamp")
        self.date = post_blob.get("date")
        self.format = post_blob.get("format")
        self.reblog_key = post_blob.get("reblog_key")
        self.tags = post_blob.get("tags")
        self.bookmarklet = post_blob.get("bookmarklet", False)
        self.mobile = post_blob.get("mobile", False)
        self.source_url = post_blob.get("source_url", None)
        self.source_title = post_blob.get("source_title", None)
        self.liked = post_blob.get("liked", False)
        self.state = post_blob.get("state")
        self.total_posts = post_blob.get("total_posts")
        self.content = post_blob.get("content")


class Blog:
    """
    Object containing all the information corresponding to a Tumblr blog.
    """

    title: str = None
    """
    The display title of the blog.
    Default is None.
    """

    posts: int = 0
    """
    The total number of posts to this blog.
    Default is 0.
    """

    name: str = None
    """
    The short blog name that appears before tumblr.com in a standard blog hostname.
    Default is None.
    """

    updated: int = 0
    """
    The time of the most recent post, in seconds since the epoch.
    Default is 0.
    """

    description: str = None
    """
    You guessed it!
    The blog's description.
    Default is None.
    """

    ask: bool = False
    """
    Indicates whether the blog allows questions.
    Default is False.
    """

    ask_anon: bool = False
    """
    Indicates whether the blog allows anonymous questions;
    returned only if ask is true.
    Default is False.
    """

    likes: int = 0
    """
    Number of likes for this user, returned only if this is the user's primary blog and sharing of likes is enabled.
    Default is 0.
    """

    is_blocked_from_primary: bool = False
    """
    Indicates whether this blog has been blocked by the calling user's primary blog;
    returned only if there is an authenticated user making this call.
    Default is False.
    """

    avatar: list = []
    """
    An array of avatar objects, each a different size, which should each have a width, height, and URL.
    Default is an empty array.
    """

    timezone: str = None
    """
    The blog's configured timezone, such as "US/Eastern".
    Only viewable by blog member.
    Partial response field ONLY.
    Default is None.
    """

    timezone_offset: str = None
    """
    The blog's configured timezone as a GMT offset such as "GMT+0800".
    Only viewable by blog member. Partial response field ONLY.
    Default is None.
    """

    def __init__(self, returned_blob: dict):
        """
        Constructor for the blog object.
        :param returned_blob: The json dict containing all the information of the blog.
        """

        blog_blob: dict = returned_blob.get("blog")
        """
        Dictionary of the json returned by the blog.
        Used as a shortcut within the constructor to complete the variables for the blog object.
        """

        self.title = blog_blob.get("title")
        self.posts = blog_blob.get("posts")
        self.name = blog_blob.get("name")
        self.updated = blog_blob.get("updated")
        self.description = blog_blob.get("description")
        self.ask = blog_blob.get("ask")
        self.ask_anon = blog_blob.get("ask_anon")
        self.likes = blog_blob.get("likes")
        self.is_blocked_from_primary = blog_blob.get("is_blocked_from_primary")
        self.avatar = blog_blob.get("avatar")
        self.timezone = blog_blob.get("timezone")
        self.timezone_offset = blog_blob.get("timezone_offset")
