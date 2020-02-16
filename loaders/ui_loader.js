const loaderUtils = require('loader-utils')
const { exec } = require("child_process")


module.exports = function (source) {
    const optinos = loaderUtils.getOptions(this)
    const output = optinos.filename

    const ui_script = "loaders/ui_loader.py " + output
    const cmd = "python3 " + ui_script
    exec(cmd)

    return ""
}