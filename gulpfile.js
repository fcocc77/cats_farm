var gulp = require('gulp');
var sass = require('gulp-sass');
var shell = require('gulp-shell')

// convierte los sass en un archivo css
gulp.task('sass', () => gulp.src("src/monitor/sass/*.sass")
    .pipe(sass({}))
    .pipe(shell("python3 " + py))
    .pipe(gulp.dest("src/monitor/sass"))
);
// -----------------------------------

// convierte las plantillas xml en ui
var ui_path = "src/monitor/ui";
var ui_script = "scripts/ui_templates.py";
var cmd = "python3 " + ui_script + " " + ui_path;
gulp.task('ui', shell.task(cmd))
// -----------------------------------


// task default para que este mirando los cambios que se hacen
exports.default = () => {
    gulp.watch("src/monitor/sass/*", gulp.series('sass'));
    gulp.watch("src/monitor/ui/*.xml", gulp.series('ui'));
};
// ----------------------------------/