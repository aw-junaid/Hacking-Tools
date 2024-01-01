from stem.control import Controller

controller = Controller.from_port(port=9051)
controller.authenticate()

print(controller.get_info('circuit-status'))

