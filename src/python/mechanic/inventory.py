#!/usr/bin/python
# -*- coding: UTF-8 -*-
  
from os.path import isfile, isdir, join, basename
from inventory_db import InventoryDb
from mechanic.Migration import Migration
from mechanic.file_util import collectfiles

class Inventory:
  def __init__(self, logger, config):
    self.config = config
    self.inventoryDb = InventoryDb(config.getInventoryDbFile())
    self.logger = logger;

  def listUnappliedMigrations(self):
    migrationDirs = self.config.getMigrationDirs()
    self.logger.debug("Scanning for migrations (dirs: %s)..." % migrationDirs)
    return self.__collectMigrationsFrom(migrationDirs, True)

  def listPreMigrations(self):
    migrationDirs = self.config.getPreMigrationDirs()
    self.logger.debug("Scanning for pre migrations (dirs: %s)..." % migrationDirs)
    return self.__collectMigrationsFrom(migrationDirs, False)

  def listPostMigrations(self):
    migrationDirs = self.config.getPostMigrationDirs()
    self.logger.debug("Scanning for pre migrations (dirs: %s)..." % migrationDirs)
    return self.__collectMigrationsFrom(migrationDirs, False)

  def __collectMigrationsFrom(self, migrationDirs, checkIfApplied=True):
    files = collectfiles(migrationDirs)
    migrations = []
    for file in files:
      if not checkIfApplied or not self.inventoryDb.hasMigrationSucceeded(file):
        migrations.append(Migration(None,file,basename(file)))
      else:
        self.logger.debug("Migration %s already applied." % file)

    return migrations

  def markMigrationAsStarted(self, migrationName):
    self.inventoryDb.markMigrationAsStarted(migrationName)

  def markMigrationAsFailed(self, migrationName):
    self.inventoryDb.markMigrationAsFailed(migrationName)
  
  def markMigrationAsSucceeded(self, migrationName):
    self.inventoryDb.markMigrationAsSucceeded(migrationName)

  def listMigrations(self, orderBy):
    return self.inventoryDb.listMigrations(orderBy)

  def close(self):
    self.inventoryDb.close()
    self.inventoryDb = None
    self.config = None
