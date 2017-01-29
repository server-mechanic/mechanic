#!/usr/bin/python
# -*- coding: UTF-8 -*-

from mechanic.MechanicLogger import MechanicLogger
from mechanic.MigrateCommand import MigrateCommand
from mechanic.ListMigrationsCommand import ListMigrationsCommand
from mechanic.HelpCommand import HelpCommand
from mechanic.VersionCommand import VersionCommand
from mechanic.Inventory import Inventory
from config import Config
from config_reader import ConfigReader

class Mechanic:
  commands = { 
      'migrate': MigrateCommand,
      'list-migrations': ListMigrationsCommand,
      'version': VersionCommand,
      'help':HelpCommand }

  def __init__(self, args):
    self.logger = MechanicLogger()
    self.config = self.__loadConfig()
    self.inventory = Inventory(self.logger, self.config)

  def __loadConfig(self):
    configReader = ConfigReader(self.logger)
    return configReader.loadConfig()

  def getLogger(self):
    return self.logger

  def getCommand(self, commandName):
    return Mechanic.commands.get(commandName)

  def getDefaultCommand(self):
    return Mechanic.commands.get("help")

  def getCommands(self):
    return Mechanic.commands

  def getConfig(self):
    return self.config;

  def getInventory(self):
    return self.inventory

