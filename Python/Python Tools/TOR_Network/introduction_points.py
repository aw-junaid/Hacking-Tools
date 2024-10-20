from stem.control import Controller

with Controller.from_port(port = 9051) as controller:
  controller.authenticate()
  desc = controller.get_hidden_service_descriptor('3g2upl4pq6kufc4m')

  print("DuckDuckGo's introduction points are...\n")

  for introduction_point in desc.introduction_points():
    print('  %s:%s => %s' % (introduction_point.address, introduction_point.port, introduction_point.identifier))
