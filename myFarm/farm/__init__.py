from .mqtt_client import client

default_app_config = 'farm.apps.FarmConfig'

def ready(self):
    client.loop_start()
