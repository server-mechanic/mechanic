#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from mechanic.MechanicCommandArgs import MechanicCommandArgs

class MechanicCommandArgsTest(unittest.TestCase):
    def setUp(self):
      self.args = MechanicCommandArgs(["-v", "sub", "--sub-flag=sub_val"])

    def givenArgsOf(self, args):
      self.args = MechanicCommandArgs(args)

    def testCommandNameRecognized(self):
      self.assertEqual(self.args.commandName, "sub")

    def testVerboseTrueRecognized(self):
      self.assertTrue(self.args.verbose)

    def testVerboseFalseRecognized(self):
      self.givenArgsOf(["sub"])
      self.assertFalse(self.args.verbose)

    def tearDown(self):
      self.args = None

