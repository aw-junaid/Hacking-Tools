"""
Copyright (c) 2015-2016 Hypsurus <hypsurus@mail.ru>.

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

from time import strftime


class Logger(object):
    """ Log text into a file """
    def __init__(self, logger_file):
        self.cursor = None
        self.logger_file = logger_file

    def open(self):
        """ Will keep it open until calling logger.close()
            using 1 log file, to log all data"""
        self.cursor = open("%s.log" % (self.logger_file), "a")

    def write(self, data):
        """ Write to the log file """
        self.cursor.write("[%s] %s" % (strftime("%H:%M:%S"), data))

    def close(self):
        """ Here we close the log file,
         and we can write to it again."""
        if self.cursor:
            self.cursor.close()
