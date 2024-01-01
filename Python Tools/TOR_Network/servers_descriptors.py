from stem.descriptor.remote import DescriptorDownloader

downloader = DescriptorDownloader()

descriptors = downloader.get_server_descriptors().run()

for descriptor in descriptors:
    print('Descriptor', str(descriptor))
    print('Certificate', descriptor.certificate)
    print('ONion key', descriptor.onion_key)
    print('Signing key', descriptor.signing_key)
    print('Signature', descriptor.signature)

