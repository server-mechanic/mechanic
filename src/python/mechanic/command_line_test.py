#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from command_line import CommandLine
from exceptions import MechanicException

class CommandLineTest(unittest.TestCase):
    def setUp(self):
      self.args = CommandLine(["-v", "--global-flag=global_val", "sub", "--sub-flag=sub_val", "--", "followUp", "followUpFlag"])

    def __givenArgsOf(self, args):
      self.args = CommandLine(args)

    def testCommandNameRecognized(self):
      self.assertEqual(self.args.commandName, "sub")

    def testVerboseTrueRecognized(self):
      self.assertTrue(self.args.verbose)

    def testDuplicateCommandDetected(self):
      with self.assertRaises(Exception) as context:
        CommandLine(["help", "version"])
      self.assertTrue('Only single command allowed.' in str(context.exception))

    def testVerboseFalseRecognized(self):
      self.__givenArgsOf(["sub"])
      self.assertFalse(self.args.verbose)

    def testSubFlagRecognized(self):
      self.assertTrue(self.args.hasSubOpt('sub-flag'))
      self.assertEquals(self.args.subOpts['sub-flag'], 'sub_val')

    def testGlobalFlagRecognized(self):
      self.assertTrue(self.args.hasGlobalOpt('global-flag'))
      self.assertEquals(self.args.globalOpts['global-flag'], 'global_val')

    def testFollowUpRecognized(self):
      self.assertEquals(self.args.followUpCommand, ["followUp", "followUpFlag"])

    def testFollowUpAbsentWhenNonePassedIn(self):
      self.__givenArgsOf(["sub"])
      self.assertEquals(self.args.followUpCommand, None)

    def testFollowUpAbsentWhenEmptyPassedIn(self):
      self.__givenArgsOf(["sub", "--"])
      self.assertEquals(self.args.followUpCommand, None)

    def tearDown(self):
      self.args = None

