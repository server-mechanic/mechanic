#!/usr/bin/python
# -*- coding: UTF-8 -*-

from config import Config
from inventory import Inventory
from migration_executor import MigrationExecutor
from follow_up_command_executor import FollowUpCommandExecutor
from mechanic.file_util import makeparentdirs, makedirs
import os

class MigrateCommand:
  description = "Apply pending migrations."
  name = "migrate"

  def __init__(self, mechanic):
    self.inventory = mechanic.getInventory()
    self.config = mechanic.config
    self.migrationExecutor = MigrationExecutor(mechanic.config, mechanic.getInventory(), mechanic.logger)
    self.logger = mechanic.logger
    self.followUpCommandExecutor = FollowUpCommandExecutor(mechanic.config)

  def run(self, args):
    migrations = self.inventory.listUnappliedMigrations()
    self.logger.debug("%s migration(s) found." % len(migrations))
    if len(migrations) > 0:
      preMigrations = self.inventory.listPreMigrations()
      postMigrations = self.inventory.listPostMigrations()

      makedirs(self.config.getMigrationTmpDir())
      for migration in preMigrations:
        self.logger.info("Applying pre migration %s..." % migration.name)
        self.migrationExecutor.applyMigration(migration)

      for migration in migrations:
        self.logger.info("Applying migration %s..." % migration.name)
        self.migrationExecutor.applyMigration(migration)

      for migration in postMigrations:
        self.logger.info("Applying post migration %s..." % migration.name)
        self.migrationExecutor.applyMigration(migration)

    if args.followUpCommand is not None:
      self.followUpCommandExecutor.executeFollowUpCommand(args.followUpCommand)

