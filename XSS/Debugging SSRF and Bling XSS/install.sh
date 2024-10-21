#!/bin/bash

# install correct Ruby version
rvm install 2.3

# generate SSL certificate for port 8443
if [ ! -f ssl/8443.pem ]
then
  openssl req -x509 -newkey rsa:2048 -keyout ssl/8443.pem \
    -out ssl/8443.pem -days 365 -nodes \
    -subj "/C=US/ST=California/L=San Francisco/O=HackerOne, Inc./OU=NOC/CN=does.not.matter"
fi

# create access log
if [ ! -f logs/access_log ]
then
  touch logs/access_log
fi

# seed empty configuration
if [ ! -f config.json ]
then
  echo '{"callback_tokens":{}}' > config.json
fi
