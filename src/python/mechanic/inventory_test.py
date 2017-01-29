#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from inventory import Inventory

class InventoryTest(unittest.TestCase):
    def setUp(self):
      self.inventory = Inventory()

