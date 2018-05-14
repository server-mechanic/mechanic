# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
import unittest
from mechanic.list_migrations_command import ListMigrationsCommand
from mock import MagicMock
import tempfile
from mechanic.command_line import CommandLine
from mechanic.exceptions import MechanicException

class ListMigrationsCommandTest(unittest.TestCase):
    def setUp(self):
      mechanic = MagicMock()
      self.command = ListMigrationsCommand(mechanic)

    def testRunWithNoMigrations(self):
      args = CommandLine([])
      self.command.run(args)

    def testRunWithInvalidOrderBy(self):
      args = CommandLine(["list-migrations", "--order-by=invalid"])
      with self.assertRaises(MechanicException) as context:
        self.command.run(args)
      self.assertTrue('invalid' in str(context.exception))

