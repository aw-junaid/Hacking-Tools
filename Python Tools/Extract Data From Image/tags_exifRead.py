import exifread
file = open('images/image.jpg', 'rb')
tags = exifread.process_file(file)

for tag in tags.keys():
    print("Key: %s, value %s" % (tag, tags[tag]))
