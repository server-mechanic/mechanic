#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from mechanic.MechanicCommandArgs import MechanicCommandArgs

class MechanicCommandArgsTest(unittest.TestCase):
    def setUp(self):
      self.args = MechanicCommandArgs(["-v", "sub", "--sub-flag=sub_val"])

    def testCommandNameRecognized(self):
      self.assertEqual(self.args.commandName, "sub")

    def tearDown(self):
      self.args = None

