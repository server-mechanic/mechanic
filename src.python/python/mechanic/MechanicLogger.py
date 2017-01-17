#!/usr/bin/python
# -*- coding: UTF-8 -*-

from mechanic.file_util import makeparentdirs
import datetime
import inspect

class MechanicLogger:
  def __init__(self):
    self.verbose = False

  def setVerbose(self, verbose):
    self.verbose = verbose

  def setLogFile(self, file):
    if file is not None:
      makeparentdirs(file)
    self.file = open(file, "wa")

  def debug(self, message):
    if self.verbose:
      self.__write("DEBUG", message)

  def info(self, message):
    self.__write("INFO ", message)

  def error(self, message):
    self.__write("ERROR", message)

  def __write(self, level, message):
    output = "%s %s %s" % ( datetime.datetime.now(), level, message )
    if self.file is not None:
      print >> self.file, output
    print output
