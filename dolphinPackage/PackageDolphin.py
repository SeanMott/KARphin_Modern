#compresses the Dolphin into distribution Archives

import os
import subprocess

uncompressedPackageFP = "KARphin"
tarPackageFP = "KARphin.tar"
brotliPackageFP = "KARphin.br"

#pack into TAR
command = ['tar', '-cvf', tarPackageFP, uncompressedPackageFP]
subprocess.run(command)

#pack TAR into Brotli
command = ['brotli', '-q', '5', '-f', tarPackageFP, '-o', brotliPackageFP]
subprocess.run(command)