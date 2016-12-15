FROM ubuntu:xenial

ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update \
	&& apt-get install -y git gccgo build-essential sqlite3 libsqlite3-dev golang-go 

VOLUME /build
