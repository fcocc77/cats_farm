const { exec } = require("child_process")
const fs = require('fs')

const fread = path => {
    return fs.readFileSync(path, 'utf8')
}

const fwrite = (path, data) => {
    fs.writeFile(path, data, () => { });
}

const print = data =>
    console.log(data)

const nospace = string =>
    string.replace(/ /g, '')


const ui = "./test.ui"
const jade_file = './test.jade'
const jade = fread(jade_file)

let output = ""

const lines = jade.split('\n')
const tags = []

const widgets = ['widget', 'casa']

let tag
lines.map(line => {
    if (line.charAt(0) != ' ') {
        tag = nospace(line)
        tags.push({
            'name': tag,
            'props': []
        })
    }
    else {
        const _tag = tags[tags.length - 1]
        const [key, value] = line.split('=')

        _tag.props.push([
            nospace(key),
            value
        ])
    }
})

tags.map(tag => {
    let props = ''

    tag.props.map(prop => {
        const [key, value] = prop
        props += key + '=' + value + ' '
    })

    output += '<' + tag.name + ' ' + props + ' >'
    // children ...

    output += '\n</' + tag.name + '>'
})

fwrite(ui, output)