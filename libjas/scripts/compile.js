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

let namesList = [];
let output = "";

for (const [key, instr] of Object.entries(groups)) {
  output = output.concat(`instr_encode_table_t ${key.toString()}[] = {\n`)
  instr.forEach((group) => {
    if (group[1] == "-") group[1] = "ign";
    const ident = `ENC_${group[1].toUpperCase()}`;
    if (group[2] === "-") group[2] = "NULL";
    const ext = group[2];
    const opcode = addQuotes(group[3] == "-" ? "NULL" : group[3]);
    const opcode_size = countComma(group[3]) + 1;
    const byte_opcode = addQuotes(group[4] == "-" ? "NULL" : group[4]);
    let byte_opcode_size = countComma(group[4]) + 1;
    if (group[4] == "-") byte_opcode_size = 0;

    let pre;
    if (group[5] == "-")
      pre = "NULL";
    else
      pre = `&${group[5]}`;

    // Putting it all together
    output = output.concat(
        `  {${ident}, ${ext}, ${opcode}, ${byte_opcode}, ${opcode_size}, ${pre}, ${byte_opcode_size}},\n`);
  });

  output = output.concat(`  INSTR_TAB_NULL,\n};\n`);
  namesList.push(`"${key.toString().toLowerCase().replaceAll("_", "")}"`);
}

const names = `char *instr_tab_names[] = {${namesList.join(', ')}};`
const prepend = `#include "instruction.h" \n#include "pre.c"\n\n`;
fs.writeFileSync('tabs.c', prepend + output + names);

process.exit(0);