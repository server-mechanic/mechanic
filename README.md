# Server Mechanic [![Build Status](https://travis-ci.org/server-mechanic/mechanic.svg)](https://travis-ci.org/server-mechanic/mechanic) [![Release](https://img.shields.io/github/release/server-mechanic/mechanic.svg)](https://github.com/server-mechanic/mechanic/releases/latest) [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)

### Server Evolution Made Easy.

![Server Mechanic](https://server-mechanic.github.io/website/images/mechanic.png "Server Mechanic")

#### Evolve your containers and virtual machines more easily.

## What Server Mechanic is for?

With Server Mechanic you (admin or dev) author change scripts in your favorite language, e.g. shell scripts. The Server Mechanic collects these migration scripts, executes them and records them as applied in an internal database. Next time Server Mechanic is run the migrations already applied will be skipped and only new migrations will be executed. In case of errors you fix the problem and start over.

Server Mechanic is considered as a lightweight alternative when tools like puppet, ansible, etc. seem to be oversized.

### Server Mechanic for docker containers

Docker already utilizes the step by step approach with the commands you put into your Dockerfile. These commands are run at container image build time. So you have full access to the container's internal data. External volumes injected into a newly created container cannot be manipulated with Dockerfile commands. Here Server Mechanic comes into play.

Server Mechanic gets executed on container startup, migrates state of the volumes mounted and replaces itself with the actual enty point so container startup can proceed. So you can roll out adjustments to external volumes with new container versions so container and volume stay in sync.

## Features

### Pros
* Evolve your servers step by step.
* Write server migrations in your favorite language.
* Focus on the migrations instead of developing reusable plugins.
* Minimal overhead, no central server required.

### Cons
* List of migrations can get very long for new installs.

## Status

Server Mechanic is in an early and experimental state. We appreciate feedback about any real world usage so we can learn and improve Server Mechanic.

## Installation

For installation of Server Mechanic *unstable* see [Server Mechanic Packages Repository](https://github.com/server-mechanic/packages).

## History

[History](HISTORY.md)

## TODOs
[TODOs](TODOs.md)

## Contributing

Server Mechanic is an open source project, and contributions are welcome! Feel free to raise an issue or submit a pull request.

## License
Copyright (c) 2016 by the [Server Mechanic Team](https://github.com/orgs/server-mechanic/people).

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a [copy of the GNU General Public License](LICENSE)
along with this program.  If not, see [GNU GPL v3 at http://www.gnu.org](http://www.gnu.org/licenses/gpl-3.0.txt).

## Trademark hints
Docker is a trademark or registered trademark of Docker, Inc.
Puppet is a trademark or registered trademark of Puppet, Inc.
Ansible is a trademark or registered trademark of Red Hat, Inc.
