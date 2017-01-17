#!/usr/bin/python
# -*- coding: UTF-8 -*-

from mechanic.Config import Config
from mechanic.Inventory import Inventory

class ListMigrationsCommand:
  description = "List migrations."
  name = 'list-migrations'

  def __init__(self, mechanic):
    self.inventory = mechanic.getInventory()

  def run(self, args):
    for migration in self.inventory.listMigrations():
      print "%d\t%s\t%s\t%s\t%s" % (migration.getId(), migration.getName(), migration.getStartTime(), migration.getEndTime(), migration.getStatus())
