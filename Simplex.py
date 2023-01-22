# -*- coding: utf-8 -*-
"""
Created on Sat Dec 31 21:08:52 2022

@author: Acer
"""

import numpy as np
import time

def CheckBR(BRidx):
    BR = A[BRidx]
    BR = np.transpose(BR)
    if np.linalg.matrix_rank(BR) != M:
        return False, np.array([])
    if np.linalg.det(BR) == 0:
        return False, np.array([])
    BR = np.linalg.inv(BR)
    temp = np.dot(BR, B)
    for x in temp:
        if x < 0:
            return False, np.array([])
    return True, temp

def FindNextResult(TR):
    if TR == []:
        for i in range(0,M):
            TR.append(i)
        return TR
    for i in range(M-1,-1,-1):
        if(TR[i] == N + i):
            continue
        else:
            TR[i] += 1
            for j in range(i+1,M):
                TR[j] = TR[j-1] + 1
            return TR
        
def IsOptimum(BRidx, NBRidx):
    Cb = C[BRidx]
    Cn = C[NBRidx]
    Ab = A[BRidx]
    An = A[NBRidx]
    Temp = np.dot(np.dot(np.transpose(Cb),np.linalg.inv(Ab)), np.transpose(An))
    Temp -= Cn
    for t in Temp:
        if(tip == "min"):
            if(t >= 0):
                return False
        else:
            if(t <= 0):
                return False
    return True

def FindBestNonBaseVariable(BRidx, NBRidx):
    Cb = C[BRidx]
    Ab = A[BRidx]
    Temp = -1000000 if tip == "min" else 1000000
    RVId = -1
    for i in NBRidx:
        Ci = C[i]
        Ai = A[i]
        t = np.dot(np.dot(np.transpose(Cb),np.linalg.inv(Ab)), Ai) - Ci
        Temp = max(Temp, t) if tip == "min" else min(Temp, t)
        if Temp == t:
            RVId = i
    return RVId, Temp
  
def FindWorstBaseVariable(BRidx, NBRidx, k):   
    Ab = A[BRidx]
    An = A[NBRidx]
    Ank = An[k]
    AbB = np.dot(np.linalg.inv(Ab), B)
    y = np.dot(np.linalg.inv(Ab), Ank)
    RetVal = 1000000
    RVId = -1
    NCounter = 0
    for i in range(0, M):
        if y[i] == 0:
            continue
        Temp = AbB[i]/y[i]
        if Temp < 0:
            NCounter += 1
            continue
        RetVal = min(Temp, RetVal)
        if Temp == RetVal:
            RVId = BRidx[i]
    if NCounter == M:
        return -2, 0
    return RVId, RetVal
        
Z = 0
while Z < 1 or Z > 4:
    Z = int(input("Izaberite zadatak (1-4):"))

#ZADATAK 1
A = np.array([[1, 0, 0, 3, 0, 0, 1, 0, 0], 
              [0, 1, 0, 0, 3, 0, 1, 0, 0], 
              [0, 0, 1, 0, 0, 3, 1, 0, 0], 
              [1, 0, 0, 2, 0, 0, 0, 1, 0],
              [0, 1, 0, 0, 2, 0, 0, 1, 0],
              [0, 0, 1, 0, 0, 2, 0, 1, 0],
              [1, 0, 0, 1, 0, 0, 0, 0, 1],
              [0, 1, 0, 0, 1, 0, 0, 0, 1],
              [0, 0, 1, 0, 0, 1, 0, 0, 1]])

B = np.array([400, 600, 300, 600, 800, 375, 600, 500, 325])

C = np.array([1000, 1000, 1000, 750, 750, 750, 250, 250, 250])

tip = "max"


if Z == 2:
    #PRIMER 2
    A = np.array([[1,  1],
                  [1, -1]])
    B = np.array([4, 6])
    C = np.array([-2, 3])
    tip = "min"
elif Z == 3:
    #PRIMER 3
    A = np.array([[8, 4, 2, 0],
                  [6, 2, 1.5, 0],
                  [1, 1.5, 0.5, 1]])
    
    B = np.array([48, 20, 8, 5])
    C = np.array([60, 30, 20])
    tip = "max"
elif Z == 4:
    #PRIMER 4
    A = np.array([[-3, 2, 1],
                  [2, -4, 1]])
    B = np.array([2, 3, 6])
    C = np.array([2, -1])
    tip = "max"


#ODREDJIVANJE BROJA OGRANICENJA I PROMENLJIVIH
N, M = A.shape

#PRETVARANJE OGRANICENJA TIPA NEJEDNAKOSTI U TIP JEDNAKOSTI

I = np.eye(M)
A = np.concatenate((A, I))
C = np.concatenate((C, np.zeros(M)))

Xb = np.array([])
Xn = np.zeros(N)

#BIRANJE BAZNOG RESENJA
RF = False
BaseResult = []
while RF == False:
    BaseResult = FindNextResult(BaseResult)
    #print(BaseResult)
    RF, Xb = CheckBR(BaseResult)
    if RF or BaseResult[0] == N:
        break

#=========================
# BaseResult = [2,3]
# RF = True
# Xb = np.dot(np.linalg.inv(np.transpose(A[BaseResult])), B)
#=========================
if RF == False:
    print("Ne mozemo naci odgovarajuce bazno resenje!")
    time.sleep(2)
    exit();
    
NonBaseResult = []
for i in range(0,M+N):
    if i not in BaseResult:
        NonBaseResult.append(i)
# print("Pronadjeno bazno resenje:", BaseResult)
# print("Pronadjeno nebazno resenje:", NonBaseResult)

#BIRANJE BOLJEG RESENJA
WorstBVId = -1
while(True):
    if IsOptimum(BaseResult, NonBaseResult):
        break
    else:
        BestNBVId, BestNBV = FindBestNonBaseVariable(BaseResult, NonBaseResult)
        #print(BestNBVId, BestNBV)
        if(tip == "min"):
            if(BestNBV <= 0):
                break
        else:
            if(BestNBV >= 0):
                break
                
    #print(BestNBVId, BestNBV)
                
        
    WorstBVId, NewValueOfNBV = FindWorstBaseVariable(BaseResult, NonBaseResult, BestNBVId)
    if WorstBVId < 0:
        break
    #Izbacujemo najgoru baznu promenljivu
    Xb = np.delete(Xb, BaseResult.index(WorstBVId))
    BaseResult.remove(WorstBVId)
    #Ubacujemo novu nebaznu promenljivu
    NonBaseResult.append(WorstBVId)
    #Izbacujemo najbolju nebaznu promenljivu
    NonBaseResult.remove(NonBaseResult.index(BestNBVId))
    #Ubacujemo novu baznu promenljivu
    BaseResult.append(BestNBVId)
    
    Xb = np.dot(np.linalg.inv(np.transpose(A[BaseResult])), B)

if WorstBVId != -2:
    for i in range(0,M):
        print("x" + str(BaseResult[i]), "=", round(Xb[i],2))
    FinalResult = 0
    for i in BaseResult:
        FinalResult += Xb[BaseResult.index(i)]*C[i]
        
    print("OPTIMUM: ", round(FinalResult,2))
else:
    print("OPTIMUM: INFINITY")

        
    
    
    

    

    