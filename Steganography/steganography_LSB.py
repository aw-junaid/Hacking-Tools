#!/usr/bin/env python

from PIL import Image

def set_LSB(value, bit):
    if bit == '0':
        value = value & 254
    else:
        value = value | 1
    return value

def get_LSB(value):
    if value & 1 == 0:
        return '0'
    else:
        return '1'

def get_pixel_pairs(iterable):
    a = iter(iterable)
    return zip(a, a)


def extract_message(image):
 
    c_image = Image.open(image)
    pixel_list = list(c_image.getdata())
    message = ""

    for pix1, pix2 in get_pixel_pairs(pixel_list):
        message_byte = "0b"
        for p in pix1:
            message_byte += get_LSB(p)

        for p in pix2:
            message_byte += get_LSB(p)
            
        if message_byte == "0b00000000":
            break

        message += chr(int(message_byte,2))

    return message
    
def hide_message(image, message, outfile):

    message += chr(0)
    c_image = Image.open(image)
    c_image = c_image.convert('RGBA')
    out = Image.new(c_image.mode, c_image.size)
    width, height = c_image.size
    pixList = list(c_image.getdata())
    newArray = []
    
    for i in range(len(message)):
        charInt = ord(message[i])
        cb = str(bin(charInt))[2:].zfill(8)
        pix1 = pixList[i*2]
        pix2 = pixList[(i*2)+1]
        newpix1 = []
        newpix2 = []

        for j in range(0,4):
            newpix1.append(set_LSB(pix1[j], cb[j]))
            newpix2.append(set_LSB(pix2[j], cb[j+4]))

        newArray.append(tuple(newpix1))
        newArray.append(tuple(newpix2))

    newArray.extend(pixList[len(message)*2:])
    
    out.putdata(newArray)
    out.save(outfile)
    return outfile   

	
if __name__ == "__main__":
    print("Testing hide message in python_secrets.png with LSB ...")
    print(hide_message('python.png', 'Hidden message', 'python_secrets.png'))
    print("Hide test passed, testing message extraction ...")
    print(extract_message('python_secrets.png'))


