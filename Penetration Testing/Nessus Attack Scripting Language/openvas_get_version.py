#!/usr/bin/env python3

import gvm
from gvm.protocols.latest import Gmp

connection = gvm.connections.TLSConnection(hostname='localhost')
with Gmp(connection=connection) as gmp:
    version = gmp.get_version()
    print(version)
