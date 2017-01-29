#!/usr/bin/python
# -*- coding: UTF-8 -*-

class MechanicException(Exception):
  def __init__(self, message):
    self.message = message

  def __str__(self):
    return self.message 

class MigrationFailedException(Exception):
  def __init__(self, message):
    self.message = message

  def __str__(self):
    return self.message 

class FollowUpCommandFailedException(Exception):
  def __init__(self, message):
    self.message = message

  def __str__(self):
    return self.message 
