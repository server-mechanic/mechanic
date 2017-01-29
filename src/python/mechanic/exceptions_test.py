#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from exceptions import MechanicException

class MechanicExceptionTest(unittest.TestCase):
    def setUp(self):
      self.exception = MechanicException()

