#!/usr/bin/python
# -*- coding: UTF-8 -*-

class MechanicCommandArgs:
  def __init__(self, args):
    self.args = args
    self.verbose = False
    self.followUpCommand = None
    self.commandName = None
    self.__process(args)

  def __process(self, args):
    doubleDashSeen = False
    commandSeen = False
    for arg in args:
      if "--" == arg:
        doubleDashSeen = True
        self.followUpCommand = []
      elif doubleDashSeen:
        self.followUpCommand.append(arg)
      elif "-v" == arg or "--verbose" == arg:
        self.verbose = True
      elif not arg.startswith("-"):
        if self.commandName is not None:
          raise MechanicException("Only single command allowed.")
        self.commandName = arg
    if self.followUpCommand is not None and len(self.followUpCommand) == 0:
      self.followUpCommand = None
