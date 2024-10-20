#!usr/bin/env python3

import PyPDF2

pdfFile = open("pdf/XMPSpecificationPart3.pdf","rb")
pdfReader = PyPDF2.PdfFileReader(pdfFile)

page_number= input("Enter page number:")
pageObj = pdfReader.getPage(int(page_number)-1)
text_pdf = str(pageObj.extractText())

print(text_pdf)
