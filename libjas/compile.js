#!/usr/bin/env node

const fs = require('fs');
const file = process.argv[2];

let valid_data = []

fs.readFileSync(file, 'utf8').split('\n').forEach((line) => {
  let valid_lines = [];
  if (line[0] === '#') return;
  if (line === '') return;

  valid_lines.push(line);

  valid_lines.forEach((line) => {
    line = line.replaceAll(' ', '');
    let parts = line.split('|');
    valid_data.push(parts);
  })
});

let groups = {};

valid_data.forEach((line) => {
  if (groups[line[0]] == undefined) groups[line[0]] = [];
  groups[line[0]].push(line);
});

function addQuotes(str) { return `{${str}}`; };
function countComma(input) { return (input.match(/,/g) || []).length; }

let output = "";
for (const [key, instr] of Object.entries(groups)) {
  output = output.concat(`instr_encode_table_t ${key.toString()}[] = {\n`)
  instr.forEach((group) => {
    const ident = `ENC_${group[1].toUpperCase()}`;
    if (group[2] === "-") group[2] = "NULL";
    const ext = group[2];
    const opcode = addQuotes(group[3]);
    const opcode_size = countComma(group[3]) + 1;
    const byte_opcode = addQuotes(group[4]);
    const byte_opcode_size = countComma(group[4]) + 1;
    const pre = `&${group[5]}`;

    // Putting it all together
    output = output.concat(
        `  {${ident}, ${ext}, ${opcode}, ${byte_opcode}, ${opcode_size}, ${pre}, ${byte_opcode_size}},\n`);
  });

  output = output.concat(`  INSTR_TAB_NULL,\n};\n`);
}

var prepend = `#include "instr_encode.h" \n#include "pre.c"\n\n`;
fs.writeFileSync('tabs.c', prepend + output);
process.exit(0);