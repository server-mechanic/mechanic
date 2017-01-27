#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from mechanic.Config import Config

class ConfigTest(unittest.TestCase):
    def setUp(self):
      self.config = Config()
      self.config.mechanicRootDir = "_ROOT_"

    def testGetMigrationTmpDirIsExpanded(self):
      self.assertEqual(self.config.getMigrationTmpDir(), "_ROOT_/var/lib/mechanic/tmp/")

    def testGetInventoryDbFileIsExpanded(self):
      self.assertEqual(self.config.getInventoryDbFile(), "_ROOT_/var/lib/mechanic/state/inventory.db")

    def testGetConfigFileIsExpanded(self):
      self.assertEqual(self.config.getConfigFile(), "_ROOT_/etc/mechanic.conf")

    def testGetMigrationDirsIsExpanded(self):
      migrationDirs = self.config.getMigrationDirs()
      self.assertEqual(migrationDirs[0], "_ROOT_/etc/mechanic/migration.d")
      self.assertEqual(migrationDirs[1], "_ROOT_/var/lib/mechanic/migration.d")

    def tearDown(self):
      self.config = None

if __name__ == '__main__':
    unittest.main()
