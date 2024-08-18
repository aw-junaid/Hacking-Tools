#!/usr/bin/env python3

import sqlite3

def sqlite_table_schema(connection, table_name):
    cursor = connection.execute("SELECT sql FROM sqlite_master WHERE name=?;", [table_name])
    sql = cursor.fetchone()[0]
    cursor.close()
    return sql

connection = sqlite3.connect('database.sqlite')

table_name =input("Enter the table name:")
print(sqlite_table_schema(connection,table_name ))

connection.close()
