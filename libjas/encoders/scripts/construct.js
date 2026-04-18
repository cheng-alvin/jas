/**
 * Script allowing instruction encoder reference tables to be rep-
 * resented based off a .csv file. Allows for the rapid addition of
 * new instructions without needing to manually write out a YAML
 * markup file manually.
 *
 * Ultimately, the YAML files can be used for specific modifications
 * and granular control of the instruction encodings once these tables
 * has been generated.
 *
 * @usage node construct.js <input file>
 *
 * A file for the CSV file header input should appear as follows: The
 * instruction's name is not included as it is assumed to be represented
 * as the name of the file itself, excluding the .csv file extension.
 *
 *      "opcode","long","legacy","type-x","encoder-x"
 *
 * (where `x` is the operand index starting from 1)
 *
 * The construction of the CSV file can be done by exporting any type
 * of spreadsheet file via any spreadsheet software that supports the
 * export of the file through CSV, as long as the headers remain con-
 * sistent with the above format.
 *
 * You can use this https://shorturl.at/DW73H Google Drive link as a
 * template for the CSV file. You may download or make a copy of the
 * spreadsheet for editing on your own before conversion to the CSV
 * file format.
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

const yaml = require("js-yaml");
const fs = require("fs");
const csv = require("csv-parse");

const fileName = process.argv[2];
const fileData = fs.readFileSync(fileName, "utf8");

const parserInstance = csv.parse(fileData, {
  columns: true,
  skip_empty_lines: true,
});

parserInstance.on("error", (err) => {
  console.error(err.message);
  process.exit(1);
});

let instructionName = "";
let instructions = [];

parserInstance.on("readable", () => {
  let record;
  while ((record = parserInstance.read()) !== null) {
    instructionName = fileName.split("/").pop().split(".")[0];
    instructions.push(record);
  }
});

parserInstance.on("end", constructYamlFile);

function constructYamlFile() {
  let template = { instructions: [{ [instructionName]: null, variants: [] }] };

  for (let i = 0; i < instructions.length; i++) {
    const current = instructions[i];
    let variant = {};

    variant.opcode = `[${current.opcode.toString()}]`;

    variant.operands = [];
    let operandCount = 1;
    while (
      current[`type-${operandCount}`] &&
      current[`encoder-${operandCount}`]
    ) {
      let operand = {
        type: current[`type-${operandCount}`],
        encoder: current[`encoder-${operandCount}`],
      };
      const match = current[`encoder-${operandCount}`].match(/^\{.*\}$/);

      if (match) {
        operand.encoder = "literal";
        operand.value = match[0].slice(1, -1);
      }

      variant.operands.push(operand);
      operandCount++;
    }

    variant.compatibility = {
      long: current.long.toLowerCase() === "true",
      legacy: current.legacy.toLowerCase() === "true",
    };

    template.instructions[0].variants.push(variant);
  }
  console.log(
    yaml
      .dump(template, {
        flowLevel: 6,
        noRefs: true,
        styles: { "!!null": "empty", "!!int": "hexadecimal" },
      })
      .replaceAll("'", ""),
  );
}
