FROM ubuntu:bionic

env VERSION=V1

COPY conf/config.toml /usr/local/conf/
COPY server/qza-server /usr/local/bin/
RUN mkdir /usr/local/log

WORKDIR /usr/local/bin
ENTRYPOINT qza-server
