from stem import CircStatus
from stem.control import Controller

with Controller.from_port(port = 9051) as controller:
  controller.authenticate()

  for circ in sorted(controller.get_circuits()):
    if circ.status != CircStatus.BUILT:
      continue

    print("Circuit %s (%s)" % (circ.id, circ.purpose))

    for i, entry in enumerate(circ.path):
      div = '+' if (i == len(circ.path) - 1) else '|'
      fingerprint, nickname = entry

      desc = controller.get_network_status(fingerprint, None)
      address = desc.address if desc else 'unknown'

      print(" %s- %s (%s, %s)" % (div, fingerprint, nickname, address))
