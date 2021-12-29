FROM alpine:3.13.5

COPY trpc_go.yaml /usr/local/bin/
COPY trpc-server /usr/local/bin/
WORKDIR /usr/local/bin
ENTRYPOINT ./trpc-server
