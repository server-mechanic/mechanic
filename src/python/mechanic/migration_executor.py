#!/usr/bin/python
# -*- coding: UTF-8 -*-

from config import Config
from inventory import Inventory
from exceptions import MigrationFailedException
from mechanic.file_util import makedirs
import os
import shutil
import subprocess

class MigrationExecutor:
  def __init__(self, config, inventory, logger):
    self.config = config
    self.inventory = inventory
    self.logger = logger

  def applyMigration(self, migration):
    if not os.access(migration.file, os.X_OK):
      raise MigrationFailedException("Migration %s is not executable." % migration.name)

    self.inventory.markMigrationAsStarted(migration.name)
    migrationTmpDir = os.path.join( self.config.getMigrationTmpDir(), migration.name + ".tmp" )
    logFile = os.path.join(migrationTmpDir, "log")
    try:
      makedirs(migrationTmpDir)
      logFileFd = open(logFile, 'wa')
      migrationProcess = subprocess.Popen([migration.file],bufsize=0,stdout=logFileFd,stderr=logFileFd,stdin=None,shell=False,env=None,)
      exitCode = migrationProcess.wait()
      if exitCode != 0:
        raise MigrationFailedException("Migration %s failed with exit code %d." % (migration.name, exitCode) )

      self.inventory.markMigrationAsSucceeded(migration.name)
      shutil.rmtree(migrationTmpDir)
    except MigrationFailedException as e:
      self.inventory.markMigrationAsFailed(migration.name)
      raise e
    except Exception as e:
      self.inventory.markMigrationAsFailed(migration.name)
      raise e
    finally:
      self.__copyMigrationOutput(migration, logFile)

  def __copyMigrationOutput(self, migration, logFile):
      if os.path.isfile(logFile):
        with open(logFile) as f:
          for line in f:
            self.logger.info("%s: %s" % (migration.name, line.strip()))

