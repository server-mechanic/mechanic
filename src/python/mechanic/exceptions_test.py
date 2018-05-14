# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
import unittest
from mechanic.exceptions import *

class ExceptionsTest(unittest.TestCase):
    def testMechanicException(self):
      self.assertEqual("test", MechanicException("test").message)

    def testMigrationFailedException(self):
      self.assertEqual("test", MigrationFailedException("test").message)

    def testFollowUpCommandFailedException(self):
      self.assertEqual("test", FollowUpCommandFailedException("test").message)
