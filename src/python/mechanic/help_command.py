#!/usr/bin/python
# -*- coding: UTF-8 -*-

class HelpCommand:
  name = 'help'
  description = 'Print this help message.'

  def __init__(self, mechanic):
    self.mechanic = mechanic

  def run(self, args):
    print("usage: mechanic [ -v ] [ -c file ] [ -m SYSTEM|USER ] [ -o key=value ]* <command> [ <args> ]")
    print("\nglobal opts:")
    print("  -v             - increase logging output")
    print("  -c file        - specifiy location of config file")
    print("  -m SYSTEM|USER - run in system or user mode")
    print("  -o key=value   - override config option, e.g. main.log_file=/dev/null")
    print("\ncommands:")
    for commandName, commandClass in self.mechanic.commands.iteritems():
      print("  %-15s - %s" % ( commandName, commandClass.description ))
