#!usr/bin/env python3

import fitz

pdf_document = fitz.open("pdf/XMPSpecificationPart3.pdf")  
for current_page in range(len(pdf_document)):  
    for image in pdf_document.getPageImageList(current_page):
        xref = image[0]
        pix = fitz.Pixmap(pdf_document, xref)
        if pix.n < 5:
            pix.writePNG("page%s-%s.png" % (current_page, xref))
        else:
            pix1 = fitz.Pixmap(fitz.csRGB, pix)
            pix1.writePNG("page%s-%s.png" % (current_page, xref))
