#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from context import Mechanic

class MechanicTest(unittest.TestCase):
    def setUp(self):
      self.context = Mechanic()

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
