#!/usr/bin/env ruby

# frozen_string_literal: true

contents = ''

while (line = STDIN.gets)
  contents += line
end

REGEX = %r{(^.*?("|')(/[\w\d\W\?/&=\#\.\!:_-]*?)(\2).*$)}.freeze
EXCERPT_FORMAT = "------------------------------------------------\r\n%s\r\n"

def sanitize_non_ascii(string)
  encoding_options = {
    invalid: :replace,
    undef: :replace,
    replace: '_'
  }

  string.encode(Encoding.find('ASCII'), **encoding_options)
end

matched_endpoints = []

sanitize_non_ascii(contents).gsub(/;/, "\n").scan(REGEX).map do |string|
  next if matched_endpoints.include?(string[2])

  matched_endpoints << string[2]

  puts string[2]

  puts format(EXCERPT_FORMAT, string[0]) if ARGV[0] == '--show-line'
end
