import gdown
import os
import pathlib


def download(id):
    gdown.download(id=id, output="data.7z")
    os.system("7z x data.7z -odata")
    pathlib.Path("data.7z").unlink()


download("1RzBH4LWSqHGdE-0SIpZZCluV6MLZ6b4f")
download("1ag21EFO0VzZlUccjTSVJ9tlT0uh7DbkS")
