#! /bin/node

// Defined constants for ignore/resume markers as shown:
const ignoreMarker = '<!-- @mdformat pause -->';
const resumeMarker = '<!-- @mdformat resume -->';

const options = '--wrap 80';  // Add additional supported options here.

/**
 * A simple wrapper around mdformat to ignore certain regions marked by
 * ignore/resume markers, as defined above. Options to mdformat may be
 * added in the `options` constant.
 *
 * @usage $ node mdformatwrapper.js <input files>
 *
 * Arguments after the script name are treated as file paths that are
 * processed by `mdformat` with ignored regions preserved by this wrapper.
 *
 * @license MIT
 * Copyright (c) 2026 Alvin Cheng <eventide1029@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

const process = require('process');
const {execSync} = require('child_process');
const fs = require('fs');

if (process.argv.length < 3) {
  console.error('error: no input files. stop.');
  console.error(`usage: node ${process.argv[1]} <input files>`);
  process.exit(1);
}

function findAllPositions(text, marker) {
  const positions = [];
  let index = text.indexOf(marker);

  while (index !== -1) {
    positions.push(index);
    index = text.indexOf(marker, index + marker.length);
  }

  return positions;
}

function findIgnoreRegions(text) {
  const ignorePositions = findAllPositions(text, ignoreMarker);
  const resumePositions = findAllPositions(text, resumeMarker);

  if (ignorePositions.length !== resumePositions.length) {
    console.error('error: unmatched ignore/resume markers. stop.');
    process.exit(1);
  }

  const regions = [];
  for (let i = 0; i < ignorePositions.length; i++) {
    const startIndex = ignorePositions[i] + ignoreMarker.length;
    const endIndex = resumePositions[i];

    regions.push({
      start: startIndex,
      end: endIndex,
      text: text.slice(startIndex, endIndex),
    });
  }
  return regions;
}

function replaceCalloutsToIgnore(text) {
  const noteRegex = /^>\s?\[!(.*?)\]/gm;

  return text.replaceAll(noteRegex, (_, p1) => {
    return `<!-- @mdformat note ${p1} -->`;
  });
}

function recoverCalloutsFromIgnore(text) {
  const ignoreNoteRegex = /<!-- @mdformat note (.*?) -->\n/gm;
  return text.replaceAll(ignoreNoteRegex, (_, p1) => {
    return `> [!${p1}]`;
  });
}

for (let i = 2; i < process.argv.length; i++) {
  const filePath = process.argv[i];
  let fileText = fs.readFileSync(filePath, 'utf8');

  const ignoreRegions = findIgnoreRegions(fileText);

  for (let j = ignoreRegions.length - 1; j >= 0; j--) {
    fileText = fileText.slice(0, ignoreRegions[j].start) +
        fileText.slice(ignoreRegions[j].end);
  }

  fileText = replaceCalloutsToIgnore(fileText);
  fs.writeFileSync(filePath, fileText);
  execSync(`mdformat "${filePath}" ${options || ''}`);

  let updatedFileText = fs.readFileSync(filePath, 'utf8');
  updatedFileText = recoverCalloutsFromIgnore(updatedFileText);
  const updatedRegions = findIgnoreRegions(updatedFileText);

  for (let j = updatedRegions.length - 1; j >= 0; j--) {
    updatedFileText = updatedFileText.slice(0, updatedRegions[j].start) +
        ignoreRegions[j].text + updatedFileText.slice(updatedRegions[j].end);
  }

  fs.writeFileSync(filePath, updatedFileText);
}

process.exit(0);
