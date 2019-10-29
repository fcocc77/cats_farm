from django.conf import settings
from django.template.loader import get_template
import django

ui = "src/monitor/ui"
settings.configure(TEMPLATES=[
    {
        'BACKEND': 'django.template.backends.django.DjangoTemplates',
        'DIRS': ["./", ui],
    }
])

django.setup()

template = get_template(ui + "/main.xml")

f = open(ui + "/main.ui", "w")
f.write(template.render())
f.close()
