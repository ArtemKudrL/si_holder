from struct import pack
from math import sqrt


def gen_lattice(string):
    string = string.replace(" ", "").split(";")
    Fhkl = []
    for s in string:
        if len(s.split(",")) == 3:
            temp = []
            temp.append(1.0)
            for v in s.split(","):
                temp.append(float(v))
        elif len(s.split(",")) == 4:
            temp = [float(v) for v in s.split(",")]
        Fhkl.append(temp)

    a = 5.4310
    file = open("./../data/in/lattice.dat", "wb")
    file.write(pack('i', len(Fhkl)))
    for F, h, k, l in Fhkl:
        norm = sqrt(h**2 + k**2 + l**2)
        file.write(pack('d', h / norm))
        file.write(pack('d', k / norm))
        file.write(pack('d', l / norm))
        file.write(pack('d', a / norm))
        file.write(pack('d', F))
    file.close()
