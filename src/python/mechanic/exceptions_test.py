#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from exceptions import *

class ExceptionsTest(unittest.TestCase):
    def testMechanicException(self):
      self.assertEqual("test", MechanicException("test").message)

    def testMigrationFailedException(self):
      self.assertEqual("test", MigrationFailedException("test").message)

    def testFollowUpCommandFailedException(self):
      self.assertEqual("test", FollowUpCommandFailedException("test").message)
