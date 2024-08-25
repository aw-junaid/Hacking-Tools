"""
Copyright (c) 2015-2016 Roberto Christopher Salgado Bjerre.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

from os import name

# Operating System
WINDOWS = name == "nt"

# ANSI color codes
W = '\033[0m'
BW = '\033[1m'
R = '\033[31m'
G = '\033[32m'
O = '\033[33m'
B = '\033[34m'
P = '\033[35m'
C = '\033[36m'
GR = '\033[37m'

# Information levels
ASK = "[%s?%s]" % (("", "") if WINDOWS else (B, W))
PLUS = "[%s+%s]" % (("", "") if WINDOWS else (G, W))
INFO = "[%si%s]" % (("", "") if WINDOWS else (C, W))
TEST = "[%s*%s]" % (("", "") if WINDOWS else (B, W))
WARN = "[%s!%s] %sWarning%s:" % (("", "", "", "") if WINDOWS else (O, W, O, W))
ERROR = "[%sx%s] %sERROR%s:" % (("", "", "", "") if WINDOWS else (R, W, R, W))
DEBUG = "[%sd%s] %sDEBUG%s:" % (("", "", "", "") if WINDOWS else (P, W, P, W))
