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

  def getStartTime(self):
    return self.startTime

  def getEndTime(self):
    return self.endTime

  def getId(self):
    return self.id

  def getName(self):
    return self.name

  def getFile(self):
    return self.file

  def getStatus(self):
    return self.status
