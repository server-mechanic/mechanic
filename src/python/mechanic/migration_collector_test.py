#!/usr/bin/python
# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
import unittest
from mechanic.migration_collector import MigrationCollector
import os
from os.path import basename
import tempfile
from mechanic import file_util
from mock import MagicMock
from mechanic.migration import Migration

class CollectMigrationsTest(unittest.TestCase):
  def setUp(self):
    logger = MagicMock()
    self.collector = MigrationCollector(logger)
    self.tempDir = tempfile.mkdtemp()

  def testCollectingFromEmptyListGivesNoMigrations(self):
    migrations = self.collector.collectMigrationsFrom([])
    self.assertEqual(migrations, [])

  def testNonExistentDirsIgnored(self):
    migrations = self.collector.collectMigrationsFrom(["/doesNotExist", "/thisToo"])
    self.assertEqual(migrations, [])

  def testCollectingFromEmptyDirGivesNoMigrations(self):
    migrations = self.collector.collectMigrationsFrom([self.tempDir])
    self.assertEqual(migrations, [])

  def testCollectingFromDirGivesFileBasedMigrations(self):
    tempFile = tempfile.mkstemp(dir=self.tempDir)[1]
    migrations = self.collector.collectMigrationsFrom([self.tempDir])
    self.assertEqual([m.file for m in migrations], [tempFile])

  def testCollectingFromDirGivesFileBasedMigrations(self):
    mig1File = os.path.join(self.tempDir, "mig1")
    file_util.touch(mig1File)
    mig2File = os.path.join(self.tempDir, "mig2", "run")
    file_util.makeparentdirs(mig2File)
    file_util.touch(mig2File)
    migrations = self.collector.collectMigrationsFrom([self.tempDir])
    self.assertEqual([m.file for m in migrations], [mig1File, mig2File])
