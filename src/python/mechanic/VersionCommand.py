#!/usr/bin/python
# -*- coding: UTF-8 -*-

from mechanic.MechanicVersionInfo import MechanicVersionInfo

class VersionCommand:
  name = 'version'
  description = 'Print mechanic version info.'

  def __init__(self, mechanic):
    self.mechanic = mechanic

  def run(self, args):
    print "Version: %s SCM-Version: %s" % (MechanicVersionInfo.packageVersion, MechanicVersionInfo.scmVersion)
