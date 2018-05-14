# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
from mechanic.mechanic_command import MechanicCommand
from mechanic.command_line import CommandLine
import unittest
from mock import MagicMock

class MechanicCommandTest(unittest.TestCase):
    def setUp(self):
      self.mechanic = MagicMock()
      self.mechanic.config = MagicMock()
      self.mechanic.config.logFile = ""
      self.mechanic.config.getLogFile = MagicMock()
      self.mechanic.config.getLogFile.return_value = ""
      self.mechanic.logger = MagicMock()
      self.mechanic.logger.addHandler = MagicMock()
      self.mechanic.commands = {}
      self.mechanic.defaultCommand = MagicMock()
      self.command = MechanicCommand(self.mechanic)

    def testRunWithNoArgsExecutedDefaultCommand(self):
      self.command.run()
      self.mechanic.defaultCommand.run.assert_called()

    def testRunWithCommandName(self):
      fooCommand = MagicMock()
      self.mechanic.commandLine = MagicMock()
      self.mechanic.commandLine.commandName = "foo"
      self.mechanic.commands["foo"] = fooCommand
      self.command.run()
      fooCommand.run.assert_called()
