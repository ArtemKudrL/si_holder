import os
from struct import pack, unpack
from numpy import fromfile, deg2rad

INPUT_DIRECTORY = "./../data/out/"
OUTPUT_FILE = "./../data/in/setup.dat"


def compile():
    os.system("g++ -std=c++17 ./../src/main.cpp ./../src/func.cpp ./../src/vector.cpp ./../src/quaternion.cpp -I./../include -o ./../bin/main")


def run_cpp():
    os.system("./../bin/main")


def put_data(SIZES, LENGTH, VARS, TO_DEGREE):
    file = open(OUTPUT_FILE, "wb")

    for size in SIZES:
        file.write(pack('i', size))

    file.write(pack('d', LENGTH))

    for n, var in enumerate(VARS):
        if TO_DEGREE[n]:
            file.write(pack('d', deg2rad(var)))
        else:
            file.write(pack('d', var))

    file.close()


def get_data(filename):
    Data = fromfile(INPUT_DIRECTORY + filename, dtype=float)

    return Data.reshape((len(Data) // 4, 4))
