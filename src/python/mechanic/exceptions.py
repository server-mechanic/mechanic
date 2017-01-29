#!/usr/bin/python
# -*- coding: UTF-8 -*-

class MechanicException(Exception):
  def __init__(self, message):
    self.message = message

  def __str__(self):
    return self.message 

class MigrationFailedException(MechanicException):
  def __init__(self, message):
    super(MigrationFailedException, self).__init__(message)

class FollowUpCommandFailedException(MechanicException):
  def __init__(self, message):
    super(FollowUpCommandFailedException, self).__init__(message)
