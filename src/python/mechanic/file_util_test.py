#!/usr/bin/python
# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
import unittest
from mechanic import file_util
import os
import tempfile

class MakedirsTest(unittest.TestCase):
  def setUp(self):
    self.tempDir = tempfile.mkdtemp()

  def testTouchCreatesFile(self):
    tempFile = os.path.join(self.tempDir, "file")
    self.assertFalse(os.path.isfile(tempFile))
    file_util.touch(tempFile)
    self.assertTrue(os.path.isfile(tempFile))

  def testMakeDirsIsQuietIfDirAlreadyExists(self):
    files = file_util.makedirs(self.tempDir)
    self.assertTrue(os.path.isdir(self.tempDir))

  def testMakeDirsMakesDirs(self):
    path = os.path.join(self.tempDir, "a/b/c")
    files = file_util.makedirs(path)
    self.assertTrue(os.path.isdir(path))

  def testMakeParentDirsMakesDirsOfParent(self):
    path = os.path.join(self.tempDir, "a/b/c")
    parentPath = os.path.dirname(path)
    files = file_util.makeparentdirs(path)
    self.assertTrue(os.path.isdir(parentPath))
    self.assertFalse(os.path.isdir(path))

  def testMakeDirsFailsWhenFileIsInWay(self):
    tempFile = tempfile.mkstemp()[1]
    with self.assertRaises(Exception) as context:
      file_util.makedirs(tempFile)
    self.assertTrue('already is a file' in str(context.exception))
  
  def testMakeDirsFailsWhenParentNotWritable(self):
    os.chmod(self.tempDir,0o0500)
    with self.assertRaises(Exception) as context:
      file_util.makedirs(os.path.join(self.tempDir,"sub"))
    self.assertTrue('ermission denied' in str(context.exception))
