#!/usr/bin/python
# -*- coding: UTF-8 -*-

from os.path import isfile
import ConfigParser
from config import Config
from string import join, split

class ConfigReader:
  def __init__(self, logger):
    self.logger = logger

  def loadConfig(self):
    config = Config()
    configFile = config.getConfigFile()
    if not isfile(configFile):
      self.logger.debug("Config file %s does not exist. Using defaults." % configFile)
      return config

    self.logger.debug("Loading config from %s." % configFile)
    loader = ConfigParser.SafeConfigParser(allow_no_value=True)
    loader.add_section("main")
    loader.set("main", "log_file", config.logFile)
    loader.set("main", "migration_dirs", join(config.migrationDirs, ":"))
    loader.set("main", "pre_migration_dirs", join(config.preMigrationDirs, ":"))
    loader.set("main", "post_migration_dirs", join(config.postMigrationDirs, ":"))
    loader.read(configFile)

    config.logFile = loader.get("main", "log_file")
    config.migrationDirs = split(loader.get("main", "migration_dirs"), ":")

    return config
