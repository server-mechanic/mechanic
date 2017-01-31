#!/usr/bin/python
# -*- coding: UTF-8 -*-

from context import Mechanic
from command_line import CommandLine
from exceptions import MigrationFailedException
from exceptions import MechanicException
from exceptions import FollowUpCommandFailedException
import sys

class MechanicCommand:
  def __init__(self, mechanic):
    self.mechanic = mechanic
    self.logger = mechanic.logger

  def run(self, args):
    args = CommandLine(args)
    self.logger.setVerbose(args.verbose)
    self.logger.setLogFile(self.mechanic.config.getLogFile() )

    command = self.mechanic.commands.get(args.commandName)
    try:
      if command is not None:
        command.run(args)
      else:
        self.mechanic.defaultCommand.run(args)
    except MigrationFailedException as e:
      self.logger.error(e.message)
      return 2
    except FollowUpCommandFailedException as e:
      self.logger.error(e.message)
      return 3
    except MechanicException as e:
      self.logger.error(e.message)
      return 1
    except Exception as e:
      self.logger.error(e)
      return 1
    return 0

