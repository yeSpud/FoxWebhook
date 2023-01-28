# FoxWebhook

FoxWebhooks is a project that pulls data from a service (so far just [Tumblr](https://www.tumblr.com/) is supported) and can then upload data to a [discord](https://discord.com/) webhook.

There are 3 example use case files provided ([main.cpp](main.cpp), [get_numer_of_posts_util.cpp](get_number_of_posts_util.cpp), 
and [cron.cpp](cron.cpp)) all three do the same high level goal - 
check a blog on Tumblr for new posts (or get a number of posts in the case of [get_numer_of_posts_util.cpp](get_number_of_posts_util.cpp)), and post them to a discord channel.

## Why is it called FoxWebhook?

This project started as just pulling new posts from a blog called [Foxes in Love](https://foxes-in-love.tumblr.com) and posting them to a discord channel via a webhook. 

Hence: Fox(es in love) + (discord) Webhook = FoxWebhook. 

I soon realized that this could be broadened to include more than just the one blog, and possibly more than pulling just from Tumblr, 
so the scope was broadened to pulling data from "a service" (although tumblr is the only service implemented for now).

As such the name FoxWebhook has stuck, 
and is used for the class name that stores the config details of where to pull data from and where to send it to.

## How to use

You are welcome to write your own script, or use those provided. 
If you do make you're own script feel free to open a pull request and add them to the main project.

[main.cpp](main.cpp) TODO `./make\ foxwebhook.sh`

[cron.cpp](cron.cpp) TODO `./make\ cron.sh`

[get_number_of_posts_util](get_number_of_posts_util.cpp) TODO `./make\ post\ util.sh`


### Config setup / example

A [config.json](config.json) file is expected to be in the same directory, or one directory above the current working directory.

Single:

```json
{
  "Keys": {
	"Tumblr": "tumblr-api-key"
  },
  "Webhooks": [
	{
	  "Retrieve-From": "foxes-in-love.tumblr.com",
	  "Send-To": "discord-webhook-link",
	  "Service-Key": "Tumblr"
	}
  ]
}
```

Secondary:

```json
{
  "Keys": {
	"Tumblr": "tumblr-api-key"
  },
  "Webhooks": [
	{
	  "Retrieve-From": "foxes-in-love.tumblr.com",
	  "Send-To": "discord-webhook-link",
	  "Service-Key": "Tumblr"
	},
	{
	  "Retrieve-From": "david.tumblr.com",
	  "Send-To": "discord-webhook-link2",
	  "Service-Key": "Tumblr"
	}
  ]
}
```