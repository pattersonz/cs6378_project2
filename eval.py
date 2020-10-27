import os
import sys
import re

def msgOrder(x):
    return int(x['clock'])

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
            printProc(msgs,i)
        elif x.lower() == 'm':
            evalMutex(msgs)
        elif x.lower() == 'f':
            evalFairness(msgs)

if __name__ == "__main__":
    main()
