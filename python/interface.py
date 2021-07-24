from varibles import *
from cpp_work import *
from data_work import *
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, RadioButtons, TextBox


FIG, SUBPLOTS_AXES = plt.subplots(2, 4, sharex=False)

for n, Hax in enumerate(axes[0]):
    Hax.margins(0.0)
    Hax.set_xlim(xmin=LIMS[0][0], xmax=LIMS[0][1])
    Hax.set_ylim(ymin=LIMS[1][0], ymay=LIMS[1][1])
    Hax.set_facecolor("grey")
    temp = Hax.scatter(HData[:, LV, 2], HData[:, LV, 3],
                       c=HData[:, LV, n], s=5)
    Hplots.append(temp)
for n, Dax in enumerate(axes[1]):
    Dax.margins(0.0)
    Dax.set_xlim(xmin=LIMS[2][0], xmax=LIMS[2][1])
    Dax.set_ylim(ymin=LIMS[3][0], ymay=LIMS[3][1])
    Dax.set_facecolor("grey")
    temp = Dax.scatter(DData[:, LV, 2], DData[:, LV, 3],
                       c=DData[:, LV, n], s=5)
    Dplots.append(temp)

axcolor = 'lightgoldenrodyellow'
SAXES = []
for n in range(len(NAMES)):
    SAXES.append(plt.axes(
        [0.05, 0.45 - 0.05 * n, 0.4, 0.03], facecolor=axcolor))

SLIDERS = []
for n in range(len(SAXES)):
    SLIDERS.append(Slider(SAXES[n], LABELS[n],
                          BORDERS[n][0], BORDERS[2],
                          valinit=VALS[n], valstep=BORDERS[1]))


def updateVals(val=None):
    global SLIDERS, VALS
    for n in range(len(SLIDERS)):
        VALS[n] = SLIDERS.val


for silder in SLIDERS:
    slider.on_changed(updateVals)

axRun = plt.axes([0.2, 0.05, 0.1, 0.04])
button = Button(axRun, 'Run', color=axcolor, hovercolor='0.975')


def run(event):
    put_data()
    run_cpp()
    global HData, DData
    HData = get_data("h_out.dat")
    DData = get_data("d_out.dat")
    for n, plot in enumerate(Hplots):
        plot.set_offsets(HData[:, 1, 2:4])
        plot.set_array(HData[:, 1, n])
    for n, plot in enumerate(Dplots):
        plot.set_offsets(DData[:, 1, 2:4])
        plot.set_array(DData[:, 1, n])
    fig.canvas.draw_idle()


button.on_clicked(run)


def key_press(event):
    if event.key == 'right':


FIG.canvas.mpl_connect('key_press_event', key_press)

mng = plt.get_current_fig_manager()
mng.window.showMaximized()
plt.show()
