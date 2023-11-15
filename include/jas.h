/**
 * Welcome to the Jas assembler project! Jas is a MIT licensed, free
 * and open-source x86 assembler capable of generating ELF files
 * designed to run on on linux systems.
 *
 * Jas is also designed to be used with an API for easy integration
 * with pragmatic approaches! (I.E. C/C++ ABI and APIs)
 *
 * Please check out our Github page for more information!
 * https://github.com/cheng-alvin/jas
 *
 * Made with love by Alvin / the Jas crew and contributors ❤️.
 *
 * ------------------------------------------------------------------------------
 *
 * Copyright (c) 2023 Alvin Cheng

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "aaa.h"
#include "endian.h"
#include "error.h"
#include "hint.h"