# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
from mechanic.inventory_db import InventoryDb
from mechanic.migration import Migration
from mechanic.migration_metadata_reader import MigrationMetadataReader
from os import listdir
from os.path import isfile, isdir, dirname, basename, join

class MigrationCollector:
  def __init__(self, logger):
    self.logger = logger
    self.metadataReader = MigrationMetadataReader(logger=logger)

  def collectMigrationsFrom(self, migrationDirs, isAppliedCallback=None, defaultMetadataProps=None):
    allMigrations = self.__collectMigrations(migrationDirs, defaultMetadataProps=defaultMetadataProps)
    pendingMigrations = []
    for migration in allMigrations:
      if isAppliedCallback is None or not isAppliedCallback(migration.name):
        pendingMigrations.append(migration)
      else:
        self.logger.debug("Migration %s already applied." % migration.name)
    return pendingMigrations

  def __collectMigrations(self, dirs, defaultMetadataProps):
    migrations = []
    for dir in dirs:
      if isdir(dir):
        for file in listdir(dir):
          file = join(dir, file)
          if isfile(file):
            metadata = self.metadataReader.readMetadata(file, defaultMetadataProps=defaultMetadataProps)
            migrations.append(Migration(None,file,basename(file),metadata=metadata))
          elif isdir(file):
            runFile = join(file,"run")
            if isfile(runFile):
              metadata = self.metadataReader.readMetadata(runFile, defaultMetadataProps=defaultMetadataProps)
              migrations.append(Migration(None,runFile,basename(file),metadata=metadata))
    migrations.sort(key=lambda m: m.name)
    return migrations
