#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from config_reader import ConfigReader
from logger import Logger
from config import Config
import tempfile

class ConfigReaderTest(unittest.TestCase):
    def setUp(self):
      logger = Logger()
      self.configReader = ConfigReader(logger)

      self.tempFile = tempfile.NamedTemporaryFile(delete=False)
      with self.tempFile as fileHandle:
        fileHandle.write("""[main]
log_file=${MECHANIC_ROOT_DIR}/aLogFile
pre_migration_dirs=${MECHANIC_ROOT_DIR}/aPreMigDir:${MECHANIC_ROOT_DIR}/bPreMigDir
migration_dirs=${MECHANIC_ROOT_DIR}/aMigDir:${MECHANIC_ROOT_DIR}/bMigDir
post_migration_dirs=${MECHANIC_ROOT_DIR}/aPostMigDir:${MECHANIC_ROOT_DIR}/bPostMigDir
run_dir=${MECHANIC_ROOT_DIR}/runDir
state_dir=${MECHANIC_ROOT_DIR}/stateDir
""")

    def testWorksWithoutConfigFile(self):
      config = Config()
      config.mechanicRootDir = "__ROOT__"
      config.configFile = self.tempFile.name
      configRead = self.configReader.loadConfig(config)
      self.assertEqual(config.getLogFile(), "__ROOT__/aLogFile")
      self.assertEqual(config.getInventoryDbFile(), "__ROOT__/stateDir/inventory.db")
      self.assertEqual(config.getMigrationDirs(), ["__ROOT__/aMigDir", "__ROOT__/bMigDir"])
      self.assertEqual(config.getPreMigrationDirs(), ["__ROOT__/aPreMigDir", "__ROOT__/bPreMigDir"])
      self.assertEqual(config.getPostMigrationDirs(), ["__ROOT__/aPostMigDir", "__ROOT__/bPostMigDir"])
      self.assertEqual(config.getMigrationTmpDir(), "__ROOT__/runDir")
