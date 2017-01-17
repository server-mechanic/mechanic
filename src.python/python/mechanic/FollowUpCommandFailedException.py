#!/usr/bin/python
# -*- coding: UTF-8 -*-

class FollowUpCommandFailedException(Exception):
  def __init__(self, message):
    self.message = message

  def __str__(self):
    return self.message 
