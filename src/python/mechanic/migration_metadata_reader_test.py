# -*- coding: UTF-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
import unittest
from mechanic.migration_metadata_reader import MigrationMetadataReader
import logging
import tempfile


class MigrationMetadataReaderTest(unittest.TestCase):
  def setUp(self):
    logger = logging.getLogger()
    self.metadataReader = MigrationMetadataReader(logger=logger)

    self.tempFile = tempfile.NamedTemporaryFile(delete=False)
    with self.tempFile as fileHandle:
      fileHandle.write("""# mechanic-repeat: always
#  mechanic-run-as: user
""")

  def testRecognizesFlags(self):
    metadata = self.metadataReader.readMetadata(self.tempFile.name)
    self.assertEqual(metadata.props["repeat"], True)
    self.assertEqual(metadata.props["run-as"], "user")
