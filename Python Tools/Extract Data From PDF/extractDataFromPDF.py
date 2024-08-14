#!usr/bin/env python3

from PyPDF2 import PdfFileReader, PdfFileWriter
import os, time, os.path, stat
from PyPDF2.generic import NameObject, createStringObject

def get_metadata():
	for dirpath, dirnames, files in os.walk("pdf"):
		for data in files:
			ext = data.lower().rsplit('.', 1)[-1]
			if ext in ['pdf']:
				print("[--- Metadata : " + "%s ", (dirpath+os.path.sep+data))
				print("------------------------------------------------------------------------------------")
				pdfReader = PdfFileReader(open(dirpath+os.path.sep+data, 'rb'))
				info = pdfReader.getDocumentInfo()

				for metaItem in info:

					print ('[+] ' + metaItem.strip( '/' ) + ': ' + info[metaItem])
					
				pages = pdfReader.getNumPages()
				print ('[+] Pages:', pages)
				
				layout = pdfReader.getPageLayout()
				print ('[+] Layout: ' + str(layout))

				xmpinfo = pdfReader.getXmpMetadata()

				if hasattr(xmpinfo,'dc_contributor'): print ('[+] Contributor:' , xmpinfo.dc_contributor)
				if hasattr(xmpinfo,'dc_identifier'): print ( '[+] Identifier:', xmpinfo.dc_identifier)
				if hasattr(xmpinfo,'dc_date'): print ('[+] Date:', xmpinfo.dc_date)
				if hasattr(xmpinfo,'dc_source'): print ('[+] Source:', xmpinfo.dc_source)
				if hasattr(xmpinfo,'dc_subject'): print ('[+] Subject:' , xmpinfo.dc_subject)
				if hasattr(xmpinfo,'xmp_modifyDate'): print ('[+] ModifyDate:', xmpinfo.xmp_modifyDate)
				if hasattr(xmpinfo,'xmp_metadataDate'): print ('[+] MetadataDate:', xmpinfo.xmp_metadataDate)
				if hasattr(xmpinfo,'xmpmm_documentId'): print ('[+] DocumentId:' , xmpinfo.xmpmm_documentId)
				if hasattr(xmpinfo,'xmpmm_instanceId'): print ('[+] InstanceId:', xmpinfo.xmpmm_instanceId)
				if hasattr(xmpinfo,'pdf_keywords'): print ('[+] PDF-Keywords:', xmpinfo.pdf_keywords)
				if hasattr(xmpinfo,'pdf_pdfversion'): print ('[+] PDF-Version:', xmpinfo.pdf_pdfversion)

				if hasattr(xmpinfo,'dc_publisher'):
					for published in xmpinfo.dc_publisher:
						if publisher:
							print ("[+] Publisher:\t" + publisher) 

			fsize = os.stat((dirpath+os.path.sep+data))
			print ('[+] Size:', fsize[6], 'bytes \n\n')

if __name__ == "__main__":
    get_metadata()
