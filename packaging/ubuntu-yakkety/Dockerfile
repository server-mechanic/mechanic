FROM ubuntu:yakkety

RUN apt-get update \
	&& apt-get install --no-install-recommends -y dpkg sqlite3 \
        && apt-get autoremove -y \
	&& rm -rf /var/lib/apt/lists/*

VOLUME /build
