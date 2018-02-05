import os
import re
import sys

kResultFolder = './result'

def parseValueFromLine(line):
    infValue = (line[:-1].split(':')[1]).split(' ')[-2]
    return float(infValue)

def parseResultToCSV(exp_name):
    f = open('%s/%s_processed.csv' % (kResultFolder, exp_name), 'w');
    
    currentResult = []
    lines = open("%s/%s.result" % (kResultFolder, exp_name)).readlines()
    for i in xrange(len(lines)):
        line = lines[i]
        if('edgeWeight' in line):
            myMatch = re.search('edgeWeight=([0-9]+\.[0-9]+)', line)
            if(myMatch):
                currentWeight = myMatch.group(1)
        if('InfMax:Obv'  in line):
            currentInfObv = parseValueFromLine(lines[i+1])
        if('edgeDet' in line):
            myMatch = re.search('edgeDet=([0-9]+\.[0-9]+)', line)
            if(myMatch):
                currentDet = myMatch.group(1)
                currentResult = [0.0] * 9
        if('InfMax:Min' in line):
            currentResult[2] = parseValueFromLine(lines[i+1])
        if('InfMax:Max' in line):
            currentResult[3] = parseValueFromLine(lines[i+1])
        if('InfDiffMax:MaxMin' in line):
            currentResult[5] = parseValueFromLine(lines[i+1])
            currentResult[0] = currentWeight
            currentResult[1] = currentDet
            currentResult[4] = currentInfObv
            outputArr = []
            for tn in currentResult:
                num = float(tn)
                if(num<0):
                    num = 0
                outputArr.append(num)
            f.write('\t'.join([str(num) for num in outputArr]) + '\n')
        if('InfDiffMax:MaxObv' in line):
            currentResult[6] = parseValueFromLine(lines[i+1])
        if('InfDiffMaxMin' in line):
            myMatch = re.search('InfDiffMaxMin=([-]*[0-9]+\.?[0-9]+)', line)
            currentResult[7] = myMatch.group(1)
        if('InfDiffObvMin' in line):
            myMatch = re.search('InfDiffObvMin=([-]*[0-9]+\.[0-9]+)', line)
            #print line
            currentResult[8] = myMatch.group(1)

if __name__ == '__main__':
    exp_name = sys.argv[1]
    parseResultToCSV(exp_name)
