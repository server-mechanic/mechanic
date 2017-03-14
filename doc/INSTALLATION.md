# Installation

## Bare metal/ virtual machines
* Install mechanic
```
curl -s https://raw.githubusercontent.com/server-mechanic/packages/master/install-mechanic.sh | sudo bash -s unstable
```
* Place your migrations below /var/lib/mechanic/migration.d/ or /etc/mechanic/migration.d/. (Make sure they are executable and exit with 0 in case of success.)
* Start the Server Mechanic
```
mechanic migrate
```

## Docker container
* Add installation of mechanic to your Dockerfile
```
RUN curl -s https://raw.githubusercontent.com/server-mechanic/packages/master/install-mechanic.sh | sudo bash -s unstable
```
* Supposedly you want mechanic state to survice container restarts, so add a volume:
```
VOLUME /mechanic.volume
```
* Adjust /etc/mechanic.conf so mechanic will put state into the volume:
```
# /etc/mechanic.conf
[main]
log_file=/mechanic.volume/log/mechanic.log
pre_migration_dirs=/var/lib/mechanic/pre-migration.d/
migration_dirs=/var/lib/mechanic/migration.d/
post_migration_dirs=/var/lib/mechanic/post-migration.d/
run_dir=/mechanic.volume/tmp/
state_dir=/mechanic.volume/state
```
* Place your migrations below /var/lib/mechanic/migration.d/. (Make sure they are executable and exit with 0 in case of success.)
* Extend your container entrypoint so, that mechanic migrations will be run on container startup. Mechanic will replace itself with the actual endpoint.
```
ENTRYPOINT ["/usr/bin/mechanic", "migrate", "--", "PUT", "ACTUAL", "ENTRYPOINT", "HERE" ]
```
* Don't forget to start your container with the external volume mounted. (-v /path/to/volume:/mechanic.volume).