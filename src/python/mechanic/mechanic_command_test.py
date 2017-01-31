#!/usr/bin/python
# -*- coding: UTF-8 -*-

from mechanic_command import MechanicCommand
from command_line import CommandLine
import unittest
from mock import MagicMock

class MechanicCommandTest(unittest.TestCase):
    def setUp(self):
      self.mechanic = MagicMock()
      self.mechanic.logger = MagicMock()
      self.mechanic.commands = {}
      self.mechanic.defaultCommand = MagicMock()
      self.command = MechanicCommand(self.mechanic)

    def testRunWithNoArgsExecutedDefaultCommand(self):
      self.command.run([])
      self.mechanic.defaultCommand.run.assert_called()

    def testRunWithCommandName(self):
      fooCommand = MagicMock()
      self.mechanic.commands["foo"] = fooCommand
      self.command.run(["foo"])
      fooCommand.run.assert_called()
