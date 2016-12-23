FROM ubuntu:xenial

ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update \
	&& apt-get install -y git gcc make sqlite3 libsqlite3-dev

VOLUME /build
