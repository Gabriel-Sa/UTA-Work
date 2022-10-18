# Gabriel de Sa 1001676832
import sys
import numpy as np


def linear_regression(training_file, test_file, degree, lmbda):
    training_data = []
    w = []
    degree = int(degree)
    lmbda = float(lmbda)
    t = []
    i = 0
    with open(training_file) as file:
        lines = file.readlines()
        for line in lines:
            line = line.split()
            training_data.append([1])
            for x in line:
                x = float(x)
                if len(training_data[i])-1 < (degree*len(line)-degree):
                    for n in range(0, degree):
                        training_data[i].append(x**(n+1))
                else:
                    t.append(x)
            i = i + 1
    for i in range(0, len(training_data[0])):
        w.append(1.00)
    t = np.matrix(t)
    phi = np.matrix(training_data)
    w = np.matrix(w)
    # w = (lambdaI + phi' * phi)^-1 *phi' * t
    lmbdaI = lmbda*np.identity(len(w.transpose()))
    phiT = phi.transpose()
    w = np.linalg.pinv(lmbdaI + phiT*phi) * (phiT * t.transpose())
    # output training results
    for index in range(0, len(w)):
        print("w%d=%.4f" % (index, w[index]))
    # beginning testing:
    with open(test_file) as file:
        lines = file.readlines()
        for i, line in enumerate(lines):
            testing_data = []
            line = line.split()
            testing_data.append(1)
            for x in line:
                x = float(x)
                if len(testing_data)-1 < (degree*len(line)-degree):
                    for n in range(0, degree):
                        testing_data.append(x**(n+1))
                else:
                    t = x
            testing_data = np.matrix(testing_data).transpose()
            output = w.transpose()*testing_data
            squaredError = (t - output)**2
            print("ID = %5d, Output = %14.4f, Target = %10.4f, Squared Error = %.4f"
                  % (i+1, output, t, squaredError))
    return training_file, test_file, degree, lmbda


linear_regression(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
