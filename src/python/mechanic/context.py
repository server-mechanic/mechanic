#!/usr/bin/python
# -*- coding: UTF-8 -*-

import logging
import os
from migrate_command import MigrateCommand
from list_migrations_command import ListMigrationsCommand
from help_command import HelpCommand
from version_command import VersionCommand
from inventory import Inventory
from config import Config
from config_reader import ConfigReader
from command_line import CommandLine

class Mechanic:
  def __init__(self, args, logger=None, mode=None):
    self.commandLine = CommandLine(args)
    if mode is not None:
      self.commandLine.mode = mode
    if logger is None:
      logging.basicConfig(format="%(asctime)-15s %(levelname)s [%(filename)s:%(lineno)d-%(thread)d] %(message)s")
      logger = logging.getLogger()
      logger.setLevel(logging.INFO)
    self.logger = logger

    self.mode = self.__detectMode(self.commandLine.mode)
    self.config = self.__loadConfig()
    self.inventory = Inventory(logger, self.config)
    self.commands = { 
      'migrate': MigrateCommand(self),
      'list-migrations': ListMigrationsCommand(self),
      'version': VersionCommand(self),
      'help':HelpCommand(self) }
    self.defaultCommand = HelpCommand(self)

  def __detectMode(self, mode=None):
    if mode is None:
      mode = "SYSTEM"
      if os.getenv("MECHANIC_ROOT_DIR") is None and os.getuid() != 0:
        mode = "USER"
    return mode

  def __loadConfig(self):
    config = Config(self.mode)
    self.__applyOverridesTo(config)
    configReader = ConfigReader(self.logger)
    config = configReader.loadConfig(config)
    self.__applyOverridesTo(config)
    return config

  def __applyOverridesTo(self, config):
    if self.commandLine.configFile is not None:
       config.configFile = self.commandLine.configFile
    if "main.log_file" in self.commandLine.overrideOpts:
      config.logFile = self.commandLine.overrideOpts["main.log_file"]
    if "main.migration_dirs" in self.commandLine.overrideOpts:
      config.migrationDirs = split(self.commandLine.overrideOpts["main.migration_dirs"], ":") 
    if "main.pre_migration_dirs" in self.commandLine.overrideOpts:
      config.preMigrationDirs = split(self.commandLine.overrideOpts["main.pre_migration_dirs"], ":") 
    if "main.post_migration_dirs" in self.commandLine.overrideOpts:
      config.postMigrationDirs = split(self.commandLine.overrideOpts["main.post_migration_dirs"], ":") 
    if "main.state_dir" in self.commandLine.overrideOpts:
      config.stateDir = self.commandLine.overrideOpts["main.state_dir"]
    if "main.run_dir" in self.commandLine.overrideOpts:
      config.runDir = self.commandLine.overrideOpts["main.run_dir"]
