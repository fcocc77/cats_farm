app = app1


created_nodes = []


def createNode(name, group=app):
    nodes = {
        'blur': 'net.sf.cimg.CImgBlur',
        'text': 'net.fxarena.openfx.Text',
        'transform': 'net.sf.openfx.TransformPlugin',
        'merge': 'net.sf.openfx.MergePlugin'
    }

    node_name = app.createNode(nodes[name], -1, group).getScriptName()

    node = getattr(group, node_name)
    created_nodes.append(node)
    return node


def deleteNodes():
    for node in created_nodes:
        node.destroy()
    for node in created_nodes:
        node.destroy()

    del created_nodes[:]


def create_letter(letter, position, index):

    def expression(field, name, index, dimension=0, add=0):
        exp = 'value = thisGroup.' + name + \
            '.curve(frame - ' + str(index) + '*thisGroup.delay.get());'
        exp += 'ret = value + ' + str(add)

        field.setExpression(exp, False, dimension)

    # create text
    text = createNode('text', textAnimation)
    text.autoSize.set(True)
    text.text.set(letter)
    # Opacity expression
    for i in range(3):
        expression(text.color, 'opacity', index, i)
    # ------------------------

    # Blur
    blur = createNode('blur', textAnimation)
    blur.cropToFormat.set(False)
    blur.connectInput(0, text)
    expression(blur.size, 'blur_x', index, 0)
    expression(blur.size, 'blur_y', index, 1)
    # ------------------

    letter_width = text.getRegionOfDefinition(1, 1).x2

    transform = createNode('transform', textAnimation)

    # Transform expression
    expression(transform.translate, 'position_x', index, 0, position)
    expression(transform.translate, 'position_y', index, 1)
    expression(transform.rotate, 'rotate', index)
    expression(transform.scale, 'scale', index, 0)
    expression(transform.scale, 'scale', index, 1)
    # -----------------------

    transform.connectInput(0, blur)

    return [transform, letter_width]


def create_word():
    merge = createNode('merge', textAnimation)
    textAnimation.Output3.connectInput(0, merge)

    pos = 0
    word = textAnimation.text.get()
    for index, letter in enumerate(word):
        transform, letter_width = create_letter(letter.strip(), pos, index)

        # la entrada numero 2 pertenece a la maskara, asi que la omite
        if index >= 2:
            index += 1

        merge.connectInput(index, transform)
        pos += letter_width


# node de text animacion
def update_callback(thisParam, thisNode, thisGroup, app, userEdited):
    button_name = thisParam.getScriptName()
    if button_name == 'update':
        deleteNodes()
        create_word()


textAnimation = app.TextAnimation
textAnimation.onParamChanged.set('update_callback')
# -----------------------------
