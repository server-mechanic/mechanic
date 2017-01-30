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
  defaultCommand = HelpCommand

  def __init__(self):
    self.logger = Logger()
    self.config = self.__loadConfig()
    self.inventory = Inventory(self.logger, self.config)

  def __loadConfig(self):
    configReader = ConfigReader(self.logger)
    return configReader.loadConfig()

