#!/usr/bin/env node

// Usage: node tablegen.js <name> <ident> <opcode_ext> <opcode> <byte_opcode> <pre>
// And you can use `>>` to append to a file, usually for the instructions.tbl file.

const process = require('process');

const name = process.argv[2];
const ident = process.argv[3];
const opcode_ext = process.argv[4];
const opcode = process.argv[5];
const byte_opcode = process.argv[6];
const pre = process.argv[7];

let out = ""
out = out.concat(`  ${name}  |`);
out = out.concat(` ${ident}${" ".repeat(9 - ident.length)}|`);
out = out.concat(` ${opcode_ext}${" ".repeat(17 - opcode_ext.length)}|`);
out = out.concat(` ${opcode}${" ".repeat(18 - opcode.length)}|`);
out = out.concat(` ${byte_opcode}${" ".repeat(18 - byte_opcode.length)}|`);
out = out.concat(` ${pre}\n`);

console.log(out);
process.exit(0);
