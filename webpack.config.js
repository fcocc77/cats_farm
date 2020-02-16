const MiniCssExtractPlugin = require("mini-css-extract-plugin")


module.exports = {
    entry: {
        monitor: "./src/monitor/sass/main.sass",
        ui: "./src/monitor/ui/main.xml",
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
    resolveLoader: {
        modules: ['node_modules', __dirname + '/loaders']
    },
    module: {
        rules: [
            {
                test: /main.sass/,
                use: [
                    { loader: MiniCssExtractPlugin.loader },
                    "css-loader",
                    "sass-loader"
                ]
            },
            {
                test: /\.xml/,
                use: {
                    loader: 'ui_loader',
                    options: {
                        filename: "src/monitor/ui/main.ui"
                    }
                }
            }
        ]
    }
}