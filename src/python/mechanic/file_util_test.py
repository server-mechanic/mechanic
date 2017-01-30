#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
import file_util
import os
import tempfile

class CollectfilesTest(unittest.TestCase):
  def setUp(self):
    self.tempDir = tempfile.mkdtemp()

  def testCollectingFromEmptyListGivesEmptyList(self):
    files = file_util.collectfiles([])
    self.assertEqual(files, [])

  def testNonExistentDirsIgnored(self):
    files = file_util.collectfiles(["/doesNotExist", "/thisToo"])
    self.assertEqual(files, [])

  def testCollectingFromEmptyDirGivesEmptyList(self):
    files = file_util.collectfiles([self.tempDir])
    self.assertEqual(files, [])

  def testCollectingFromDirGivesFileList(self):
    tempFile = tempfile.mkstemp(dir=self.tempDir)[1]
    files = file_util.collectfiles([self.tempDir])
    self.assertEqual(files, [tempFile])

class MakedirsTest(unittest.TestCase):
  def setUp(self):
    self.tempDir = tempfile.mkdtemp()

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
    os.chmod(self.tempDir,0500)
    with self.assertRaises(Exception) as context:
      file_util.makedirs(os.path.join(self.tempDir,"sub"))
    self.assertTrue('ermission denied' in str(context.exception))
