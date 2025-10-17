const yaml = require("js-yaml");
const fs = require("fs")

let instructions = [];

for (let i = 1; i < process.argv.length; i++){
  const fileContents = fs.readFileSync(process.argv[i], {encoding: 'utf-8'});
  instructions.push(yaml.load(fileContens).instructions);
}

