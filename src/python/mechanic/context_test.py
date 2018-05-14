#!/usr/bin/python
# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
import unittest
from mechanic.context import Mechanic

class MechanicTest(unittest.TestCase):
    def setUp(self):
      self.context = Mechanic([])

    def testLoggerInitialized(self):
      self.assertIsNotNone(self.context.logger)

    def testConfigInitialized(self):
      self.assertIsNotNone(self.context.config)

    def testCommandsInitialized(self):
      self.assertIsNotNone(self.context.commands)

    def testDefaultCommandInitialized(self):
      self.assertIsNotNone(self.context.defaultCommand)

    def testInventoryInitialized(self):
      self.assertIsNotNone(self.context.inventory)
