#!/usr/bin/python
# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
from os.path import isfile
import ConfigParser
from mechanic.config import Config
from string import join, split

class ConfigReader:
  def __init__(self, logger):
    self.logger = logger

  def loadConfig(self, config):
    configFile = config.getConfigFile()
    if not isfile(configFile):
      self.logger.warn("Config file %s does not exist. Using defaults." % configFile)
      return config

    self.logger.debug("Loading config from %s." % configFile)
    loader = ConfigParser.SafeConfigParser(allow_no_value=True)
    loader.add_section("main")
    loader.set("main", "log_file", config.logFile)
    loader.set("main", "migration_dirs", join(config.migrationDirs, ":"))
    loader.set("main", "pre_migration_dirs", join(config.preMigrationDirs, ":"))
    loader.set("main", "post_migration_dirs", join(config.postMigrationDirs, ":"))
    loader.set("main", "state_dir", config.stateDir)
    loader.set("main", "run_dir", config.runDir)
    loader.read(configFile)

    config.logFile = loader.get("main", "log_file")
    config.migrationDirs = split(loader.get("main", "migration_dirs"), ":")
    config.preMigrationDirs = split(loader.get("main", "pre_migration_dirs"), ":")
    config.postMigrationDirs = split(loader.get("main", "post_migration_dirs"), ":")
    config.stateDir = loader.get("main", "state_dir")
    config.runDir = loader.get("main", "run_dir")

    return config
