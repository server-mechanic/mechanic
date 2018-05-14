# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
import unittest
from mechanic.inventory import Inventory
from mock import MagicMock
import tempfile
import os

class InventoryTest(unittest.TestCase):
    def setUp(self):
      logger = MagicMock()
      config = MagicMock()
      self.inventory = Inventory(logger=logger, config=config)
      self.inventoryDb = MagicMock()
      self.inventory.inventoryDb = self.inventoryDb
      tempDir = tempfile.mkdtemp()
      config.migrationDirs = [tempDir]
      config.preMigrationDirs = [tempDir]
      config.postMigrationDirs = [tempDir]

    def testListUnappliedMigrationsFromEmptyDir(self):
      migrations = self.inventory.listUnappliedMigrations()
      self.assertEqual(migrations, [])

    def testListPreMigrationsFromEmptyDir(self):
      migrations = self.inventory.listPreMigrations()
      self.assertEqual(migrations, [])

    def testListPostMigrationsFromEmptyDir(self):
      migrations = self.inventory.listPostMigrations()
      self.assertEqual(migrations, [])

    def testMarkMigrationAsStarted(self):
      self.inventory.markMigrationAsStarted("test")
      self.inventory.inventoryDb.markMigrationAsStarted.assert_called_with("test")

    def testMarkMigrationAsFailed(self):
      self.inventory.markMigrationAsFailed("test")
      self.inventory.inventoryDb.markMigrationAsFailed.assert_called_with("test")

    def testMarkMigrationAsSucceeded(self):
      self.inventory.markMigrationAsSucceeded("test")
      self.inventory.inventoryDb.markMigrationAsSucceeded.assert_called_with("test")

    def testListMigrations(self):
      migrations = self.inventory.listMigrations("my-order")
      self.inventory.inventoryDb.listMigrations.assert_called_with("my-order")

    def testClose(self):
      self.inventory.close()
      self.inventoryDb.close.assert_called_once()
      self.assertIsNone(self.inventory.inventoryDb)
