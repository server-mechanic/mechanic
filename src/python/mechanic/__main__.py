from __future__ import absolute_import
from __future__ import unicode_literals
from mechanic.mechanic_command import MechanicCommand
from mechanic.context import Mechanic
import sys

if __name__ == '__main__':
  exitCode = MechanicCommand(Mechanic(args=sys.argv[1:])).run()
  sys.exit(exitCode)
