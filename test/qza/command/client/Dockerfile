FROM ubuntu:bionic

COPY conf/config.toml /usr/local/conf/
COPY client/qza-client /usr/local/bin/
COPY client/run.sh /usr/local/bin/
RUN mkdir /usr/local/log

WORKDIR /usr/local/bin
ENTRYPOINT bash run.sh
