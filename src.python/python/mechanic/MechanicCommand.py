#!/usr/bin/python
# -*- coding: UTF-8 -*-

from mechanic.Mechanic import Mechanic
from mechanic.MechanicCommandArgs import MechanicCommandArgs
from mechanic.MigrationFailedException import MigrationFailedException
from mechanic.FollowUpCommandFailedException import FollowUpCommandFailedException
import sys

class MechanicCommand:
  def __init__(self, args):
    self.args = MechanicCommandArgs(args)
    self.mechanic = Mechanic(args) 
    self.logger = self.mechanic.getLogger()
    self.logger.setVerbose(self.args.verbose)
    self.logger.setLogFile(self.mechanic.getConfig().getLogFile() )

  def run(self):
    commandClass = self.mechanic.commands.get(self.args.commandName)
    try:
      if commandClass is not None:
        commandClass(self.mechanic).run(self.args)
      else:
        self.mechanic.getDefaultCommand()(self.mechanic).run(self.args)
    except MigrationFailedException as e:
      self.logger.error(e)
      return 2
    except FollowUpCommandFailedException as e:
      self.logger.error(e)
      return 3
    except Exception as e:
      self.logger.error(e)
      return 1
    return 0

