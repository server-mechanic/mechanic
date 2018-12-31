# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals

class MigrationMetadata(object):
  PROP_REPEATABLE = "mechanic-migration-repeatable"

  def __init__(self, props):
    self.props = props

  def isRepeatable(self):
    return self.props.get(MigrationMetadata.PROP_REPEATABLE) == True
