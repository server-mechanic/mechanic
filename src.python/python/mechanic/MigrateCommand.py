#!/usr/bin/python
# -*- coding: UTF-8 -*-

from mechanic.Config import Config
from mechanic.Inventory import Inventory
from mechanic.MigrationExecutor import MigrationExecutor
from mechanic.file_util import makeparentdirs, makedirs
import os

class MigrateCommand:
  description = "Apply pending migrations."
  name = "migrate"

  def __init__(self, mechanic):
    self.inventory = mechanic.getInventory()
    self.config = mechanic.getConfig()
    self.migrationExecutor = MigrationExecutor(mechanic.getConfig(), mechanic.getInventory())
    self.logger = mechanic.getLogger()

  def run(self, args):
    migrations = self.inventory.listUnappliedMigrations()
    self.logger.debug("%s migration(s) found." % len(migrations))
    if len(migrations) > 0:
      makedirs(self.config.getMigrationTmpDir())
      for migration in migrations:
        self.logger.info("Applying migration %s..." % migration.getName())
        self.migrationExecutor.applyMigration(migration)

    if args.followUpCommand is not None:
      self.__execFollowUpCommand(args.followUpCommand)

  def __execFollowUpCommand(self, followUpCommand):
    try:
      print "executing %s" % followUpCommand[0]
      environment = { "MECHANIC_ROOT_DIR": self.config.mechanicRootDir }
      exitCode = os.execve(followUpCommand[0], followUpCommand, environment)
      if exitCode != 0:
        raise FollowUpCommandFailedException("Follow up command failed with exit code %s." % exitCode )
    except Exception as e:
      raise FollowUpCommandFailedException("Follow up command failed. %s" % e.message)
