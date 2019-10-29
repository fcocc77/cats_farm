import os


submit = "C:/vinarender/bin/win/submit.exe"


cmd = submit + " " + "-jobName test"

for n in range(100):
    os.system(cmd)
