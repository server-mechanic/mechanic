# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
from mechanic.config import Config
from mechanic.inventory import Inventory
from mechanic.exceptions import MechanicException

class ListMigrationsCommand:
  description = "List migrations."
  name = 'list-migrations'

  def __init__(self, mechanic):
    self.inventory = mechanic.inventory

  def run(self, args):
    orderBy = args.getSubOpt("order-by", "start_date")
    if orderBy != "start_date" and orderBy != "id":
      raise MechanicException("Invalid --order-by value: %s." % (orderBy))

    migrations = self.inventory.listMigrations(orderBy)
    for migration in migrations:
      print("%d\t%s\t%s\t%s\t%s" % (migration.id, migration.name, migration.startTime, migration.endTime, migration.status))
