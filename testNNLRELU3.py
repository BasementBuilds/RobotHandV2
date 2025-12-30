import numpy as np
import pandas as pd
import random

numHiddenNodes = 18

def loadSensorData(file_path):   #load sensor data
    """Load pressure sensor data from CSV file"""
    # Expected CSV format: 
    # sensor1,sensor2,...,sensorN,label
    data = pd.read_csv(file_path)
    
    # Separate features and labels
    X = data.iloc[:, :-1].values  # All columns except last
    y = data.iloc[:, -1].values   # Last column is label
    
    return X, y

def relu(x):
    out = np.zeros(len(x))
    for a in range(len(x)):
        if (x[a]>0):
            out[a] = x[a]
        elif x[a]<=0:
            out[a] = .1*x[a]
    
    return out
   # return np.maximum(0.0,x)


def reluDerriv(x):
    d=0
    if (x>0):
        d = 1
    else:
        d = 0.1
    return d

def calculateCost(predicted,actual):
    error = 0
    for i in range(len(predicted)):
        error = error + np.square(predicted[i]-actual[i])

    return error

def forwardPass(input, inHW,inHB,HOutW,HOutB):

    hiddenWAvg = np.dot(inHW,input)
    hiddenWB = hiddenWAvg+inHB
    hidden = relu(hiddenWB)

    outWAvg = np.dot(HOutW,hidden)
    outWB = outWAvg + HOutB
    out = relu(outWB) 

    intGuess = 1 #find the guess as an integer (1,2,3,or4)
    if(out[1]>out[0] and out[1]>out[2] and out[1]>out[3] ):
        intGuess = 2
    elif(out[2]>out[0] and out[2]>out[1] and out[2]>out[3]):
        intGuess = 3
    elif(out[3]>out[0] and out[3]>out[1] and out[3]>out[2]):
        intGuess = 4

    returnVar =[hidden,out,intGuess]

    return returnVar

def findGradients (input, inHW,inHB,HOutW,HOutB,trueAns):
    
    hiddenLayer = np.zeros(numHiddenNodes)

    fPassVars= forwardPass(input, inHW,inHB,HOutW,HOutB)
    hiddenLayer =fPassVars[0]
    out = fPassVars[1] 

    #print("guess: ",out)

    cost = calculateCost(out,trueAns)
    #print(cost)

    dCdOut = np.zeros(4)
    dAdZ = np.zeros(4)
    dZdW = np.zeros (numHiddenNodes)
    dCdW = np.zeros((4,numHiddenNodes))
    dCdB = np.zeros(4)

    #gradient calculations for last layer (hidden and out)
    for i in range(len(dCdOut)):        #derriv of cost wrt output
        dCdOut[i] = 2*(out[i]-trueAns[i])

    for i in range(len(dAdZ)):        #partial derriv of cost wrt activation function of output layer   
        dAdZ[i] = reluDerriv(out[i])
 
    for i in range(len(dZdW)):          #derriv of z wrt weights in last layer
        dZdW[i] = hiddenLayer[i]
            
    for i in range(len(dCdB)):          #derriv of cost wrt biases in last layer
        dCdB[i] = 1*dCdOut[i]*dAdZ[i]

    for row in range(len(dCdW)):        #derriv of cost wrt weights in last layer
        for col in range(len(dCdW)):
            dCdW[row][col] = dCdOut[row]*dAdZ[row]*dZdW[col]

    dCdW_in = np.zeros((numHiddenNodes,16))
    dCdH = np.zeros(numHiddenNodes)
    dCdB_in = np.zeros(numHiddenNodes)
    
    #gradient for first layer (in and hidden)

    for i in range(len(dCdH)): #derriv of cost wrt Hidden layer (after act func)
        for j in range(4):
            #sum over all of output layer   dc/dout * dA(out)/dz * dz(out)/dh 
            dCdH[i] = dCdH[i]+dCdOut[j]*dAdZ[j]* HOutW[j][i]

    for i in range(numHiddenNodes):  #derriv of cost wrt input/hidden weights
        for j in range(16):
            dCdW_in[i][j]= dCdH[i]*reluDerriv(hiddenLayer[i])*input[j]
    
    for i in range(numHiddenNodes):  #derriv of cost wrt hidden biases
        dCdB_in[i] = dCdH[i]*reluDerriv(hiddenLayer[i])*1

    gradients = [dCdB,dCdW,dCdB_in,dCdW_in,cost]

    return gradients


trainValues,trainLabelInt = loadSensorData(r"C:\robotHandV2\sensorsData.csv")
testVals,testLabelInt = loadSensorData(r"C:\robotHandV2\testData16sensors.csv")
trainLabels = []
testLabels = []   # change labels from number (1,2,3,4) to array, for use in NN cost function [0,1,0,0]
for i in range(len(trainLabelInt)):
    if trainLabelInt[i]==1:
        trainLabels.append( [1,0,0,0])
    elif trainLabelInt[i]==2:
        trainLabels.append( [0,1,0,0])
    elif trainLabelInt[i]==3:
        trainLabels.append( [0,0,1,0])
    elif trainLabelInt[i]==4:
        trainLabels.append( [0,0,0,1])
for i in range(len(testLabelInt)):
    if testLabelInt[i]==1:
        testLabels.append( [1,0,0,0])
    elif testLabelInt[i]==2:
        testLabels.append( [0,1,0,0])
    elif testLabelInt[i]==3:
        testLabels.append( [0,0,1,0])
    elif testLabelInt[i]==4:
        testLabels.append( [0,0,0,1])
trainValues = trainValues*.0001
testVals = testVals*.0001


inH_weights= np.random.uniform  (-1,1,(numHiddenNodes,16)) #first index is which sensor input, second index is which node of hidden layer it connects (16 cols, numHiddenNodes rows)
inH_biases = np.random.uniform  (-1,1,numHiddenNodes) #biases for hidden layer
HOut_weights = np.random.uniform(-1,1,(4,numHiddenNodes)) # weights bet hidden and output layer
HOut_biases = np.random.uniform (-1,1,4)      #output biases

wOriginal = inH_weights

for g in range(200):

    totalCost = 0

    for j in range(len(trainValues)):

        gradients = findGradients(trainValues[j],inH_weights,inH_biases,HOut_weights,HOut_biases,trainLabels[j])
        #print((i+j))
        inH_weights = inH_weights   -.05*gradients[3]
        inH_biases = inH_biases     -.05*gradients[2]
        HOut_weights = HOut_weights -.05*gradients[1]
        HOut_biases = HOut_biases   -.05*gradients[0]

        totalCost = totalCost + gradients[4]
        
        

    for i in range (4):
        for j in range (numHiddenNodes):
            if (HOut_weights[i][j]>10):
                HOut_weights[i][j]=10
    for i in range (numHiddenNodes):
        for j in range (16):
            if (inH_weights[i][j]>10):
                inH_weights[i][j]=10


    corr,incorr = 0,0
    for i in range(len(testVals)):

        fpass = forwardPass(testVals[i],inH_weights,inH_biases,HOut_weights,HOut_biases)
        out = fpass[1]
        intGuess = fpass[2]

        tval = testLabels[i]


        if(intGuess == testLabelInt[i]):
            corr +=1
        else:
            incorr +=1

    corrPerc = 100*corr/(corr+incorr)
    print(corrPerc, " , ",totalCost)
    #print("correct:",corr," incorrect:",incorr, "  cost  ",totalCost)



corr,incorr = 0,0

for i in range(len(testVals)):

    fpass = forwardPass(testVals[i],inH_weights,inH_biases,HOut_weights,HOut_biases)
    out = fpass[1]
    intGuess = fpass[2]

    tval = testLabels[i]


    if(intGuess == testLabelInt[i]):
        corr +=1
    else:
        incorr +=1


    #print("guess: ", out, " true: ", tval)
    #print(out, "   guess: ", intGuess, " true: ", testLabelInt[i])

print("correct:",corr," incorrect:",incorr)

