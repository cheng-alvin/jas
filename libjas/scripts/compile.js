const yaml = require("js-yaml"); // Ensure `js-yaml` is installed and accessible
const fs = require("fs")

let instructions = [];
let instructionNames = [];

for (let i = 2; i < process.argv.length; i++) {
  const fileContents = fs.readFileSync(process.argv[i], {encoding : 'utf-8'});
  const data = yaml.load(fileContents);

  for (let j = 0; j < data.instructions.length; j++) {
    instructionNames.push(Object.keys(data.instructions[j])[0]);
    instructions.push(data.instructions[j]);
  }
}

// TODO refactor since this is a mess
function produceOperands(instruction) {
  for (let i = 0; i < instruction.variants.length; i++) {
    for (let j = 0; j < instruction.variants[i].operands.length; j++) {
      const operand = instruction.variants[i].operands[j];
      const match = operand.type.match(/^r\/m(8|16|32|64)$/);

      if (match) {
        let modifiedVariant = structuredClone(instruction.variants[i]);
        instruction.variants[i].operands[j].type = `m${match[1]}`;

        modifiedVariant.operands[j].type = `r${match[1]}`;
        instruction.variants.push(modifiedVariant)
      }
    }
  }

  return instruction
}

instructions.forEach((instr) => {
  produceOperands(instr);
});

function handleOperands(operands) {
  let res = "";

  for (let i = 0; i < operands.length; i++) {
    const type = operands[i].type.toUpperCase();
    const encoder = operands[i].encoder.toUpperCase().replace("/", "");

    res += `{ .type = OP_${type}, .encoder = ENC_${encoder} }, `
  }

  return `{${res}${'{ 0 }, '.repeat(4 - operands.length)}}`
}

// clang-format off
let output =
    "#include <stdbool.h> \n" + "#include \"instruction.h\" \n " +
    "#include \"operand.h\" \n " + "#include \"encoder.h\" \n\n ";
// clang-format on

output += `struct instr_encode_table instr_table[] = {`

for (let k = 0; k < instructions.length; k++) {
  const variants = instructions[k].variants;

  for (let l = 0; l < variants.length; l++)
    output += handleVariant(variants[l]);
}
output += "};"

fs.writeFileSync("tabs.c", output);

function handleVariant(variant) {
  let res = "";

  res += `.opcode = { `

  variant.opcode.forEach(e => { res += `0x${e.toString(16)}, `; });

  res += `${"0x00, ".repeat(3 - variant.opcode.length)}}, `
  res += `.operand_descriptors = ${handleOperands(variant.operands)}, `

  res += `.long_mode = ${variant.compatibility.long ? "true" : "false"}, `
  res += `.leg_mode = ${variant.compatibility.legacy ? "true" : "false"}, `

  res += `.opcode_size = ${variant.opcode.length}, `
  res += `.operand_count = ${variant.operands.length},`

  return `{ ${res} },`;
}