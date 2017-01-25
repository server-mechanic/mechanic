#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from mechanic.InventoryDb import InventoryDb

class InventoryDbTest(unittest.TestCase):
    def setUp(self):
      self.db = InventoryDb()
      self.db.open("example.db")

    def testMarkStarted(self):
      self.db.markMigrationAsStarted("test")

    def tearDown(self):
      self.db.close()
      self.db = None

if __name__ == '__main__':
    unittest.main()

