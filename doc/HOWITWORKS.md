# How it works

## Server Mechanic for servers and virtual machines
If you want to manage your machines in a reproducable way, one strategy is the creation of "infrastructure as code".

With Server Mechanic you author changes to your machines as scripts in your favorite language, e.g. shell scripts. Server Mechanic collects these migration scripts, executes them and records them as applied in an internal database. Next time only new migrations will be executed. In case of an error you fix the problem and start over.

## Server Mechanic for Docker containers
Docker already utilizes the step by step approach with the commands in your Dockerfile. These commands are run at container image build time. So you have full access to the container's internal state. But external volumes cannot be manipulated by Dockerfile commands. Here Server Mechanic comes into play.

Server Mechanic is started on container startup, applies your migration which migrates the state in your volumes, then container startup proceeds. So you can roll out adjustments to external volumes with new container versions and containers and volumes stay in sync.