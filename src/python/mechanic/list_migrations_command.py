#!/usr/bin/python
# -*- coding: UTF-8 -*-

from config import Config
from inventory import Inventory
from exceptions import MechanicException

class ListMigrationsCommand:
  description = "List migrations."
  name = 'list-migrations'

  def __init__(self, mechanic):
    self.inventory = mechanic.getInventory()

  def run(self, args):
    orderBy = args.getSubOpt("order-by", "start_date")
    if orderBy != "start_date" and orderBy != "id":
      raise MechanicException("Invalid --order-by value: %s." % (orderBy))

    for migration in self.inventory.listMigrations(orderBy):
      print "%d\t%s\t%s\t%s\t%s" % (migration.id, migration.name, migration.startTime, migration.endTime, migration.status)
