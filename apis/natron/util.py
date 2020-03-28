import json
import os
from argparse import Namespace
import random
import string
from time import sleep


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


def hash_generator(keyLen):
    def base_str():
        return (string.ascii_letters + string.digits)

    keylist = [random.choice(base_str()) for i in range(keyLen)]
    return ("".join(keylist))


def debug(text):
    # a veces el print de natron no funciona cunando creamos nodos
    # asi que este debug, va agregando textos, y con el debug_show
    # muestra todos los textos juntos al final
    _file = '/tmp/natron.debug'
    os.system('echo "' + str(text) + '" >> ' + _file)


def debug_show():
    _file = '/tmp/natron.debug'
    if os.path.isfile(_file):
        print(fread(_file))
        os.remove(_file)
