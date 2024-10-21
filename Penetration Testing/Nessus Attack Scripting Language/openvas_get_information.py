#!/usr/bin/env python3

import gvm
from gvm.protocols.latest import Gmp
from gvm.transforms import EtreeCheckCommandTransform
from gvm.errors import GvmError

connection = gvm.connections.TLSConnection(hostname='localhost')

username = 'admin'
password = 'admin'

transform = EtreeCheckCommandTransform()

try:
    with Gmp(connection=connection, transform=transform) as gmp:
        gmp.authenticate(username, password)

        users = gmp.get_users()
        tasks = gmp.get_tasks()
        targets = gmp.get_targets()
        scanners = gmp.get_scanners()
        configs = gmp.get_configs()
        feeds = gmp.get_feeds()
        nvts = gmp.get_nvts()
        print("Users\n------------")
        for user in users.xpath('user'):
            print(user.find('name').text)
        print("\nTasks\n------------")
        for task in tasks.xpath('task'):
            print(task.find('name').text)
        print("\nTargets\n-------------")
        for target in targets.xpath('target'):
            print(target.find('name').text)
            print(target.find('hosts').text)
        print("\nScanners\n-------------")
        for scanner in scanners.xpath('scanner'):
            print(scanner.find('name').text)
        print("\nConfigs\n-------------")
        for config in configs.xpath('config'):
            print(config.find('name').text)
        print("\nFeeds\n-------------")
        for feed in feeds.xpath('feed'):
            print(feed.find('name').text)
        print("\nNVTs\n-------------")
        for nvt in nvts.xpath('nvt'):
            print(nvt.attrib.get('oid'),"-->",nvt.find('name').text)


except GvmError as error:
    print('Error connection with server:', error)
