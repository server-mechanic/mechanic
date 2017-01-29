#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from help_command import HelpCommand
from context import Mechanic

class HelpCommandTest(unittest.TestCase):
    def setUp(self):
      mechanic = Mechanic()
      self.command = HelpCommand(mechanic)

    def testRun(self):
      self.command.run([])
