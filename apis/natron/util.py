import json
import os
from argparse import Namespace


def fwrite(file, date):
    if not date:
        date = "None"
    if date == "void":
        date = ""

    try:
        f = open(file, "w")
        f.write(date)
        f.close()
    except:
        None


def fread(file):
    reading = "0"
    for n in range(100):

        if os.path.isfile(file):
            f = open(file, "r")
            reading = str(f.read().strip())
            f.close()

            if reading:
                break

        else:
            break

        sleep(0.07)

    return reading


def jread(file):
    return json.loads(
        fread(file),
        object_hook=lambda d: Namespace(**d)
    )


def jwrite(file, data):
    info = json.dumps(
        vars(data),
        sort_keys=True,
        indent=4,
        default=lambda x: x.__dict__
    )

    fwrite(file, info)
