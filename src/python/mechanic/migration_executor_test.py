#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from migration_executor import MigrationExecutor
from migration import Migration
from config import Config
from inventory import Inventory
import mock
import tempfile
from exceptions import MigrationFailedException

class MigrationExecutorTest(unittest.TestCase):
  def setUp(self):
    config = Config()
    self.inventory = mock.MagicMock()
    logger = mock.MagicMock()
    self.executor = MigrationExecutor(config, self.inventory, logger)
    config.mechanicRootDir = tempfile.mkdtemp()

  def testApplyMigrationWithSuccess(self):
    self.executor.applyMigration(Migration(1, name="true",file="/bin/true"))
    self.inventory.markMigrationAsStarted.assert_called_with("true")
    self.inventory.markMigrationAsSucceeded.assert_called_with("true")

  def testApplyMigrationWithFailure(self):
    with self.assertRaises(MigrationFailedException) as context:
      self.executor.applyMigration(Migration(1, name="false",file="/bin/false"))
    self.inventory.markMigrationAsStarted.assert_called_with("false")
    self.inventory.markMigrationAsFailed.assert_called_with("false")
    self.assertTrue('failed' in str(context.exception))

  def testApplyMigrationWithMigrationNotExecutable(self):
    with self.assertRaises(Exception) as context:
      self.executor.applyMigration(Migration(1, name="doesNotExist",file="/bin/doesNotExist"))
    self.assertTrue('not executable' in str(context.exception))

