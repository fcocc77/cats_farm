from django.conf import settings
from django.template.loader import get_template
import django
import sys

path = sys.argv[1]

settings.configure(TEMPLATES=[
    {
        'BACKEND': 'django.template.backends.django.DjangoTemplates',
        'DIRS': [path],
    }
])

django.setup()

template = get_template("main.xml")

f = open(path + "/main.ui", "w")
f.write(template.render())
f.close()
