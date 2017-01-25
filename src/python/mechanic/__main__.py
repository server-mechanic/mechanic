from mechanic.MechanicCommand import MechanicCommand
import sys

if __name__ == '__main__':
  exitCode = MechanicCommand(sys.argv[1:]).run()
  sys.exit(exitCode)
