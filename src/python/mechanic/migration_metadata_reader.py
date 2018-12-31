# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
import re
from mechanic.migration_metadata import MigrationMetadata

class MigrationMetadataReader(object):
  def __init__(self,logger=None):
    self.logger = logger

  def readMetadata(self, path, defaultMetadataProps=None):
    metadataProps = {}
    if defaultMetadataProps != None:
      metadataProps.update(defaultMetadataProps)

    with open(path, "r") as file:
      while True:
        line = file.readline()
        if not line:
          break

        line = line.rstrip()

        match = re.match('^\s*(#|//)\s*(mechanic-[A-Za-z0-9\-]+):\s*([^\s]+)\s*$', line)
        if match != None:
          key = match.group(2)
          value = self.__coerce(match.group(3))
          metadataProps[key] = value

    self.logger.debug("Metadata of {}: {}".format(path, metadataProps))
    return MigrationMetadata(props=metadataProps)

  def __coerce(self, value):
    if value.lower() == 'true':
      return True
    if value.lower() == 'false':
      return False

    match = re.match("^\s*(\d+)\s*$", value)
    if match != None:
      return int(match.group(1))

    match = re.match("^\s*\"([^\"]+)\"\s*", value)
    if match != None:
      return match.group(1)

    match = re.match("^\s*'([^']+)'\s*", value)
    if match != None:
      return match.group(1)

    return value
