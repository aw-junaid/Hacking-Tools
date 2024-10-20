from pybinaryedge import BinaryEdge
import os

key= os.environ['BINARYEDGE_API_KEY']

binaryEdge = BinaryEdge(key)

search_domain = 'www.python.org'

results = binaryEdge.host_search(search_domain)

for ip in results['events']:
    print("%s" %(ip['target']['ip']))
