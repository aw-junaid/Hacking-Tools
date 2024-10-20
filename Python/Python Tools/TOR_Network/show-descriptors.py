from stem.descriptor.remote import DescriptorDownloader

downloader = DescriptorDownloader()
descriptors = downloader.get_consensus().run()

for descriptor in descriptors:
    print('Nickname:',descriptor.nickname)
    print('Fingerprint:',descriptor.fingerprint)
    print('Address:',descriptor.address)
    print('Bandwidth:',descriptor.bandwidth)

