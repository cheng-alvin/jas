/**
 * MIT License
 * Copyright (c) 2023-2024 Alvin Cheng (eventide1029@gmail.com)
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

#ifndef JAS_H
#define JAS_H

/**
 * @author cheng-alvin
 * Copyright (c) 2023-2024 Alvin Cheng (eventide1029@gmail.com)
 *
 * Jas is a minimal, fast and simple zero-dependency assembler for the x64
 * family of processors, jas not only aims to be fast and simple when using
 * it but also aims to be a learning resource for others to learn about
 * low-level system programming and the x64 instruction set.
 *
 * @see README.md For more information on the project
 * @see `LICENSE` For the license information
 * @see https://github.com/cheng-alvin/jas
 *
 * Made with love by Alvin / the Jas crew and contributors ❤️ .
 */

#ifdef __cplusplus
namespace jas {
  extern "C" {
#endif

#include "buffer.h"
#include "codegen.h"
#include "error.h"
#include "instruction.h"
#include "label.h"
#include "mode.h"
#include "operand.h"
#include "register.h"

#ifdef __cplusplus
  }
}
#endif

#endif
