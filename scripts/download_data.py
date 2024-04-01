import gdown
import os
import pathlib

gdown.download(id="1RzBH4LWSqHGdE-0SIpZZCluV6MLZ6b4f")
os.system("7z x star_observation_scheduling.7z -odata")
pathlib.Path("star_observation_scheduling.7z").unlink()

gdown.download(id="1ag21EFO0VzZlUccjTSVJ9tlT0uh7DbkS")
os.system("7z x flexible_star_observation_scheduling.7z -odata")
pathlib.Path("flexible_star_observation_scheduling.7z").unlink()
