#!/usr/bin/python
# -*- coding: UTF-8 -*-

from config import Config
from exceptions import FollowUpCommandFailedException
import os

class FollowUpCommandExecutor:
  def __init__(self, config):
    self.config = config
    self.execve = os.execve

  def executeFollowUpCommand(self, followUpCommand):
    try:
      exitCode = self.execve(followUpCommand[0], followUpCommand, os.environ)
      if exitCode != 0:
        raise FollowUpCommandFailedException("Follow up command failed with exit code %s." % exitCode )
    except Exception as e:
      raise FollowUpCommandFailedException("Follow up command failed. %s" % e.message)
