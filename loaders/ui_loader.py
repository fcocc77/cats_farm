from django.conf import settings
from django.template.loader import get_template
import django
import sys

ui = "src/monitor/ui"
output =  sys.argv[1]
settings.configure(TEMPLATES=[
    {
        'BACKEND': 'django.template.backends.django.DjangoTemplates',
        'DIRS': ["./", ui],
    }
])

django.setup()

template = get_template(ui + "/main.xml")

f = open(output, "w")
f.write(template.render())
f.close()
