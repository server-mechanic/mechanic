#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from version_info import VersionInfo

class VersionInfoTest(unittest.TestCase):
    def setUp(self):
      self.versionInfo = VersionInfo()

    def testPackageVersionDefined(self):
      self.assertIsNotNone(VersionInfo.packageVersion)

    def testScmVersionDefined(self):
      self.assertIsNotNone(VersionInfo.scmVersion)
