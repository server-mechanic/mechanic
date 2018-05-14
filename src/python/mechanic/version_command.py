#!/usr/bin/python
# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
from mechanic.version_info import VersionInfo

class VersionCommand:
  name = 'version'
  description = 'Print mechanic version info.'

  def __init__(self, mechanic):
    self.mechanic = mechanic

  def run(self, args):
    print("Version: %s SCM-Version: %s" % (VersionInfo.packageVersion, VersionInfo.scmVersion))
