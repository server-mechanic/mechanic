#!/usr/bin/python
# -*- coding: UTF-8 -*-
  
from os.path import isfile, isdir, join, basename
from inventory_db import InventoryDb
from migration import Migration
from migration_collector import MigrationCollector

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
    self.logger.debug("Scanning for pre migrations (dirs: %s)..." % migrationDirs)
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
