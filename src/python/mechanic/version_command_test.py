#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from version_command import VersionCommand

class VersionCommandTest(unittest.TestCase):
    def setUp(self):
      mechanic = None
      self.command = VersionCommand(mechanic)

    def testRun(self):
      self.command.run([])
