#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from migration import Migration

class MigrationTest(unittest.TestCase):
    def setUp(self):
      self.migration = Migration(41, "file", "name", "start", "end", "status")

    def testPropertiesAssigned(self):
      self.assertEqual(41, self.migration.id)
      self.assertEqual("file", self.migration.file)
      self.assertEqual("name", self.migration.name)
      self.assertEqual("start", self.migration.startTime)
      self.assertEqual("end", self.migration.endTime)
      self.assertEqual("status", self.migration.status)


