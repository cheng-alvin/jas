/**
 * Script to compile YAML instruction definitions into a C structure
 * representation, allowing for a more manageable way to define
 * instruction encodings outside of C code. The general structure
 * of the YAML files should follow the following format:
 *
 * instructions:
 *  - <name>:           # Name of instruction
 *     variants:
 *      - opcode: [...] # List of opcode bytes (max 3)
 *       operands:      # List of operands (max 4)
 *        - {
 *          type: <type>,         # Type of operand to be encoded
 *          encoder: <encoder>,   # Encoder option for operand
 *         }
 *
 *      compatibility:  # Describes compatibility modes
 *       long: <bool>   # 64-bit mode
 *       legacy: <bool> # 16/32-bit mode
 *
 * Such YAML files can then be passed as arguments to this script,
 * in form of a file reference, in the OS. Instructions added to the
 * list of arguments will be compiled together into a single compiled
 * C structure representation, as a `instructions.inc` file in the
 * current working directory.
 *
 * @note `js-yaml` package must be installed and accessible to allow
 * yaml parsing functionality to be used.
 *
 * Encoder options are to be filled in each operand definition's
 * `encoder` field. The value of the `encoder` field is to either
 * map to a predefined encoder enum, or be an integer value from
 * 0 to 7 inclusive.
 *
 * Shorthand syntax:
 * Operand types can also be defined in a general shorthand form for
 * convenience. For example, `r/m16` will be generated and expanded
 * into both `r16` and `m16` operand types, with the encoder field
 *
 * Operands can also be defined lacking the operand size trailing the
 * type such as simply `r`, or `imm`, expanding into all respective
 * sizes of the type provided.
 *
 * Shorthand formats for encoder options are as follows:
 * - /[0-7]: Maps to the integer value for the `reg` field.
 * - r/m: Corresponds to the `ENC_RM` encoder, with `/` removed.
 *
 * All `/`'s will be removed from the final output, and all char-
 * acters in the encoder field will be converted to uppercase to
 * match the format as expected by the enum.
 *
 * Licensing details appear below:
 *
 * MIT License
 * Copyright (c) 2023-2026 Alvin Cheng <eventide1029@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @see `LICENSE`
 */

const yaml = require("js-yaml"); // Ensure `js-yaml` is installed and accessible
const fs = require("fs");

let instructions = [];
let instructionNames = [];

for (let i = 2; i < process.argv.length; i++) {
  const fileContents = fs.readFileSync(process.argv[i], { encoding: "utf-8" });
  const data = yaml.load(fileContents);

  for (let j = 0; j < data.instructions.length; j++) {
    instructionNames.push(Object.keys(data.instructions[j])[0]);
    instructions.push(data.instructions[j]);
  }
}

function produceOperands(instruction) {
  for (let i = 0; i < instruction.variants.length; i++) {
    for (let j = 0; j < instruction.variants[i].operands.length; j++) {
      const operand = instruction.variants[i].operands[j];

      const normalType = operand.type.match(/^[^0-9]+(8|16|32|64)$/);
      if (!normalType) {
        const type = operand.type;

        const sizes = [8, 16, 32, 64];
        instruction.variants[i].operands[j].type = `${type}8`;

        for (const size of sizes.slice(1)) {
          const variant = structuredClone(instruction.variants[i]);
          variant.operands[j].type = `${type}${size.toString()}`;
          instruction.variants.push(variant);
        }
      }

      const match = operand.type.match(/^r\/m(8|16|32|64)$/);

      if (match) {
        let modifiedVariant = structuredClone(instruction.variants[i]);
        instruction.variants[i].operands[j].type = `m${match[1]}`;

        modifiedVariant.operands[j].type = `r${match[1]}`;
        instruction.variants.push(modifiedVariant);
      }
    }
  }

  return instruction;
}

instructions.forEach((instr) => {
  produceOperands(instr);
});

function literalValue(operand) {
  if (operand.encoder != "literal") return "{ 0 }";

  const operandType = operand.type.toUpperCase();
  if (operandType.includes("IMM")) return `{ .imm = ${operand.value} }`;
  if (operandType.includes("R"))
    return `{ .reg = REG_${operand.value.toUpperCase()} }`;
}

function handleOperands(operands) {
  let res = "";

  for (let i = 0; i < operands.length; i++) {
    const type = operands[i].type.toUpperCase();
    let encoder = `ENC_${operands[i].encoder.toUpperCase()}`;

    const match = operands[i].encoder.match(/^\/([0-7])$/);
    if (match) encoder = `(enum enc_ident)${match[1]}`;
    encoder = encoder.replace("/", "");

    res += `{ .type = OP_${type}, .encoder = ${encoder}, .literal = ${literalValue(operands[i])}, }, `;
  }

  return `{${res}${"{ 0 }, ".repeat(4 - operands.length)}}`;
}

let output = "";

let includes =
  "#include <stdbool.h> \n" +
  '#include "instruction.h" \n' +
  '#include "operand.h" \n' +
  '#include "encoder.h" \n';

let headerComment = "// Auto-generated file. Do not edit directly. \n\n";
let guard = "#ifdef INSTR_ENCODER_TAB \n";

output += headerComment + guard + includes;
output += `struct instr_encode_table *instr_table[] = {`;

for (let k = 0; k < instructions.length; k++) {
  output += `(struct instr_encode_table []){`;
  const variants = instructions[k].variants;

  for (let l = 0; l < variants.length; l++)
    output += handleVariant(variants[l]);

  output += " (instr_encode_table_t){0},},";
}
let trailingGuard = "}; \n#undef INSTR_ENCODER_TAB \n#endif";
output += trailingGuard;

output += "\n\n"; // -----------------------------------

let instrEnumBoiler = "#ifdef INSTR_ENUM \n";
output += instrEnumBoiler;

output += generateEnumNames(instructionNames);
output += generateInstructionNames(instructionNames);

output += "};\n";
output += "#undef INSTR_ENUM\n#endif";

// ---

console.log(output);

function handleVariant(variant) {
  let res = "";

  res += `.opcode = { `;

  variant.opcode.forEach((e) => {
    res += `0x${e.toString(16)}, `;
  });

  res += `${"0x00, ".repeat(3 - variant.opcode.length)}}, `;
  res += `.operand_descriptors = ${handleOperands(variant.operands)}, `;

  res += `.long_mode = ${variant.compatibility.long ? "true" : "false"}, `;
  res += `.leg_mode = ${variant.compatibility.legacy ? "true" : "false"}, `;

  res += `.opcode_size = ${variant.opcode.length}, `;
  res += `.operand_count = ${variant.operands.length},`;

  return `{ ${res} },`;
}

function generateEnumNames(instructions) {
  let instrEnumEntries = "enum instructions {\n";

  let instructionNames = ["null = 0", ...instructions];
  for (let m = 0; m < instructionNames.length; m++) {
    instrEnumEntries += `  INSTR_${instructionNames[m].toUpperCase()},\n`;
  }
  instrEnumEntries += "};\n";

  return instrEnumEntries;
}

function generateInstructionNames(names) {
  let instrNames = "char *instr_tab_names[] = {\n";
  for (let n = 1; n < names.length; n++) {
    instrNames += `  "${names[n]}",`;
  }

  return instrNames;
}
