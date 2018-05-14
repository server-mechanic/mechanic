#!/usr/bin/python
# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
import unittest
from mechanic.config import Config

class ConfigTest(unittest.TestCase):
    def setUp(self):
      self.config = Config(mode="SYSTEM")
      self.config.mechanicRootDir = "_ROOT_"

    def testGetMigrationTmpDirIsExpanded(self):
      self.assertEqual(self.config.getMigrationTmpDir(), "_ROOT_/var/lib/mechanic/tmp")

    def testGetInventoryDbFileIsExpanded(self):
      self.assertEqual(self.config.getInventoryDbFile(), "_ROOT_/var/lib/mechanic/state/inventory.db")

    def testGetLogFileIsExpanded(self):
      self.assertEqual(self.config.getLogFile(), "")

    def testGetConfigFileIsExpanded(self):
      self.assertEqual(self.config.getConfigFile(), "_ROOT_/etc/mechanic.conf")

    def testGetMigrationDirsIsExpanded(self):
      migrationDirs = self.config.getMigrationDirs()
      self.assertEqual(migrationDirs[0], "_ROOT_/etc/mechanic/migration.d")
      self.assertEqual(migrationDirs[1], "_ROOT_/var/lib/mechanic/migration.d")

    def testGetPreMigrationDirsIsExpanded(self):
      migrationDirs = self.config.getPreMigrationDirs()
      self.assertEqual(migrationDirs[0], "_ROOT_/etc/mechanic/pre-migration.d")
      self.assertEqual(migrationDirs[1], "_ROOT_/var/lib/mechanic/pre-migration.d")

    def testGetPostMigrationDirsIsExpanded(self):
      migrationDirs = self.config.getPostMigrationDirs()
      self.assertEqual(migrationDirs[0], "_ROOT_/etc/mechanic/post-migration.d")
      self.assertEqual(migrationDirs[1], "_ROOT_/var/lib/mechanic/post-migration.d")

