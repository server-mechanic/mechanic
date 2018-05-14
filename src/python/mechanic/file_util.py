# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
import os

def makedirs(filename):
  if os.path.isfile(filename):
    raise OSError("Cannot make dir. %s already is a file." % filename)
  if not os.path.exists(filename):
    try:
      os.makedirs(filename)
    except OSError as exc:
      if exc.errno != os.errno.EEXIST:
          raise

def touch(file):
  open(file, "w").close()

def makeparentdirs(filename):
  makedirs(os.path.dirname(filename))
