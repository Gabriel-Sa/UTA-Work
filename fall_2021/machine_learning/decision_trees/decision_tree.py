# Gabriel de Sa 1001676832
import sys
import numpy as np


class Tree:
    def __init__(self, root, thresh, gain):
        self.root = root
        self.thresh = thresh
        self.gain = gain
        self.left_child = 0
        self.right_child = 0


def openFile(fileName):
    returnArray = []
    classArray = []
    with open(fileName) as file:
        lines = file.readlines()
        for i, line in enumerate(lines):
            line = line.split()
            returnArray.append([])
            for x in line:
                if len(returnArray[i]) < len(line)-1:
                    returnArray[i].append(float(x))
                else:
                    classArray.append(x)
    return (returnArray, classArray)


def entropy(dist):
    sum = 0
    for k in dist:
        if k == 0:
            sum = sum + 0
        else:
            sum = sum + (-k*np.log2(k))
    return sum


def informationGain(attributes, classes, threshold, classList):
    h = entropy(distribution(classes, classList))  # H(E) top distribution of classes
    # next I need to apply thresholds and separate the classes based on attribute
    classLeft, classRight = [], []
    for i, attribute in enumerate(attributes):
        if attribute < threshold:
            classLeft.append(classes[i])
        elif attribute >= threshold:
            classRight.append(classes[i])
    # Create the weights based off distribution
    kLeft = len(classLeft)/len(classes)
    kRight = len(classRight)/len(classes)
    # find entropy for left
    h1 = entropy(distribution(classLeft, classList))
    # find entropy of right
    h2 = entropy(distribution(classRight, classList))
    sum = (kLeft*h1) + (kRight*h2)
    return (h - sum)


def optimized(attributes, classes, classList):
    bestAttribute, bestThreshold, maxGain = -1, -1, -1
    for A in range(len(attributes[0])):
        attributeValues = []
        for example in attributes:
            attributeValues.append(example[A])
        L = min(attributeValues)
        M = max(attributeValues)
        for k in range(1, 50):
            threshold = L + k*(M-L)/51
            gain = informationGain(attributeValues, classes, threshold, classList)
            if gain > maxGain:
                maxGain = gain
                bestAttribute = A
                bestThreshold = threshold
    return (bestAttribute, bestThreshold, maxGain)


def randomized(attributes, classes, classList):
    maxGain, bestThreshold = -1, -1
    A = np.random.randint(0, len(attributes[0]))
    attributeValues = []
    for example in attributes:
        attributeValues.append(example[A])
    L = min(attributeValues)
    M = max(attributeValues)
    for k in range(1, 50):
        threshold = L + k*(M-L)/51
        gain = informationGain(attributeValues, classes, threshold, classList)
        if gain > maxGain:
            maxGain = gain
            bestThreshold = threshold
    return (A, bestThreshold, maxGain)


def split(attributes, classes, bestAttribute, bestThreshold, side):
    attributesArray, classesArray = [], []
    if side == "left":
        for i, example in enumerate(attributes):
            if example[bestAttribute] < bestThreshold:
                attributesArray.append(example)
                classesArray.append(classes[i])
    else:
        for i, example in enumerate(attributes):
            if example[bestAttribute] >= bestThreshold:
                attributesArray.append(example)
                classesArray.append(classes[i])
    return attributesArray, classesArray


def distribution(classes, classList):
    dist = [0] * len(classList)
    for i, class_ in enumerate(classList):
        if len(classes) != 0:
            dist[i] = (classes.count(class_)/len(classes))
        else:
            dist[i] = 0
    return dist


def createClassList(classes):
    classList = []
    for class_ in classes:
        if class_ not in classList:
            classList.append(class_)
    return classList


def DTL(attributes, classes, default, chooseAttribute, pruning, classList):
    if len(classes) < pruning:
        return default
    elif classes.count(classes[0]) == len(classes):  # if all classes are the same
        return distribution(classes, classList)
    else:
        (bestAttribute, bestThreshold, gain) = chooseAttribute(attributes, classes, classList)
        tree = Tree(bestAttribute, bestThreshold, gain)  # a new decision tree with root best
        examplesLeft, classesLeft = split(attributes, classes, bestAttribute, bestThreshold, "left")
        examplesRight, classesRight = split(attributes, classes, bestAttribute, bestThreshold, "right")
        dist = distribution(classes, classList)
        tree.left_child = DTL(examplesLeft, classesLeft, dist, chooseAttribute, pruning, classList)
        tree.right_child = DTL(examplesRight, classesRight, dist, chooseAttribute, pruning, classList)
        return tree


def average(arrays):
    ret = [0] * len(arrays[0])
    ret = np.array(ret)
    for arr in arrays:
        ret = ret + arr
    ret = ret/len(arrays)
    return list(ret)


def printTree(tree, treeId):
    queue = []
    queue.append((tree, 1))
    while(queue):
        current = queue[0][0]
        nodeId = queue[0][1]
        if type(current) != list:
            print("tree=%2d, node=%3d, feature=%2d, thr=%6.2f, gain=%f" %
                  (treeId, nodeId, current.root+1, current.thresh, current.gain))
            if(current.left_child):
                queue.append((current.left_child, 2*nodeId))
            if(current.right_child):
                queue.append((current.right_child, 2*nodeId+1))
        else:
            print("tree=%2d, node=%3d, feature=%2d, thr=%6.2f, gain=%f" % (treeId, nodeId, -1, -1, 0))
        queue.pop(0)


def traverse(example, tree):
    result = []
    current = tree
    # Traverse tree
    while type(current) == Tree:
        attribute = example[current.root]
        if attribute < current.thresh:
            current = current.left_child
        elif attribute >= current.thresh:
            current = current.right_child
    result = current
    return result


def findAccuracy(id, result, trueIndex, classList):
    accuracy = 0
    chosen = max(result)
    if (result.index(chosen) == trueIndex) & (result.count(chosen) == 1):
        accuracy = 1
        print("ID = %5d, predicted = %s, true = %s, accuracy = %4.2f"
              % (id, classList[result.index(chosen)], classList[trueIndex], accuracy))
    elif result.count(chosen) > 1:
        ties = []
        for i, c in enumerate(result):
            if c == chosen:
                ties.append(i)
        if trueIndex in ties:
            accuracy = 1/len(ties)
            print("ID = %5d, predicted = %s, true = %s, accuracy = %4.2f"
                  % (id, classList[trueIndex], classList[trueIndex], accuracy))
    else:
        print("ID = %5d, predicted = %s, true = %s, accuracy = %4.2f"
              % (id, classList[result.index(chosen)], classList[trueIndex], accuracy))
    return accuracy


def test(examples, trueClass, tree, classList):
    totalAccuracy = 0
    for i, example in enumerate(examples):
        result = traverse(example, tree)
        totalAccuracy = totalAccuracy + findAccuracy(i+1, result, classList.index(trueClass[i]), classList)
    return totalAccuracy/len(examples)


def forestTest(examples, trueClass, forest, classList):
    totalAccuracy = 0
    for i, example in enumerate(examples):
        resultsArray = []
        for tree in forest:
            resultsArray.append(traverse(example, tree))
        result = average(resultsArray)
        totalAccuracy = totalAccuracy + findAccuracy(i+1, result, classList.index(trueClass[i]), classList)
    return totalAccuracy/len(examples)


def decision_tree(training, testing, option, pruningThr):
    # Training Data:
    trainingInputs, trainingClasses = openFile(training)
    pruningThr = float(pruningThr)
    # Testing Data:
    testingInputs, testingClasses = openFile(testing)
    result = -1
    classList = createClassList(trainingClasses)
    default = distribution(trainingClasses, classList)
    if option == 'optimized':
        tree = DTL(trainingInputs, trainingClasses, default, optimized, pruningThr, classList)
        printTree(tree, 1)
        result = test(testingInputs, testingClasses, tree, classList)
    elif option == 'randomized':
        tree = DTL(trainingInputs, trainingClasses, default, randomized, pruningThr, classList)
        printTree(tree, 1)
        result = test(testingInputs, testingClasses, tree, classList)
    elif option == 'forest3':
        forest = []
        for i in range(3):
            forest.append(DTL(trainingInputs, trainingClasses, default, randomized, pruningThr, classList))
            printTree(forest[i], i+1)
        result = forestTest(testingInputs, testingClasses, forest, classList)
    elif option == 'forest15':
        forest = []
        for i in range(15):
            forest.append(DTL(trainingInputs, trainingClasses, default, randomized, pruningThr, classList))
            printTree(forest[i], i+1)
        result = forestTest(testingInputs, testingClasses, forest, classList)
    return result


results = decision_tree(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
print("Classification Accuracy = %6.4f" % results)
