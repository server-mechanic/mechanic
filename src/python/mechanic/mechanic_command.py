# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
from mechanic.file_util import makeparentdirs
import logging
from mechanic.context import Mechanic
from mechanic.command_line import CommandLine
from mechanic.exceptions import MigrationFailedException
from mechanic.exceptions import MechanicException
from mechanic.exceptions import FollowUpCommandFailedException
import sys
import traceback

class MechanicCommand:
  def __init__(self, mechanic):
    self.mechanic = mechanic
    self.logger = mechanic.logger

  def run(self):
    args = self.mechanic.commandLine
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
      traceback.print_exc()
      self.logger.error(e)
      return 1
    finally:
      logging.shutdown()
    return 0

