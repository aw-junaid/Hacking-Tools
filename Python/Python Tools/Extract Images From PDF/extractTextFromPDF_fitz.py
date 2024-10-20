#!usr/bin/env python3

import fitz

pdf_document = "pdf/XMPSpecificationPart3.pdf"
doc = fitz.open(pdf_document)
print ("number of pages: %i" % doc.pageCount)

page_number= input("Enter page number:")

page = doc.loadPage(int(page_number)-1)
page_text = page.getText("text")
print(page_text)

