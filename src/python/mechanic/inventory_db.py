#!/usr/bin/python
# -*- coding: UTF-8 -*-

import sqlite3
import os
from mechanic.file_util import makeparentdirs
from mechanic.Migration import Migration

class InventoryDb:
  def __init__(self, dbFile):
    makeparentdirs(dbFile)
    self.conn = sqlite3.connect(dbFile)
    self.cursor = self.conn.cursor()
    self.cursor.execute("create table if not exists migration ( id integer not null primary key, name text not null unique, start_time timestamp not null, end_time timestamp, status text check ( status in ( 'STARTED', 'FAILURE', 'SUCCESS' ) ) );");

  def markMigrationAsStarted(self, migrationName):
    self.cursor.execute("insert or replace into migration ( id, name, start_time, end_time, status ) values ( (select id from migration where name = ?), ?, strftime('%Y-%m-%d %H:%M:%f', 'now'), NULL, 'STARTED' )", (migrationName, migrationName) )
    self.conn.commit()

  def markMigrationAsFailed(self, migrationName):
    self.cursor.execute("update migration set status='FAILURE', end_time=strftime('%Y-%m-%d %H:%M:%f', 'now') where name = ?", (migrationName,) )
    self.conn.commit()

  def markMigrationAsSucceeded(self, migrationName):
    self.cursor.execute("update migration set status='SUCCESS', end_time=strftime('%Y-%m-%d %H:%M:%f', 'now') where name = ?", (migrationName,) )
    self.conn.commit()

  def hasMigrationSucceeded(self, migrationName):
    self.cursor.execute("SELECT count(*) FROM migration WHERE name = ? AND status = 'SUCCESS'", (migrationName,));
    count = self.cursor.fetchone()[0]
    succeeded = count > 0
    return succeeded

  def listMigrations(self, orderBy):
    migrations = []
    if orderBy == 'id':
      sql = "SELECT id, name, start_time, end_time, status FROM migration ORDER BY id asc"
    elif orderBy == 'start_date':
      sql = "SELECT id, name, start_time, end_time, status FROM migration ORDER BY start_time asc"
    else:
      sql = "SELECT id, name, start_time, end_time, status FROM migration ORDER BY id asc"

    for row in self.cursor.execute(sql):
      id=row[0]
      file=None
      name=row[1]
      startTime=row[2]
      endTime=row[3]
      status=row[4]
      migrations.append(Migration(id,file,name,startTime, endTime, status))

    return migrations

  def close(self):
    self.cursor = None
    self.conn.close()
    self.conn = None
