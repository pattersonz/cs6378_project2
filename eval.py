import os
import sys
import re

def msgOrder(x):
    return int(x['clock'])*100+int(x['id'])

def readFile(fi):
    msgs = []
    msgLine = re.compile("id:(?P<id>\d+) clock\:(?P<clock>\d+) (?P<msg>.*)")
    for l in fi:
        l = l.rstrip()
        print l
        if msgLine.match(l):
            x = msgLine.search(l)
            y = {}
            y["id"] = x.group('id')
            y["clock"] = x.group('clock')
            y["msg"] = x.group('msg')
            print y
            msgs.append(y)
    msgs.sort(key=msgOrder)
    return msgs

def printProc(msgs, i):
    for m in msgs:
        if m['id'] == str(i):
            print 'id:' + m['id'] + ' clock:' + m['clock'] + ' ' + m['msg']
    return

def evalMutex(msgs):
    isIn = False
    for m in msgs:
        if m['msg'] == "Entering":
            print m
            if isIn:
                print 'Error, mutex conflict'
                return
            else:
                isIn = True
        elif m['msg'] == "Exiting":
            print m
            if isIn:
                isIn = False
            else:
                print 'Error exiting no mutex'
                return
    print 'Success!'
    return

def evalFairness(msgs):
    queue = []
    for m in msgs:
        if m['msg'] == "Requesting":
            print m
            queue.append(m)
            queue.sort(key=msgOrder)
        elif m['msg'] == "Entering":
            if m['id'] == queue[0]['id']:
                del queue[0]
            else:
                print 'error got:' + m['id'] + ' expected:' + queue[0]['id']
                return
    print 'Success!'
    return
def main():
    x = ""
    msgs = []
    while x.lower() != "x":
        print "Action:\nRead file:R\nPrint Proc:P\nEval Mutex:M\nEval Fairness:F\nExit:X\n"
        x = raw_input("Command:")
        x = x.strip()
        if x.lower() == 'r':
            f = raw_input("File:")
            f = f.strip()
            fi = open(f)
            msgs = readFile(fi)
            fi.close()
        elif x.lower() == 'p':
            i = raw_input("Proc id:")
            i = i.strip()
            if i.lower() == 'x':
                print msgs
            else:
                printProc(msgs,i)
        elif x.lower() == 'm':
            evalMutex(msgs)
        elif x.lower() == 'f':
            evalFairness(msgs)

if __name__ == "__main__":
    main()
