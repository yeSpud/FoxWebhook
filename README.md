# FoxWebhook

TODO

## Example `config.json`

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