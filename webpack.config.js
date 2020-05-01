const MiniCssExtractPlugin = require("mini-css-extract-plugin")


module.exports = {
    entry: {
        monitor: "./src/monitor/sass/main.sass"
    },
    output: {
        path: __dirname + "/",
        filename: "temp/[name].js"
    },
    plugins: [
        new MiniCssExtractPlugin({
            filename: "src/monitor/sass/main.css"
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