from mechanic_command import MechanicCommand
from context import Mechanic
import sys

if __name__ == '__main__':
  exitCode = MechanicCommand(Mechanic()).run(sys.argv[1:])
  sys.exit(exitCode)
