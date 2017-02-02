#!/usr/bin/python
# -*- coding: UTF-8 -*-
 
import os
 
class Migration:
  def __init__(self, id, file, name=None, startTime=None, endTime=None, status='UNKNOWN'):
    self.id = id
    self.file = file
    self.name = name
    self.status = status
    self.startTime = startTime
    self.endTime = endTime

  def __str__(self):
    return "Migration(id=%s,name=%s)" % (self.id, self.name)
