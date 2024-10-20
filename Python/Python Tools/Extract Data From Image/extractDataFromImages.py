from PIL.ExifTags import TAGS, GPSTAGS
from PIL import Image
import os

def get_exif_metadata(image_path):
    exifData = {}
    image = Image.open(image_path)
    if hasattr(image, '_getexif'):
        exifinfo = image._getexif()
        if exifinfo is not None:
            for tag, value in exifinfo.items():
                decoded = TAGS.get(tag, tag)
                exifData[decoded] = value
    decode_gps_info(exifData)
    return exifData

def decode_gps_info(exif):
    gpsinfo = {}
    if 'GPSInfo' in exif:
        Nsec = exif['GPSInfo'][2][2]
        Nmin = exif['GPSInfo'][2][1]
        Ndeg = exif['GPSInfo'][2][0]
        Wsec = exif['GPSInfo'][4][2]
        Wmin = exif['GPSInfo'][4][1]
        Wdeg = exif['GPSInfo'][4][0]
        if exif['GPSInfo'][1] == 'N':
            Nmult = 1
        else:
            Nmult = -1
        if exif['GPSInfo'][1] == 'E':
            Wmult = 1
        else:
            Wmult = -1
        latitude = Nmult * (Ndeg + (Nmin + Nsec/60.0)/60.0)
        longitude = Wmult * (Wdeg + (Wmin + Wsec/60.0)/60.0)
        exif['GPSInfo'] = {"Latitue" : latitude, "Longitude" : longitude}
   
def printMetadata():
    for dirpath, dirnames, files in os.walk("images"):
        for name in files:
            print("[+] Metadata for file: %s " %(dirpath+os.path.sep+name))
            try:
                exifData = {}
                exif = get_exif_metadata(dirpath+os.path.sep+name)
                for metadata in exif:
                    print("Metadata: %s - Value: %s " %(metadata, exif[metadata]))
                print("\n")
            except:
                import sys, traceback
                traceback.print_exc(file=sys.stdout)

if __name__ == "__main__":
    printMetadata()

