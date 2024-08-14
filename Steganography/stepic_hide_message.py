from PIL import Image
import stepic

image = Image.open("python.png")
image2 = stepic.encode(image, 'This is the hidden text'.encode("utf8"))
image2.save('python_secrets.png','PNG')
image2 = Image.open('python_secrets.png')
data = stepic.decode(image2) 
print("Decoded data: " + data)
