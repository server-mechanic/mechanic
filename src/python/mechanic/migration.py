# -*- coding: UTF-8 -*-
 
from __future__ import absolute_import
from __future__ import unicode_literals
import os
 
class Migration:
  def __init__(self, id, file, name=None, startTime=None, endTime=None, status='UNKNOWN', metadata=None):
    self.id = id
    self.file = file
    self.name = name
    self.status = status
    self.startTime = startTime
    self.endTime = endTime
    self.metadata = metadata or {}

  def __str__(self):
    return "Migration(id=%s,name=%s)" % (self.id, self.name)
