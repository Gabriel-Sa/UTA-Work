# Gabriel de Sa 1001676832 Assignment 6
import sys
import numpy as np


class DataPoint:
    def __init__(self, data, cluster):
        self.data = data
        self.cluster = cluster


def checkClusters(data, prevClusters):
    if not prevClusters:
        return False
    for i, x in enumerate(data):
        if x.cluster == prevClusters[i]:
            continue
        else:
            return False
    return True


def findCluster(data, means):
    min = 100000
    cluster = None
    if(len(data) > 1):
        for i, avg in enumerate(means):
            distance = np.sqrt((data[0] - avg[0])**2 + (data[1] - avg[1])**2)
            if distance < min:
                min = distance
                cluster = i
    else:
        for i, avg in enumerate(means):
            distance = abs(data[0] - avg)
            if distance < min:
                min = distance
                cluster = i
    return cluster


def clusterMean(dataPoints, K):
    if len(dataPoints[0].data) > 1:
        sumArray, countArray, meanArray = [[0] * 2 for i in range(K)], [0]*K, [[0] * 2 for i in range(K)]
        for dataPoint in dataPoints:
            cluster = dataPoint.cluster
            sumArray[cluster][0] = sumArray[cluster][0] + dataPoint.data[0]
            sumArray[cluster][1] = sumArray[cluster][1] + dataPoint.data[1]
            countArray[cluster] = countArray[cluster] + 1
        for i in range(len(sumArray)):
            if countArray[i] != 0:
                meanArray[i][0] = sumArray[i][0]/countArray[i]
                meanArray[i][1] = sumArray[i][1]/countArray[i]
            else:
                meanArray[i][0], meanArray[i][1] = 0, 0
    else:
        sumArray, countArray, meanArray = [0]*K, [0]*K, [0]*K
        for dataPoint in dataPoints:
            cluster = dataPoint.cluster
            sumArray[cluster] = sumArray[cluster] + dataPoint.data[0]
            countArray[cluster] = countArray[cluster] + 1
        for i in range(len(sumArray)):
            if countArray[i] != 0:
                meanArray[i] = sumArray[i]/countArray[i]
            else:
                meanArray[i] = 0
    return meanArray


def k_means(inputFile, K, init):
    dataPoints = []
    K = int(K)
    with open(inputFile) as file:
        k = 0
        for i, line in enumerate(file.readlines()):
            if k == K:
                k = 0
            data = []
            for x in line.split():
                data.append(float(x))
            if init == "random":
                cluster = np.random.randint(0, K)
                dataPoints.append(DataPoint(data, cluster))
            else:
                dataPoints.append(DataPoint(data, k))
            k = k + 1
    means = clusterMean(dataPoints, K)
    prevClusters = []
    # Actual process of clustering using means and assignments
    run = True
    round = 1
    while run:
        # Check that all clusters aren't the same from last iteration.
        if checkClusters(dataPoints, prevClusters):
            run = False
        else:
            prevClusters = []
            for datapoint in dataPoints:
                prevClusters.append(datapoint.cluster)
            # First compute new assignments using the means.
            for datapoint in dataPoints:
                cluster = findCluster(datapoint.data, means)
                datapoint.cluster = cluster
            # Second, recompute the means based off the new assignments.
            means = clusterMean(dataPoints, K)
    # Print Results
    if len(dataPoints[0].data) > 1:
        for datapoint in dataPoints:
            print("(%10.4f, %10.4f) --> cluster %d" % (datapoint.data[0], datapoint.data[1], datapoint.cluster + 1))
    else:
        for datapoint in dataPoints:
            print("%10.4f --> cluster %d" % (datapoint.data[0], datapoint.cluster + 1))
    return


k_means(sys.argv[1], sys.argv[2], sys.argv[3])
