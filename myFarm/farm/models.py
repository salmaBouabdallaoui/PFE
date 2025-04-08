from django.db import models

class YourModel(models.Model):
    data_field = models.CharField(max_length=255)
    timestamp = models.DateTimeField(auto_now_add=True)

