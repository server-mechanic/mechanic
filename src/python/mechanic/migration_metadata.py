# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals


class MigrationMetadata(object):
  def __init__(self, props):
    self.props = props

  def getProp(self, name, defaultValue=None):
    try:
      return self.props[name]
    except ValueError:
      return defaultValue
