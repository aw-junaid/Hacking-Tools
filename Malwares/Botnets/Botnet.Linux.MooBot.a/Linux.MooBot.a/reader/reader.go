package main

import (
    "fmt"
    "net"
    "bufio"
    "os"
    "sync"
    "time"
    "math/big"
    "math/rand"
    "encoding/binary"
)

var syncWait sync.WaitGroup
var bruteQueue = New()
var acceptingNew = 1
var distributedTotal = 0
var workersTotal = 0
const minQueueLen = 32

type Queue struct {
	items             map[int64]interface{}
	ids               map[interface{}]int64
	buf               []int64
	head, tail, count int
	mutex             *sync.Mutex
	notEmpty          *sync.Cond
	NotEmpty chan struct{}
}

func New() *Queue {
	q := &Queue{
		items:    make(map[int64]interface{}),
		ids:      make(map[interface{}]int64),
		buf:      make([]int64, minQueueLen),
		mutex:    &sync.Mutex{},
		NotEmpty: make(chan struct{}, 1),
	}

	q.notEmpty = sync.NewCond(q.mutex)

	return q
}

func (q *Queue) Length() int {
	q.mutex.Lock()
	defer q.mutex.Unlock()

	return len(q.items)
}

func (q *Queue) resize() {
	newCount := q.count << 1

	if q.count < 2<<18 {
		newCount = newCount << 2
	}

	newBuf := make([]int64, newCount)

	if q.tail > q.head {
		copy(newBuf, q.buf[q.head:q.tail])
	} else {
		n := copy(newBuf, q.buf[q.head:])
		copy(newBuf[n:], q.buf[:q.tail])
	}

	q.head = 0
	q.tail = q.count
	q.buf = newBuf
}

func (q *Queue) notify() {
	if len(q.items) > 0 {
		select {
		case q.NotEmpty <- struct{}{}:
		default:
		}
	}
}

func (q *Queue) Append(elem interface{}) {
	q.mutex.Lock()
	defer q.mutex.Unlock()

	if q.count == len(q.buf) {
		q.resize()
	}

	id := q.newId()
	q.items[id] = elem
	q.ids[elem] = id
	q.buf[q.tail] = id
	q.tail = (q.tail + 1) & (len(q.buf) - 1)
	q.count++

	q.notify()

	if q.count == 1 {
		q.notEmpty.Broadcast()
	}
}

func (q *Queue) newId() int64 {
	for {
		id := rand.Int63()
		_, ok := q.items[id]
		if id != 0 && !ok {
			return id
		}
	}
}

func (q *Queue) pop() int64 {
	for {
		if q.count <= 0 {
			q.notEmpty.Wait()
		}

		if q.count > 0 {
			break
		}
	}

	id := q.buf[q.head]
	q.buf[q.head] = 0
	q.head = (q.head + 1) & (len(q.buf) - 1)
	q.count--
	if len(q.buf) > minQueueLen && (q.count<<1) == len(q.buf) {
		q.resize()
	}

	return id
}

func (q *Queue) Pop() interface{} {
	q.mutex.Lock()
	defer q.mutex.Unlock()

	for {
		id := q.pop()

		item, ok := q.items[id]

		if ok {
			delete(q.ids, item)
			delete(q.items, id)
			q.notify()
			return item
		}
	}
}

func (q *Queue) Remove(elem interface{}) bool {
	q.mutex.Lock()
	defer q.mutex.Unlock()

	id, ok := q.ids[elem]
	if !ok {
		return false
	}
	delete(q.ids, elem)
	delete(q.items, id)
	return true
}

func IP4toInt(IPv4Address net.IP) int64 {
    IPv4Int := big.NewInt(0)
    IPv4Int.SetBytes(IPv4Address.To4())
    return IPv4Int.Int64()
}

func distrubuteWorker(conn net.Conn) {

	defer conn.Close()
	workersTotal++

	for {
		if bruteQueue.Length() >= 1 {
			val := bruteQueue.Pop()
			ipStr := fmt.Sprintf("%v", val)
			ipv4Decimal := IP4toInt(net.ParseIP(ipStr))

			err := binary.Write(conn, binary.BigEndian, uint32(ipv4Decimal)) 
			if err != nil {
				break
			}

			distributedTotal++
			time.Sleep(45 * time.Millisecond)
		}
	}

	workersTotal--
}

func main() {

    accept, err := net.Listen("tcp", "0.0.0.0:1999")
    if err != nil {
        fmt.Println(err)
        return
    }

    go func() {
    	var i int = 0

    	for {
    		if acceptingNew == 0 && bruteQueue.Length() < 1000000 {
    			acceptingNew = 1
    		}

			if bruteQueue.Length() > 1000000 {
				acceptingNew = 0
			}

			fmt.Printf("%d's | Queue Size: %d | Distrubuted %d | Workers Online %d\n", i, bruteQueue.Length(), distributedTotal, workersTotal)
			i++
			time.Sleep(1 * time.Second)
    	}
    } ()

    go func() {
	    for {
	        conn, err := accept.Accept()
	        if err != nil {
	            break
	        }
	        
	        go distrubuteWorker(conn)
	    }
	} ()

	for {
	    reader := bufio.NewReader(os.Stdin)
	    input := bufio.NewScanner(reader)

	    for input.Scan() {
	    	if acceptingNew == 1 {
	    		bruteQueue.Append(input.Text())
	    	} else {
	    		str := input.Text()
	    		str = ""
	    		_ = str
	    	}
	    }
	}
}