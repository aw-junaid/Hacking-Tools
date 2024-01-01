from stem.control import Controller

controller = Controller.from_port(port=9051)
controller.authenticate()

entries = controller.get_network_statuses()
for routerEntry in entries:
    print('Nickname:',routerEntry.nickname)
    print('Fingerprint:',routerEntry.fingerprint)

