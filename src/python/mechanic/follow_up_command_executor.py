#!/usr/bin/python
# -*- coding: UTF-8 -*-

from config import Config
import os

class FollowUpCommandExecutor:
  def __init__(self, config):
    self.config = config

  def executeFollowUpCommand(self, followUpCommand):
    try:
      environment = { "MECHANIC_ROOT_DIR": self.config.mechanicRootDir }
      exitCode = os.execve(followUpCommand[0], followUpCommand, environment)
      if exitCode != 0:
        raise FollowUpCommandFailedException("Follow up command failed with exit code %s." % exitCode )
    except Exception as e:
      raise FollowUpCommandFailedException("Follow up command failed. %s" % e.message)
