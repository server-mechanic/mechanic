#!/usr/bin/python
# -*- coding: UTF-8 -*-

from mechanic.file_util import makeparentdirs
import datetime
import inspect
import traceback
import re

class Logger:
  def __init__(self):
    self.verbose = False

  def setVerbose(self, verbose):
    self.verbose = verbose

  def setLogFile(self, file):
    if file is not None:
      makeparentdirs(file)
    self.file = open(file, "wa")

  def debug(self, a, b = None):
    if self.verbose:
      self.__write("DEBUG", a, b)

  def info(self, a, b = None):
    self.__write("INFO ", a, b)

  def error(self, a, b = None):
    self.__write("ERROR", a, b)

  def __write(self, level, a = None, b = None):
    lines = []
    self.__formatInto(lines, a)
    self.__formatInto(lines, b)

    for line in lines:
      self.__write2("%s %s %s" % ( datetime.datetime.now(), level, line ))

  def __formatInto(self, lines, obj):
    if obj is None:
      return
    elif isinstance(obj, BaseException):
      lines.extend(traceback.format_exception(type(obj), obj, None))
    else:
      lines.append(str(obj))
    
  def __write2(self, output):
    if self.file is not None:
      print >> self.file, output
    print output
