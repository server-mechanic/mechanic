# Server Mechanic ![Written in Python](https://img.shields.io/badge/python-2.7,%203.2,%203.3,%203.4,%203.5-blue.svg) [![Build Status](https://travis-ci.org/server-mechanic/mechanic.svg)](https://travis-ci.org/server-mechanic/mechanic) [![Test Coverage](https://codecov.io/gh/server-mechanic/mechanic/branch/master/graph/badge.svg)](https://codecov.io/gh/server-mechanic/mechanic) [![Release](https://img.shields.io/github/release/server-mechanic/mechanic.svg)](https://github.com/server-mechanic/mechanic/releases/latest) [![License](https://img.shields.io/badge/License-GPL%203-yellow.svg)](http://www.gnu.org/licenses/)

### Server Evolution Made Easy.
![Server Mechanic](https://server-mechanic.github.io/website/images/mechanic_small.png "Server Mechanic")

#### Evolve your servers, virtual machines and containers more easily.
Server Mechanic is a tool to apply change scripts to your servers in a reliable and reproducable way.

### Pros
* You evolve your servers step by step.
* You author server migrations in your favorite language.
* No abstractions - you simply use your daily tools (apt, yum, cp, sed, ...)
* You focus on the changes instead of developing reusable plugins.

### Cons
* List of migrations can get very long for new installs.

Server Mechanic is considered as a lightweight alternative to tools like puppet, ansible, etc.

[How it works...](doc/HOWITWORKS.md)

## Features

* Tested platforms: Linux
* Available as platform package for Debian and Ubuntu.
* Written in Python.
* Uses battle proof sqlite3 database.

## Status
Server Mechanic is in an early stage ("unstable"). Use it carefully and expect things to break.

We appreciate any feedback about the tool and your usage scenarios so we can learn and improve Server Mechanic.

## [Installation](doc/INSTALLATION.md)

## [History](doc/HISTORY.md)

## Contributing
Server Mechanic is an open source project, and contributions are welcome! Feel free to raise an issue or submit a pull request.

## License

Copyright (c) 2016, 2017 by the [Server Mechanic Team](https://github.com/orgs/server-mechanic/people).

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a [copy of the GNU General Public License](license)
along with this program.  If not, see [GNU GPL v3 at http://www.gnu.org](http://www.gnu.org/licenses/gpl-3.0.txt).

## Trademark hints
Docker is a trademark or registered trademark of Docker, Inc.; Puppet is a trademark or registered trademark of Puppet, Inc.; Ansible is a trademark or registered trademark of Red Hat, Inc.
