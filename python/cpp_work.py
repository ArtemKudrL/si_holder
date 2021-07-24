import os
from struct import pack, unpack
import numpy as np

INPUT_DIRECTORY = "./../data/in/"
OUTPUT_FILE = "./../data/out/setup.dat"


def compile():
    os.system("g++ -std=c++17 ./../src/main.cpp ./../src/func.cpp ./../src/vector.cpp ./../src/quaternion.cpp -I./../include -o ./../bin/main")


def run_cpp():
    os.system("./../bin/main")


def put_data(SIZES, VALS, TO_DEGREE):
    file = open(OUTPUT_FILE, "wb")

    for size in SIZES:
        file.write(pack('i', size))

    for n, val in enumerate(VALS):
        if TO_DEGREE[n]:
            file.write(pack('d', np.deg2rad(val)))
        else:
            file.write(pack('d', val))

    file.close()


def get_data(filename):
    Data = np.fromfile(INPUT_DIRECTORY + filename, dtype=float)

    return Data.reshape((len(Data) // 4, 4))
