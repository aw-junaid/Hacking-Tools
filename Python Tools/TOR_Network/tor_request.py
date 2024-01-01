from torrequest import TorRequest

with TorRequest(proxy_port=9050, ctrl_port=9051, password=None) as tr:
    response = tr.get('http://ipecho.net/plain')
    print(response.text)  # not your IP address

    # TorRequest object also exposes the underlying Stem controller 
     # and Requests session objects for more flexibility.

    print(type(tr.ctrl))            # a stem.control.Controller object
    tr.ctrl.signal('CLEARDNSCACHE') # see Stem docs for the full AP

    tr.reset_identity()

    response = tr.get('http://ipecho.net/plain')
    print(response.text)  # another IP address

