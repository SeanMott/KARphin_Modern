#compresses the Dolphin into distribution Archives

import os
import subprocess
from datetime import datetime

#writes data file
f = open("new_KARphinBuild.txt", 'w')
f.write(datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S'))

uncompressedPackage = "KARphin"
tarPackageName = uncompressedPackage + ".tar.gz"
brotliPackageName = tarPackageName + ".br"

#pack into TAR
command = ['tar', '-czvf', tarPackageName, uncompressedPackage]
subprocess.run(command)

#pack TAR into Brotli
command = ['brotli', '-q', '5', '-f', tarPackageName, '-o', brotliPackageName]
subprocess.run(command)

uncompressedPackage = "KARphinDev"
tarPackageName = uncompressedPackage + ".tar.gz"
brotliPackageName = tarPackageName + ".br"

#pack into TAR
command = ['tar', '-czvf', tarPackageName, uncompressedPackage]
subprocess.run(command)

#pack TAR into Brotli
command = ['brotli', '-q', '5', '-f', tarPackageName, '-o', brotliPackageName]
subprocess.run(command)