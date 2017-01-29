#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
import tempfile
from mechanic.InventoryDb import InventoryDb

class InventoryDbTest(unittest.TestCase):
    def setUp(self):
      tmpFile = tempfile.mkstemp()
      self.db = InventoryDb(tmpFile[1])

    def testMarkStartedAndFailed(self):
      self.db.markMigrationAsStarted("test")
      self.assertFalse(self.db.hasMigrationSucceeded("test"))
      self.db.markMigrationAsFailed("test")
      self.assertFalse(self.db.hasMigrationSucceeded("test"))

    def testMarkStartedAndSucceeded(self):
      self.db.markMigrationAsStarted("test")
      self.assertFalse(self.db.hasMigrationSucceeded("test"))
      self.db.markMigrationAsSucceeded("test")
      self.assertTrue(self.db.hasMigrationSucceeded("test"))

    def tearDown(self):
      self.db.close()
      self.db = None

