GOOS=linux GOARCH=386 go build -ldflags="-s -w" bot.go
mv bot x86
GOOS=linux GOARCH=arm GOARM=7 go build -ldflags="-s -w" bot.go
mv bot armv7l
GOOS=linux GOARCH=arm GOARM=5 go build -ldflags="-s -w" bot.go
mv bot armv5l
GOOS=linux GOARCH=arm64 go build -ldflags="-s -w" bot.go
mv bot armv8l
GOOS=linux GOARCH=mips go build -ldflags="-s -w" bot.go
mv bot mips
GOOS=linux GOARCH=mipsle go build -ldflags="-s -w" bot.go
mv bot mipsel
