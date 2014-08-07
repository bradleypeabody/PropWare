#!/usr/bin/python
# @file    propwareUtils.py
# @author  David Zemon
#
# Created with: PyCharm Community Edition

"""
@description:
"""
import os
import re
import shutil
from sys import version
import subprocess
import zipfile

__author__ = 'david'


DOWNLOADS_DIRECTORY = ".external_downloads" + os.sep
MEMORY_MODELS = ["cog", "cmm", "lmm", "xmmc", "xmm-single", "xmm-split"]


def which(program):
    def is_exe(filePath):
        if os.path.isfile(filePath) and os.access(filePath, os.X_OK):
            return True
        elif "nt" == os.name and ".exe" != filePath[-4:]:
            return is_exe(filePath + ".exe")
        else:
            return False

    directory, fileName = os.path.split(program)
    if directory:
        if is_exe(program):
            return program
    else:
        for path in os.environ["PATH"].split(os.pathsep):
            path = path.strip('"')
            exe_file = os.path.join(path, program)
            if is_exe(exe_file):
                return exe_file

    # If we haven't returned anything yet, that means the program doesn't exist
    return None


def checkProperWorkingDirectory():
    if "createBinaryDistr.py" not in os.listdir('.'):
        raise IncorrectStartingDirectoryException()


def isPropWareRoot(directory):
    assert (isinstance(directory, str))
    assert (os.path.isdir(directory))

    return "PropWare.dox" in os.listdir(directory)


def isPython3():
    return '3' == version[0]


def initDownloadsFolder(propwareRoot):
    assert (isPropWareRoot(propwareRoot))

    fullPath = os.path.abspath(propwareRoot) + str(os.sep) + DOWNLOADS_DIRECTORY

    # Create the folder if it doesn't exist
    if not os.path.exists(fullPath):
        os.mkdir(fullPath)

    # If on Windows, set the hidden attribute
    if "nt" == os.name:
        winDirName = os.path.normpath(fullPath)
        import ctypes

        FILE_ATTRIBUTE_HIDDEN = 0x02

        ret = ctypes.windll.kernel32.SetFileAttributesW(winDirName, FILE_ATTRIBUTE_HIDDEN)
        if 0 == ret:  # return code of zero indicates failure, raise Windows error
            raise ctypes.WinError()


def downloadFile(src, dstDir):
    fileName = src.split('/')[-1]
    dst = os.path.abspath(dstDir) + str(os.sep) + fileName

    # If the file already exists, don't re-download it
    if os.path.exists(dst):
        return dst, None

    if isPython3():
        import urllib.request

        return urllib.request.urlretrieve(src, dst)
    else:
        # noinspection PyUnresolvedReferences
        import urllib2

        u = urllib2.urlopen(src)
        with open(dst, 'wb') as f:
            meta = u.info()
            file_size = int(meta.getheaders("Content-Length")[0])
            print("Downloading: %s Bytes: %s" % (fileName, file_size))

            fileSizeDl = 0
            blockSize = 8192
            while True:
                buffer = u.read(blockSize)
                if not buffer:
                    break

                fileSizeDl += len(buffer)
                f.write(buffer)
                status = r"%10d  [%3.2f%%]" % (fileSizeDl, fileSizeDl * 100. / file_size)
                status += chr(8) * (len(status) + 1)
                print(status)

            return dst, meta


def copytree(src, dst):
    src = os.path.normpath(src)
    dst = os.path.normpath(dst)

    for item in os.listdir(src):
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        if os.path.isdir(s):
            shutil.copytree(s, d)
        else:
            shutil.copy2(s, d)


def isAsmFile(f):
    try:
        extension = f.split(".")[1]
    except IndexError:
        return False

    return extension in ["S", "s"]


def isSourceFile(f):
    assert (isinstance(f, str))
    return re.match('.*(\.c|\.cpp|\.cxx|\.cc|\.s|\.dat|\.cogc|\.ecogc|\.spin)$', f, re.I)


def isHeaderFile(f):
    assert (isinstance(f, str))
    return re.match('.*(\.h|\.hpp)$', f, re.I)


def isSourceOrHeaderFile(f):
    return isSourceFile(f) or isHeaderFile(f)


def testPropGCC():
    """
    Determine if PropGCC is installed and in the users PATH
    """
    subprocess.check_output(["propeller-elf-gcc", "--version"])


def extractZip(zipFileName, destination):
    zipFile = zipfile.ZipFile(zipFileName, mode='r')
    zipFile.extractall(destination)


class IncorrectStartingDirectoryException(Exception):
    def __init__(self):
        pass

    def __str__(self):
        return "Must be executed from within <propware root>/util"
