const fs = require('fs');

const webToCpp = (src) => {

    const fileType = src.split('.')[1];

    console.log('Generating CPP file for: %s', fileType);

    // READ FILE...
    fs.readFile(src, 'utf8', (err, data) => {

        if (err) {
            return console.log(err);
        }

        // PROCESS FILE...
        data = data.replace(/"/g, '\\"');
        data = data.split('\n');

        if (!fs.existsSync(`../lib/${fileType}/`)){
            fs.mkdirSync(`../lib/${fileType}/`);
        }

        // CLEAR AND WRITE .h FILE...
        fs.writeFile(`../lib/${fileType}/${fileType}.h`, '', () => {

            const logger = fs.createWriteStream(`../lib/${fileType}/${fileType}.h`, {
                flags: 'a'
            });

            logger.write(`#ifndef ${fileType}_c\n`);
            logger.write(`#define ${fileType}_c\n`);
            logger.write(`class ${fileType.toUpperCase()}{\n`);
            logger.write('public:\n');
            logger.write(`${fileType.toUpperCase()}();\n`);
            logger.write(`const char *get${fileType.toUpperCase()}();\n`);
            logger.write('};\n');
            logger.write('#endif\n');

            logger.end();

        });

        // CLEAR AND WRITE .cpp FILE...
        fs.writeFile(`../lib/${fileType}/${fileType}.cpp`, '', () => {

            const logger = fs.createWriteStream(`../lib/${fileType}/${fileType}.cpp`, {
                flags: 'a'
            });

            logger.write(`#include <${fileType}.h>\n`);
            logger.write(`${fileType.toUpperCase()}::${fileType.toUpperCase()}() {}\n`);
            logger.write(`const char* ${fileType.toUpperCase()}::get${fileType.toUpperCase()}() {\n`);
            logger.write('    return ');
            logger.write(`R"rawliteral(${data.join('\n').replace(/\\/g, '')})rawliteral";`);
            logger.write('}\n');
            logger.end();

        });

    });

};

webToCpp('src/index.html');
