# Gabriel de Sa 1001676832
import sys
import numpy as np


def generateWorld(environment):
    world = []
    with open(environment) as file:
        lines = file.readlines()
        for i, line in enumerate(lines):
            world.append([])
            x = line.split(',')
            for unit in x:
                unit = unit.strip('\n')
                if unit.endswith("."):
                    world[i].append({"reward": 0.00, "type": 'S'})
                elif 'X' in unit:
                    world[i].append({"reward": 0.00, "type": 'X'})
                else:
                    world[i].append({"reward": float(unit), "type": 'T'})
    return world


def stateReward(currentPosition, reward):
    if currentPosition["type"] == 'X':
        return 0
    if currentPosition["type"] == 'T':
        return currentPosition["reward"]
    return (reward)


def bellmanMax(u, world, i, j):
    probability = [.8, .1, .1]
    # Directions = [Up, right, down, left]
    up = [i-1, j]
    lft = [i, j-1]
    dwn = [i+1, j]
    rgt = [i, j+1]
    directions = [[up, lft, rgt],  # Up, left, right
                  [lft, dwn, up],  # left, down, up
                  [dwn, rgt, lft],  # down, right, left
                  [rgt, up, dwn]]   # right, up, down
    xDim = len(u)
    yDim = len(u[0])
    # For every directional move possible take the reward for the best move.
    # look in every direction and the take the max value. There are 4 possible directions
    # with each direction having 3 possible outcomes total of 12 calculations
    sumArray = []
    for direction in directions:
        # set desired direction
        sum = 0
        for prob, indices in zip(probability, direction):
            # calculate probabiliies from this direction
            if (0 <= indices[0] < xDim) and (0 <= indices[1] < yDim):
                if(world[indices[0]][indices[1]]['type'] == 'X'):
                    sum = sum + (prob*u[i][j])
                else:
                    sum = sum + (prob*u[indices[0]][indices[1]])
            else:
                sum = sum + (prob*u[i][j])
        sumArray.append(sum)
    return max(sumArray)


def value_iteration(environment, reward, gamma, k):
    world = generateWorld(environment)
    u = np.zeros_like(world)
    for i in range(k):
        U = np.copy(u)
        # print(i)
        for k, row in enumerate(world):
            for j, state in enumerate(row):
                # print("%6.2f" % u[k][j], end=" ")
                if(state['type'] == 'S'):
                    u[k][j] = stateReward(state, reward) + gamma*bellmanMax(U, world, k, j)
                else:
                    u[k][j] = stateReward(state, reward)
            # print()

    return U, world


def findDirection(u, i, j):
    probability = [.8, .1, .1]
    # Directions = [Up, left, down, right]
    up = [i-1, j]
    lft = [i, j-1]
    dwn = [i+1, j]
    rgt = [i, j+1]
    directions = [[up, lft, rgt],  # Up, left, right
                  [lft, dwn, up],  # left, down, up
                  [dwn, rgt, lft],  # down, right, left
                  [rgt, up, dwn]]   # right, up, down
    xDim = len(u)
    yDim = len(u[0])
    # For every directional move possible take the reward for the best move.
    # look in every direction and the take the max value. There are 4 possible directions
    # with each direction having 3 possible outcomes total of 12 calculations
    sumArray = []
    retArray = []
    for direction in directions:
        # set desired direction
        sum = 0
        for prob, indices in zip(probability, direction):
            # calculate probabiliies from this direction
            if (0 <= indices[0] < xDim) and (0 <= indices[1] < yDim):
                sum = sum + (prob*u[indices[0]][indices[1]])
            else:
                sum = sum + 0
        sumArray.append(sum)
    return np.argmax(sumArray)


def policy_iteration(utilities, world):
    # Directions = [Up, left, down, right]
    policy = np.zeros_like(world)
    for i, row in enumerate(utilities):
        for j, state in enumerate(row):
            if(world[i][j]['type'] == 'T'):
                policy[i][j] = 'o'
            elif(world[i][j]['type'] == 'X'):
                policy[i][j] = 'X'
            else:
                direction = findDirection(utilities, i, j)
                if direction == 0:
                    policy[i][j] = '^'
                elif direction == 1:
                    policy[i][j] = '<'
                elif direction == 2:
                    policy[i][j] = 'V'
                elif direction == 3:
                    policy[i][j] = '>'
    return policy


utility, world = value_iteration(sys.argv[1], float(sys.argv[2]), float(sys.argv[3]), int(sys.argv[4]))
print("Utilities:")
for row in utility:
    for state in row:
        print("%6.3f" % state, end=" ")
    print()

policy = policy_iteration(utility, world)
print("Policy:")
for row in policy:
    for state in row:
        print("%6s" % state, end=" ")
    print()

#      0      1      2      3
# 0   -0.040 -0.040 -0.040  1.000
# 1   -0.040  0.000 -0.040 -1.000
# 2   -0.040 -0.040 -0.040 -0.040
# Next Step:
#      0      1      2      3
# 0   -0.072 -0.047  0.673  1.000
# 1   -0.047  0.000 -0.047 -1.000
# 2   -0.069 -0.047 -0.069 -0.159
