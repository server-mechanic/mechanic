#!/usr/bin/python
# -*- coding: UTF-8 -*-
  
from __future__ import absolute_import
from __future__ import unicode_literals
from os.path import isfile, isdir, join, basename
from mechanic.inventory_db import InventoryDb
from mechanic.migration import Migration
from mechanic.migration_collector import MigrationCollector

class Inventory:
  def __init__(self, logger, config):
    self.config = config
    self.inventoryDb = None
    self.logger = logger
    self.migrationCollector = MigrationCollector(logger)

  def listUnappliedMigrations(self):
    migrationDirs = self.config.getMigrationDirs()
    self.logger.debug("Scanning for migrations (dirs: %s)..." % migrationDirs)
    return self.migrationCollector.collectMigrationsFrom(migrationDirs, lambda file: self.__getInventoryDb().hasMigrationSucceeded(file))

  def listPreMigrations(self):
    migrationDirs = self.config.getPreMigrationDirs()
    self.logger.debug("Scanning for pre migrations (dirs: %s)..." % migrationDirs)
    return self.migrationCollector.collectMigrationsFrom(migrationDirs)

  def listPostMigrations(self):
    migrationDirs = self.config.getPostMigrationDirs()
    self.logger.debug("Scanning for post migrations (dirs: %s)..." % migrationDirs)
    return self.migrationCollector.collectMigrationsFrom(migrationDirs)

  def markMigrationAsStarted(self, migrationName):
    self.__getInventoryDb().markMigrationAsStarted(migrationName)

  def markMigrationAsFailed(self, migrationName):
    self.__getInventoryDb().markMigrationAsFailed(migrationName)
  
  def markMigrationAsSucceeded(self, migrationName):
    self.__getInventoryDb().markMigrationAsSucceeded(migrationName)

  def listMigrations(self, orderBy):
    return self.__getInventoryDb().listMigrations(orderBy)

  def close(self):
    if self.inventoryDb is not None:
      self.inventoryDb.close()
    self.inventoryDb = None

  def __getInventoryDb(self):
    if self.inventoryDb is None:
      self.inventoryDb = InventoryDb(self.config.getInventoryDbFile())
    return self.inventoryDb
