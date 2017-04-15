#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from command_line import CommandLine
from exceptions import MechanicException

class CommandLineTest(unittest.TestCase):
    def setUp(self):
      self.args = CommandLine(["-v", "-o", "foo=bar", "--global-flag=global_val", "sub", "--sub-flag=sub_val", "--", "followUp", "followUpFlag"])

    def __givenArgsOf(self, args):
      self.args = CommandLine(args)

    def testOverrideRecognized(self):
      self.assertEqual(self.args.overrideOpts["foo"], "bar")

    def testCommandNameRecognized(self):
      self.assertEqual(self.args.commandName, "sub")

    def testVerboseTrueRecognized(self):
      self.assertTrue(self.args.verbose)

    def testDuplicateCommandDetected(self):
      with self.assertRaises(Exception) as context:
        CommandLine(["help", "version"])
      self.assertTrue('Only single command allowed.' in str(context.exception))

    def testMissingArgumentToDashM(self):
      with self.assertRaises(Exception) as context:
        CommandLine(["-m"])
      self.assertTrue('-m requires' in str(context.exception))

    def testInvalidArgumentToDashM(self):
      with self.assertRaises(Exception) as context:
        CommandLine(["-m", "invalid"])
      self.assertTrue('-m requires' in str(context.exception))

    def testUserMode(self):
      self.args = CommandLine(["-m", "user"])
      self.assertEquals("USER", self.args.mode)

    def testSystemMode(self):
      self.args = CommandLine(["-m", "system"])
      self.assertEquals("SYSTEM", self.args.mode)

    def testMissingConfigFileArg(self):
      with self.assertRaises(Exception) as context:
        CommandLine(["-c"])
      self.assertTrue('-c requires' in str(context.exception))

    def testConfigFile(self):
      self.args = CommandLine(["-c", "anotherConfigFile"])
      self.assertEquals("anotherConfigFile", self.args.configFile)

    def testMissingArgumentToDashO(self):
      with self.assertRaises(Exception) as context:
        CommandLine(["-o"])
      self.assertTrue('-o requires key=value' in str(context.exception))

    def testInvalidArgumentToDashO(self):
      with self.assertRaises(Exception) as context:
        CommandLine(["-o", "key"])
      self.assertTrue('-o requires key=value' in str(context.exception))

    def testOverrfideLogFile(self):
      self.args = CommandLine(["-o", "main.log_file=anotherLogFile"])
      self.assertEquals("anotherLogFile", self.args.overrideOpts["main.log_file"])

    def testVerboseFalseRecognized(self):
      self.__givenArgsOf(["sub"])
      self.assertFalse(self.args.verbose)

    def testSubFlagRecognized(self):
      self.assertTrue(self.args.hasSubOpt('sub-flag'))
      self.assertEquals(self.args.subOpts['sub-flag'], 'sub_val')
      self.assertEquals(self.args.getSubOpt('sub-flag'), 'sub_val')
      self.assertEquals(self.args.getSubOpt('absent-sub-flag'), None)
      self.assertEquals(self.args.getSubOpt('absent-sub-flag', 'absent_val'), 'absent_val')

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
