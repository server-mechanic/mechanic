#!/usr/bin/python
# -*- coding: UTF-8 -*-

from os.path import expanduser,expandvars, isfile
from os import getenv
import ConfigParser
from string import join, split

class Config:
  def __init__(self, mode):
    self.mode = mode
    self.mechanicRootDir = getenv("MECHANIC_ROOT_DIR", "")
    if mode != "USER":
      self.configFile = "${MECHANIC_ROOT_DIR}/etc/mechanic.conf"
      self.logFile = ""
      self.migrationDirs = ["${MECHANIC_ROOT_DIR}/etc/mechanic/migration.d", "${MECHANIC_ROOT_DIR}/var/lib/mechanic/migration.d"]
      self.preMigrationDirs = ["${MECHANIC_ROOT_DIR}/etc/mechanic/pre-migration.d", "${MECHANIC_ROOT_DIR}/var/lib/mechanic/pre-migration.d"]
      self.postMigrationDirs = ["${MECHANIC_ROOT_DIR}/etc/mechanic/post-migration.d", "${MECHANIC_ROOT_DIR}/var/lib/mechanic/post-migration.d"]
      self.stateDir = "${MECHANIC_ROOT_DIR}/var/lib/mechanic/state"
      self.runDir = "${MECHANIC_ROOT_DIR}/var/lib/mechanic/tmp"
    else:
      self.configFile = "${HOME}/.mechanic/mechanic.conf"
      self.logFile = ""
      self.migrationDirs = ["${HOME}/.mechanic/migration.d"]
      self.preMigrationDirs = ["${HOME}/.mechanic/pre-migration.d"]
      self.postMigrationDirs = ["${HOME}/.mechanic/post-migration.d"]
      self.stateDir = "${HOME}/.mechanic/state"
      self.runDir = "${HOME}/.mechanic/tmp"

  def getMigrationTmpDir(self):
    return self.__expand(self.runDir)
  
  def getLogFile(self):
    return self.__expand(self.logFile)

  def getConfigFile(self):
    return self.__expand(self.configFile)

  def __expand(self, path):
    path = path.replace("${MECHANIC_ROOT_DIR}", self.mechanicRootDir)
    path = expanduser(path)
    path = expandvars(path)
    return path

  def getPreMigrationDirs(self):
    dirs = []
    for dir in self.preMigrationDirs:
      dirs.append(self.__expand(dir))

    return dirs

  def getMigrationDirs(self):
    dirs = []
    for dir in self.migrationDirs:
      dirs.append(self.__expand(dir))

    return dirs

  def getPostMigrationDirs(self):
    dirs = []
    for dir in self.postMigrationDirs:
      dirs.append(self.__expand(dir))

    return dirs

  def getInventoryDbFile(self):
    return self.__expand(self.stateDir+"/inventory.db")

