import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, RadioButtons
from struct import pack, unpack


def get_data():
    Data = np.fromfile("out.dat")
    NUM = D_XNUM * D_YNUM

    return np.reshape(Data[0:NUM], (D_XNUM, D_YNUM)), \
        np.reshape(Data[NUM:2*NUM], (D_XNUM, D_YNUM)), \
        np.reshape(Data[2*NUM:3*NUM], (D_XNUM, D_YNUM)), \
        np.reshape(Data[3*NUM:4*NUM], (D_XNUM, D_YNUM))


def compile():
    os.system("g++ main.cpp func.cpp vector.cpp quaternion.cpp -o main")


def run_cpp():
    os.system("./main")


def put_data():
    file = open("setup.dat", "wb")
    file.write(pack('i', COLS))
    file.write(pack('i', ROWS))
    file.write(pack('d', LENGTH))
    file.write(pack('d', S_W))
    file.write(pack('d', np.deg2rad(S_TH)))
    file.write(pack('d', np.deg2rad(D_TH)))
    file.write(pack('d', H_Z))
    file.write(pack('d', np.deg2rad(H_A)))
    file.write(pack('d', np.deg2rad(H_B)))
    file.write(pack('d', np.deg2rad(S_DS)))
    file.write(pack('d', np.deg2rad(S_SS)))
    file.write(pack('d', D_XMIN))
    file.write(pack('d', D_XMAX))
    file.write(pack('i', D_XNUM))
    file.write(pack('d', D_YMIN))
    file.write(pack('d', D_YMAX))
    file.write(pack('i', D_YNUM))
    file.close()


compile()

COLS = 100
ROWS = 100
LENGTH = 200.0

S_W = 0.0
S_W_borders = (-6.0, 0.5,  6.0)
S_TH = 15.0
S_TH_borders = (0.0, 0.5, 80.0)
D_TH = 15.0
D_TH_borders = (0.0, 0.5, 80.0)
H_Z = 0.0
H_Z_borders = (-5.0, 0.1, 3.0)
H_A = 0.0
H_A_borders = (0.0, 0.5, 80.0)
H_B = 0.0
H_B_borders = (0.0, 1.0, 360.0)
S_DS = 0.5
S_DS_borders = (0.05, 0.01, 1.5)
S_SS = 2.5
S_SS_borders = (0.05, 0.01, 5.0)

D_XMIN = -30.0
D_XMAX = 30.0
D_XNUM = 150
D_YMIN = -4.0
D_YMAX = 4.0
D_YNUM = 40

TO_DRAW = 'I'

put_data()
run_cpp()
I, L, HX, HY = get_data()

fig, ax = plt.subplots()
plt.subplots_adjust(left=0.7, bottom=0.1)
im = plt.imshow(I[::-1])
ax.margins(x=0)

axcolor = 'lightgoldenrodyellow'
axS_W = plt.axes([0.2, 0.9, 0.4, 0.03], facecolor=axcolor)
axS_TH = plt.axes([0.2, 0.85, 0.4, 0.03], facecolor=axcolor)
axD_TH = plt.axes([0.2, 0.8, 0.4, 0.03], facecolor=axcolor)
axH_Z = plt.axes([0.2, 0.75, 0.4, 0.03], facecolor=axcolor)
axH_A = plt.axes([0.2, 0.7, 0.4, 0.03], facecolor=axcolor)
axH_B = plt.axes([0.2, 0.65, 0.4, 0.03], facecolor=axcolor)
axS_DS = plt.axes([0.2, 0.6, 0.4, 0.03], facecolor=axcolor)
axS_SS = plt.axes([0.2, 0.55, 0.4, 0.03], facecolor=axcolor)

sS_W = Slider(axS_W, r'$w, mm$',
              S_W_borders[0], S_W_borders[2],
              valinit=S_W, valstep=S_W_borders[1])
sS_TH = Slider(axS_TH, r'$\theta_s, deg$',
               S_TH_borders[0], S_TH_borders[2],
               valinit=S_TH, valstep=S_TH_borders[1])
sD_TH = Slider(axD_TH, r'$\theta_d, deg$',
               D_TH_borders[0], D_TH_borders[2],
               valinit=D_TH, valstep=D_TH_borders[1])
sH_Z = Slider(axH_Z, r'$h, mm$',
              H_Z_borders[0], H_Z_borders[2],
              valinit=H_Z, valstep=H_Z_borders[1])
sH_A = Slider(axH_A, r'$\alpha, deg$',
              H_A_borders[0], H_A_borders[2],
              valinit=H_A, valstep=H_A_borders[1])
sH_B = Slider(axH_B, r'$\beta, deg$',
              H_B_borders[0], H_B_borders[2],
              valinit=H_B, valstep=H_B_borders[1])
sS_DS = Slider(axS_DS, r'$DS, deg$',
               S_DS_borders[0], S_DS_borders[2],
               valinit=S_DS, valstep=S_DS_borders[1])
sS_SS = Slider(axS_SS, r'$SS, deg$',
               S_SS_borders[0], S_SS_borders[2],
               valinit=S_SS, valstep=S_SS_borders[1])


def updateS_W(val):
    global S_W
    S_W = val


def updateS_TH(val):
    global S_TH
    S_TH = val


def updateD_TH(val):
    global D_TH
    D_TH = val


def updateH_Z(val):
    global H_Z
    H_Z = val


def updateH_A(val):
    global H_A
    H_A = val


def updateH_B(val):
    global H_B
    H_B = val


def updateS_DS(val):
    global S_DS
    S_DS = val


def updateS_SS(val):
    global S_SS
    S_SS = val


sS_W.on_changed(updateS_W)
sS_TH.on_changed(updateS_TH)
sD_TH.on_changed(updateD_TH)
sH_Z.on_changed(updateH_Z)
sH_A.on_changed(updateH_A)
sH_B.on_changed(updateH_B)
sS_DS.on_changed(updateS_DS)
sS_SS.on_changed(updateS_SS)

axRun = plt.axes([0.8, 0.025, 0.1, 0.04])
button = Button(axRun, 'Run', color=axcolor, hovercolor='0.975')


def run(event):
    put_data()
    run_cpp()
    global I, L, HX, HY
    I, L, HX, HY = get_data()
    if TO_DRAW == 'I':
        im.set_data(I[::-1])
    elif TO_DRAW == 'L':
        im.set_data(L[::-1])
    elif TO_DRAW == 'HX':
        im.set_data(HX[::-1])
    elif TO_DRAW == 'HY':
        im.set_data(HY[::-1])
    fig.canvas.draw_idle()


button.on_clicked(run)

axRadio = plt.axes([0.2, 0.3, 0.2, 0.2], facecolor=axcolor)
radio = RadioButtons(axRadio, ('I', 'L', 'HX', 'HY'), active=0)


def updateRadio(label):
    global TO_DRAW
    TO_DRAW = label
    if TO_DRAW == 'I':
        im.set_data(I[::-1])
    elif TO_DRAW == 'L':
        im.set_data(L[::-1])
    elif TO_DRAW == 'HX':
        im.set_data(HX[::-1])
    elif TO_DRAW == 'HY':
        im.set_data(HY[::-1])
    fig.canvas.draw_idle()


radio.on_clicked(updateRadio)

plt.show()
