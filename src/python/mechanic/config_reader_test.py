#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from config_reader import ConfigReader
from logger import Logger
from config import Config

class ConfigReaderTest(unittest.TestCase):
    def setUp(self):
      logger = Logger()
      self.configReader = ConfigReader(logger)

    def testWorksWithoutConfigFile(self):
      config = Config()
      config.logFile = "/tmp/doesNotExistConfig"
      configRead = self.configReader.loadConfig(config)
      self.assertEquals(config, configRead)

