#!/usr/bin/python
# -*- coding: UTF-8 -*-

from os import listdir
from os.path import basename, isfile, isdir, join
import os

def collectfiles(dirs):
  files = []
  for dir in dirs:
    if isdir(dir):
      for file in listdir(dir):
        file = join(dir, file)
        if isfile(file):
          files.append(file)
  files.sort(key=lambda f: basename(f))
  return files

def makedirs(filename):
  if os.path.isfile(filename):
    raise OSError("Cannot make dir. %s already is a file." % filename)
  if not os.path.exists(filename):
    try:
      os.makedirs(filename)
    except OSError as exc:
      if exc.errno != os.errno.EEXIST:
          raise

def makeparentdirs(filename):
  makedirs(os.path.dirname(filename))
