#/bin/bash
set -x
if [ $SERVICE_ADDR ]; then
	sed -i "s#SERVICE_ADDR#${SERVICE_ADDR}#" /usr/local/conf/config.toml
else
	sed -i "s#SERVICE_ADDR#dns://localhost:8091#" /usr/local/conf/config.toml
fi

cat /usr/local/conf/config.toml
./qza-client