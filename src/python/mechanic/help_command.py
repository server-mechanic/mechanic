#!/usr/bin/python
# -*- coding: UTF-8 -*-

class HelpCommand:
  name = 'help'
  description = 'Print this help message.'

  def __init__(self, mechanic):
    self.mechanic = mechanic

  def run(self, args):
    print "usage: mechanic [ -v ] <command> [ <args> ]"
    print "\ncommands:"
    for commandName, commandClass in self.mechanic.commands.iteritems():
      print "  %s - %s" % ( commandName, commandClass.description )
