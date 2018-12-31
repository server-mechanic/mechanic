# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
import unittest
from mechanic.migration_executor import MigrationExecutor
from mechanic.migration import Migration
from mechanic.config import Config
from mechanic.inventory import Inventory
import mock
import tempfile
from mechanic.exceptions import MigrationFailedException

class MigrationExecutorTest(unittest.TestCase):
  def setUp(self):
    config = Config(mode="SYSTEM")
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

