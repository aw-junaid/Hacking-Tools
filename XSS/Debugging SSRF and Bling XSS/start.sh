#!/bin/bash

ruby app/server.rb -p 80 &
PORT_80_PID=$!

if [ -f ssl/443.pem ]
then
  ruby app/server.rb -p 443 -cert ssl/443.pem &
  PORT_443_PID=$!
fi

ruby app/server.rb -p 8080 &
PORT_8080_PID=$!

ruby app/server.rb -p 8443 -cert ssl/8443.pem &
PORT_8443_PID=$!

read -p "Press any key to quit..." -n1 -s

kill $PORT_80_PID
if [[ ! -z $PORT_443_PID ]]
then
  kill $PORT_443_PID
fi
kill $PORT_8080_PID
kill $PORT_8443_PID
