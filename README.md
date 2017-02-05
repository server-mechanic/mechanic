# Server Mechanic [![Written in Python](https://img.shields.io/badge/python-2.7,%203.2,%203.3,%203.4,%203.5-blue.svg)] [![Build Status](https://travis-ci.org/server-mechanic/mechanic.svg)](https://travis-ci.org/server-mechanic/mechanic) [![Test Coverage](https://codecov.io/gh/server-mechanic/mechanic/branch/master/graph/badge.svg)](https://codecov.io/gh/server-mechanic/mechanic) [![Release](https://img.shields.io/github/release/server-mechanic/mechanic.svg)](https://github.com/server-mechanic/mechanic/releases/latest) [![License](https://img.shields.io/badge/License-Apache%202.0-lightgrey.svg)](https://opensource.org/licenses/Apache-2.0)

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
Copyright (c) 2016,2017 by the [Server Mechanic Team](https://github.com/orgs/server-mechanic/people).

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

[http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

## Trademark hints
Docker is a trademark or registered trademark of Docker, Inc.; Puppet is a trademark or registered trademark of Puppet, Inc.; Ansible is a trademark or registered trademark of Red Hat, Inc.
