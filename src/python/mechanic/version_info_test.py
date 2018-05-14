#!/usr/bin/python
# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
import unittest
from mechanic.version_info import VersionInfo

class VersionInfoTest(unittest.TestCase):
    def setUp(self):
      self.versionInfo = VersionInfo()

    def testPackageVersionDefined(self):
      self.assertIsNotNone(VersionInfo.packageVersion)

    def testScmVersionDefined(self):
      self.assertIsNotNone(VersionInfo.scmVersion)
