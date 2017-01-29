#!/usr/bin/python
# -*- coding: UTF-8 -*-

from logger import Logger
from migrate_command import MigrateCommand
from list_migrations_command import ListMigrationsCommand
from help_command import HelpCommand
from version_command import VersionCommand
from inventory import Inventory
from config import Config
from config_reader import ConfigReader

class Mechanic:
  commands = { 
    'migrate': MigrateCommand,
    'list-migrations': ListMigrationsCommand,
    'version': VersionCommand,
    'help':HelpCommand }

  def __init__(self):
    self.logger = Logger()
    self.config = self.__loadConfig()
    self.inventory = None

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
    if self.inventory is None:
       self.inventory = Inventory(self.logger, self.config)
    return self.inventory

