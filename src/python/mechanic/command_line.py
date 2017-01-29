#!/usr/bin/python
# -*- coding: UTF-8 -*-

from re import compile, match, findall
from exceptions import MechanicException

class CommandLine:
  KEY_VALUE_PATTERN = compile("--([\\w\\-_]+)=(.*)")

  def __init__(self, args):
    self.args = args
    self.verbose = False
    self.followUpCommand = None
    self.commandName = None
    self.globalOpts = {}
    self.subOpts = {}
    self.__process(args)

  def hasGlobalOpt(self, key):
    return self.globalOpts.has_key(key)

  def hasSubOpt(self, key):
    return self.subOpts.has_key(key)

  def getSubOpt(self, key, defaultValue = None):
    if self.subOpts.has_key(key):
      return self.subOpts.get(key)
    else:
      return defaultValue

  def __process(self, args):
    doubleDashSeen = False
    commandSeen = False
    for arg in args:
      matchResult = findall(CommandLine.KEY_VALUE_PATTERN, arg)
      if "--" == arg:
        doubleDashSeen = True
        self.followUpCommand = []
      elif doubleDashSeen:
        self.followUpCommand.append(arg)
      elif "-v" == arg or "--verbose" == arg:
        self.verbose = True
      elif not commandSeen and len(matchResult) > 0:
        self.globalOpts[matchResult[0][0]] = matchResult[0][1]
      elif commandSeen and len(matchResult) > 0:
        self.subOpts[matchResult[0][0]] = matchResult[0][1]
      elif not arg.startswith("-"):
        if commandSeen:
          raise MechanicException("Only single command allowed. (Already %s.)" % self.commandName)
        self.commandName = arg
        commandSeen = True
    if self.followUpCommand is not None and len(self.followUpCommand) == 0:
      self.followUpCommand = None
