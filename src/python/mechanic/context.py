#!/usr/bin/python
# -*- coding: UTF-8 -*-

import logging
from migrate_command import MigrateCommand
from list_migrations_command import ListMigrationsCommand
from help_command import HelpCommand
from version_command import VersionCommand
from inventory import Inventory
from config import Config
from config_reader import ConfigReader

class Mechanic:
  def __init__(self, logger=None):
    if logger is None:
      logging.basicConfig(format="%(asctime)-15s %(levelname)s [%(filename)s:%(lineno)d-%(thread)d] %(message)s")
      logger = logging.getLogger()
      logger.setLevel(logging.INFO)
      logger.addHandler(logging.StreamHandler())
    self.logger = logger
    self.config = self.__loadConfig()
    self.inventory = Inventory(logger, self.config)
    self.commands = { 
      'migrate': MigrateCommand(self),
      'list-migrations': ListMigrationsCommand(self),
      'version': VersionCommand(self),
      'help':HelpCommand(self) }
    self.defaultCommand = HelpCommand(self)

  def __loadConfig(self):
    configReader = ConfigReader(self.logger)
    return configReader.loadConfig()

