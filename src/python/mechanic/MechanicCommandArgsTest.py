#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from mechanic.MechanicCommandArgs import MechanicCommandArgs

class MechanicCommandArgsTest(unittest.TestCase):
    def setUp(self):
      self.args = MechanicCommandArgs(["-v", "sub", "--sub-flag=sub_val", "--", "followUp", "followUpFlag"])

    def __givenArgsOf(self, args):
      self.args = MechanicCommandArgs(args)

    def testCommandNameRecognized(self):
      self.assertEqual(self.args.commandName, "sub")

    def testVerboseTrueRecognized(self):
      self.assertTrue(self.args.verbose)

    def testVerboseFalseRecognized(self):
      self.__givenArgsOf(["sub"])
      self.assertFalse(self.args.verbose)

    def testFollowUpRecognized(self):
      self.assertEquals(self.args.followUpCommand, ["followUp", "followUpFlag"])

    def testFollowUpEmptyWhenNonePassedIn(self):
      self.__givenArgsOf(["sub"])
      self.assertEquals(self.args.followUpCommand, None)

    def tearDown(self):
      self.args = None

