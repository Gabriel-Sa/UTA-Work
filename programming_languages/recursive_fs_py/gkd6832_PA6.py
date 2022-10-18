""" Gabriel de Sa
UTA ID: 1001676832
Lab 6 Recursive File Size April 30 2020
Python 3.7.7 Mac OS Catalina 10.15.4"""
import os
import os.path


def findDirSize(dir):
    """Gets the total size of the directory recursively."""
    files = os.listdir(dir)                             # creates a list of files
    size = 0                                            # starts size for each iteration
    for file in files:                                  # iterates through the list of files
        withPath = os.path.join(dir, file)              # joins that paths together
        if os.path.isdir(withPath):                     # test to see if this is a directory
            size = size + findDirSize(withPath)         # call and add recursively
        elif os.path.isfile(withPath):                  # checks to see if file
            size = size + os.path.getsize(withPath)     # get file size and add to var
    return size


def main():
    directory = os.getcwd()                             # Get current directory
    totalSize = findDirSize(directory)                  # get size
    print("Total size is {} bytes".format(totalSize))   # print with format
    return 0


if __name__ == '__main__':
    main()
