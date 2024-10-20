#!/usr/bin/python3

import sqlite3
from sqlite3 import DatabaseError

def read_from_db(cursor):
  cursor.execute('SELECT * FROM Customer')
  data = cursor.fetchall()
  print(data)
  for row in data:
    print(row)

try:
    connection = sqlite3.connect("database.sqlite")
    cursor = connection.cursor()
    read_from_db(cursor)
except DatabaseError as exception:
    print("DatabaseError:",exception)
finally:
    connection.close()
