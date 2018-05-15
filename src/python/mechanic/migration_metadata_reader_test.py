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
        fileHandle.write("""# mechanic-prop-True: True
#  mechanic-prop-true: true
#  mechanic-prop-False: False
 # mechanic-prop-false: false
#  mechanic-prop-int: 123
 # mechanic-prop-sqstring: 'test'
# mechanic-prop-dqstring: "test"
 // mechanic-prop-doubleslash-comment: "test"
""")

    def testRecognizesFlags(self):
      metadata = self.metadataReader.readMetadata(self.tempFile.name)
      self.assertEqual(metadata.props["mechanic-prop-True"], True)
      self.assertEqual(metadata.props["mechanic-prop-true"], True)
      self.assertEqual(metadata.props["mechanic-prop-False"], False)
      self.assertEqual(metadata.props["mechanic-prop-false"], False)
      self.assertEqual(metadata.props["mechanic-prop-int"], 123)
      self.assertEqual(metadata.props["mechanic-prop-sqstring"], "test")
      self.assertEqual(metadata.props["mechanic-prop-dqstring"], "test")
      self.assertEqual(metadata.props["mechanic-prop-doubleslash-comment"], "test")
