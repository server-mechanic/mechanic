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
  if not os.path.exists(filename):
    try:
      os.makedirs(filename)
    except OSError as exc:
      if exc.errno != errno.EEXIST:
          raise

def makeparentdirs(filename):
  if not os.path.exists(os.path.dirname(filename)):
    try:
      os.makedirs(os.path.dirname(filename))
    except OSError as exc:
      if exc.errno != os.errno.EEXIST:
          raise

