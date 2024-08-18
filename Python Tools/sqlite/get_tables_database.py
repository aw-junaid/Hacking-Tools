#!/usr/bin/env python3

import sqlite3

connection = sqlite3.connect('database.sqlite')

def tables_in_sqlite_database(connection):
    cursor = connection.execute("SELECT name FROM sqlite_master WHERE type='table';")
    tables = [
        v[0] for v in cursor.fetchall()
        if v[0] != "sqlite_sequence"
    ]
    cursor.close()
    return tables

tables = tables_in_sqlite_database(connection)
tables.remove('Order')

cursor = connection.cursor()

for table in tables:
    sql="select * from {}".format(table)
    cursor.execute(sql)
    records = cursor.fetchall()
    print(sql+" "+ str(len(records))+" elements")

connection.close()
