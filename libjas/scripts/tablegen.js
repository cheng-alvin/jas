#!/usr/bin/env node

// Usage: node tablegen.js <name> <ident> <opcode_ext> <opcode> <byte_opcode> <pre>
// And you can use `>>` to append to a file, usually for the instructions.tbl file.

// Note: if argv[3] zo, just type n an opcode and everything will fall into place automatically.

const process = require('process');

let name = process.argv[2];
let ident = process.argv[3];
let opcode_ext = process.argv[4];
let opcode = process.argv[5];
let byte_opcode = process.argv[6];

if (process.argv[3] == 'zo') {
  opcode = process.argv[4];
  byte_opcode = "-";
  opcode_ext = "-";
}

let out = ""
let len = 5 - name.length
if (len < 0) len = 1;

out = out.concat(`  ${name}${" ".repeat(len)}|`);
out = out.concat(` ${ident}${" ".repeat(9 - ident.length)}|`);
out = out.concat(` ${opcode_ext}${" ".repeat(17 - opcode_ext.length)}|`);
out = out.concat(` ${opcode}${" ".repeat(18 - opcode.length)}|`);
out = out.concat(` ${byte_opcode}${" ".repeat(18 - byte_opcode.length)}|`);

console.log(out);
process.exit(0);
