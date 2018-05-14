#!/usr/bin/python
# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
from re import compile, match, findall
from mechanic.exceptions import MechanicException

class CommandLine:
  KEY_VALUE_OPT_PATTERN = compile("--([\\w\\-_]+)=(.*)")
  KEY_VALUE_PATTERN = compile("([\\.\\w\\-_]+)=(.*)")

  def __init__(self, args):
    self.args = args
    self.verbose = False
    self.followUpCommand = None
    self.commandName = None
    self.globalOpts = {}
    self.subOpts = {}
    self.overrideOpts = {}
    self.mode = "SYSTEM"
    self.configFile = None
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
    index = 0
    while index < len(args):
      arg = args[index]
      index = index+1
      matchResult = findall(CommandLine.KEY_VALUE_OPT_PATTERN, arg)
      if "--" == arg:
        doubleDashSeen = True
        self.followUpCommand = []
      elif doubleDashSeen:
        self.followUpCommand.append(arg)
      elif "-c" == arg:
        if index >= len(args):
          raise MechanicException("-c requires config file argument.")
        self.configFile = args[index]
        index = index+1
      elif "-m" == arg:
        if index >= len(args) or args[index].upper() not in ["USER", "SYSTEM"]:
          raise MechanicException("-m requires argument user|system.")
        self.mode = args[index].upper()
        index = index+1
      elif "-o" == arg:
        if index >= len(args):
          raise MechanicException("-o requires key=value.")
        keyValueMatchResult = findall(CommandLine.KEY_VALUE_PATTERN, args[index])
        if len(keyValueMatchResult) == 0:
          raise MechanicException("-o requires key=value.")
        self.overrideOpts[keyValueMatchResult[0][0]] = keyValueMatchResult[0][1]
        index = index+1
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

