import nmap

portScannerAsync = nmap.PortScannerAsync()

def callback_result(host, scan_result):
    print(host, scan_result)

portScannerAsync.scan(hosts='scanme.nmap.org', arguments='-p 21', callback=callback_result)
portScannerAsync.scan(hosts='scanme.nmap.org', arguments='-p 22', callback=callback_result)
portScannerAsync.scan(hosts='scanme.nmap.org', arguments='-p 23', callback=callback_result)
portScannerAsync.scan(hosts='scanme.nmap.org', arguments='-p 80', callback=callback_result)

while portScannerAsync.still_scanning():
    print("Scanning >>>")
    portScannerAsync.wait(5)
