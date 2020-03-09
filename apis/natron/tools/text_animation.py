app = app1


text_group = app.TextAnimation


def create_letter(letter, position):

    text_name = app.createNode(
        "net.fxarena.openfx.Text", -1, text_group).getScriptName()
    transform_name = app.createNode(
        "net.sf.openfx.TransformPlugin", -1, text_group).getScriptName()

    text = getattr(text_group, text_name)
    text.autoSize.set(True)
    text.text.set(letter)
    letter_width = text.getRegionOfDefinition(1, 1).x2

    transform = getattr(text_group, transform_name)
    transform.translate.set(position, 0)

    transform.connectInput(0, text)

    return [transform, letter_width]


merge = app.createNode('net.sf.openfx.MergePlugin', -1, text_group)
text_group.Output1.connectInput(0, merge)


word = 'Francisco'


pos = 0

for index, letter in enumerate(word):
    transform, letter_width = create_letter(letter.strip(), pos)

    # la entrada numero 2 pertenece a la maskara, asi que la omite
    if index >= 2:
        index += 1

    merge.connectInput(index, transform)
    pos += letter_width
