const MiniCssExtractPlugin = require("mini-css-extract-plugin")


module.exports = {
    entry: {
        monitor: "./stylesheet/main.sass"
    },
    output: {
        path: __dirname + "/",
        filename: "temp/[name].js"
    },
    plugins: [
        new MiniCssExtractPlugin({
            filename: "./resources/css/style.css"
        }),
    ],
    module: {
        rules: [
            {
                test: /main.sass/,
                use: [
                    { loader: MiniCssExtractPlugin.loader },
                    "css-loader",
                    "sass-loader"
                ]
            }
        ]
    }
}