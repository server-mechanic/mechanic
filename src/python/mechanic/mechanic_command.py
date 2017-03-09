#!/usr/bin/python
# -*- coding: UTF-8 -*-

from mechanic.file_util import makeparentdirs
import logging
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
    if args.verbose:
      self.logger.setLevel(logging.DEBUG)
    else:
      self.logger.setLevel(logging.INFO)
    if not self.mechanic.config.getLogFile() in [ "", "/dev/stderr", "stderr" ]:
      makeparentdirs(self.mechanic.config.getLogFile()) 
      self.logger.addHandler(logging.FileHandler(self.mechanic.config.getLogFile()))

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
    finally:
      logging.shutdown()
    return 0

