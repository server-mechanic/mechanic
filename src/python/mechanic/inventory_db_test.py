#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
import tempfile
from inventory_db import InventoryDb

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

    def testListMigrationsWhenEmpty(self):
      migrations = self.db.listMigrations("id")
      self.assertEqual([], migrations)

    def testListMigrationsAfterOneStarted(self):
      self.db.markMigrationAsStarted("test")
      migrations = self.db.listMigrations("id")
      self.assertEqual(1, len(migrations))
      self.assertEqual(1, migrations[0].id)
      self.assertEqual("test", migrations[0].name)
      self.assertEqual("STARTED", migrations[0].status)

    def testListMigrationsAfterOneFinished(self):
      self.db.markMigrationAsStarted("test")
      self.db.markMigrationAsSucceeded("test")
      migrations = self.db.listMigrations("start_date")
      self.assertEqual(1, len(migrations))
      self.assertEqual(1, migrations[0].id)
      self.assertEqual("test", migrations[0].name)
      self.assertEqual("SUCCESS", migrations[0].status)

    def tearDown(self):
      self.db.close()
      self.db = None

