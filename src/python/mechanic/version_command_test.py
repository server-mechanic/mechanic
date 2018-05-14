#!/usr/bin/python
# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
import unittest
from mechanic.version_command import VersionCommand

class VersionCommandTest(unittest.TestCase):
    def setUp(self):
      mechanic = None
      self.command = VersionCommand(mechanic)

    def testRun(self):
      self.command.run([])
