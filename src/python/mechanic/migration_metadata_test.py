# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
import unittest
from mechanic.migration_metadata import MigrationMetadata


class MigrationMetadataTest(unittest.TestCase):
  def testRepeatable(self):
    metadata = MigrationMetadata(props={"repeat": True})
    self.assertEqual(metadata.getProp("repeat", False), True)
