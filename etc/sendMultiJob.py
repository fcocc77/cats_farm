import os


submit = "C:/cats_farm/bin/win/submit.exe"


cmd = submit + " " + "-jobName test"

for n in range(100):
    os.system(cmd)
