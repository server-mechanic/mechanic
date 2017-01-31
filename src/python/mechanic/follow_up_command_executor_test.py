#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
from follow_up_command_executor import FollowUpCommandExecutor
from mock import MagicMock
from exceptions import MigrationFailedException

class FollowUpCommandExecutorTest(unittest.TestCase):
  def setUp(self):
    config = MagicMock()
    self.executor = FollowUpCommandExecutor(config)
    self.executor.execve = MagicMock()

  def testExecSuccess(self):
    self.executor.execve.return_value = 0
    self.executor.executeFollowUpCommand(["/bin/true"])
    self.executor.execve.assert_called_once()

  def testExecFailure(self):
    self.executor.execve.return_value = 1
    self.executor.executeFollowUpCommand(["/bin/false"])
    self.executor.execve.assert_called_once()
