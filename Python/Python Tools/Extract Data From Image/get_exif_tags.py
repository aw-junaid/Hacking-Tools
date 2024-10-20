from PIL import Image
from PIL.ExifTags import TAGS

for (i,j) in Image.open('images/image.jpg')._getexif().items():
        print('%s = %s' % (TAGS.get(i), j))
