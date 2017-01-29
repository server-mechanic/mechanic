#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from migrate_command import MigrateCommand

class MigrateCommandTest(unittest.TestCase):
    def setUp(self):
      self.command = MigrateCommand()

