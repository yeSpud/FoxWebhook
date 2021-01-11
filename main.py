from typing import List
from src.FoxWebhook import FoxWebhook
from src.Handlers import ConfigHandler, Logger
from src.UpdateMethods import UpdateMethods

if not Logger.is_not_verbose_mode():
    Logger.create_new_log_file()

# Create a list to store our webhooks which will be loaded from the configuration file.
fox_webhooks: List[FoxWebhook] = []

# Try loading from the configuration file. If anything goes wrong, print the error, and exit with a status code of -2.
try:
    config: ConfigHandler = ConfigHandler()

    # Set the webhooks to be what was loaded from the config file.
    fox_webhooks = config.get_web_hooks()
except Exception as e:
    Logger.log_error(str(e))
    exit(-1)

# Create a list to store all the blogs that we are going to be querying from time to time.
foo: List[dict] = []
try:
    foo = UpdateMethods.init(fox_webhooks=fox_webhooks)
except Exception as e:
    Logger.log_error(str(e))
    exit(-2)


# Since we are done with initialization, start looping until killed from a process kill.
try:
    UpdateMethods.loop(items=foo)
except KeyboardInterrupt:
    Logger.write_to_file("Requested to halt.")
    exit(0)
except Exception as e:
    Logger.log_error(str(e))

Logger.write_to_file("Exiting...")
