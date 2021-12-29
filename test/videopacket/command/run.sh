cd /usr/local/bin

if [[ $version == "add" ]]; then
  echo "add ...."
  ./videopacket-server-add
else
  echo "multication ...."
  ./videopacket-server-multication
fi
