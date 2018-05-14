# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
from mechanic.inventory_db import InventoryDb
from mechanic.migration import Migration
from os import listdir
from os.path import isfile, isdir, dirname, basename, join

class MigrationCollector:
  def __init__(self, logger):
    self.logger = logger

  def collectMigrationsFrom(self, migrationDirs, isAppliedCallback=None):
    allMigrations = self.__collectmigrations(migrationDirs)
    pendingMigrations = []
    for migration in allMigrations:
      if isAppliedCallback is None or not isAppliedCallback(migration.name):
        pendingMigrations.append(migration)
      else:
        self.logger.debug("Migration %s already applied." % migration.name)
    return pendingMigrations

  def __collectmigrations(self, dirs):
    migrations = []
    for dir in dirs:
      if isdir(dir):
        for file in listdir(dir):
          file = join(dir, file)
          if isfile(file):
            migrations.append(Migration(None,file,basename(file)))
          elif isdir(file):
            runFile = join(file,"run")
            if isfile(runFile):
              migrations.append(Migration(None,runFile,basename(file)))
    migrations.sort(key=lambda m: m.name)
    return migrations
