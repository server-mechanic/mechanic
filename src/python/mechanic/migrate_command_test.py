#!/usr/bin/python
# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
from mechanic.migrate_command import MigrateCommand
from mechanic.command_line import CommandLine
import unittest
from mock import MagicMock

class ListMigrationsCommandTest(unittest.TestCase):
    def setUp(self):
      mechanic = MagicMock()
      self.command = MigrateCommand(mechanic)
      self.command.followUpCommandExecutor = MagicMock()

    def testRunWithNoMigrationsAndNoFollowUpCommand(self):
      args = CommandLine([])
      self.command.run(args)
      self.command.followUpCommandExecutor.executeFollowUpCommand.assert_not_called()

    def testRunWithNoMigrationsAndFollowUpCommand(self):
      args = CommandLine(["migrate", "--", "followUpCommand", "followUpCommandArg0"])
      self.command.run(args)
      self.command.followUpCommandExecutor.executeFollowUpCommand.assert_called_with(["followUpCommand", "followUpCommandArg0"])
