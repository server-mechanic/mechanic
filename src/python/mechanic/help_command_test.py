#!/usr/bin/python
# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
import unittest
from mechanic.help_command import HelpCommand
from mechanic.context import Mechanic
from mock import MagicMock

class HelpCommandTest(unittest.TestCase):
    def setUp(self):
      mechanic = MagicMock()
      self.command = HelpCommand(mechanic)

    def testRun(self):
      self.command.run([])
