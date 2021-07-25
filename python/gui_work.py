import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, RadioButtons, CheckButtons, TextBox
import numpy as np


def fig_set():
    # plt.style.use('dark_background')
    figure = plt.figure(figsize=(16, 9),  # facecolor='#2B3133', edgecolor='#D19A66',
                        num="Powder diffractometer monocrystall X-ray diffraction modeling GUI")

    return figure


def plot_axes_set(figure):
    grid = plt.GridSpec(2, 4, left=0.035, bottom=0.425,
                        right=0.965, top=0.965, figure=figure)

    result = []
    for row in range(2):
        temp = []
        for col in range(4):
            temp.append(
                figure.add_subplot(grid[row, col]))
        result.append(temp)

    return result


def slider_axes_set(figure):
    result = []
    for row in range(8):
        result.append(
            figure.add_axes([0.050, 0.35 - 0.04 * row, 0.3, 0.02]))

    return result


def button_axis_set(figure):
    return figure.add_axes([0.050, 0.02, 0.05, 0.03])


def radio_axis_set(figure):
    return figure.add_axes([0.40, 0.2, 0.10, 0.17])


def check_axis_set(figure):
    return figure.add_axes([0.52, 0.03, 0.10, 0.34])


def textbox_axes_set(figure):
    return [figure.add_axes([0.43, 0.13, 0.07, 0.04]),
            figure.add_axes([0.43, 0.07, 0.07, 0.04])]


def plots_set(axes, lims):
    result = []
    for n, row in enumerate(axes):
        temp = []
        for col in row:
            Z = np.random.rand(10, 10)
            temp.append(col.imshow(Z, cmap='viridis',
                                   aspect='auto',
                                   extent=lims[n]))
        result.append(temp)

    return result


def sliders_set(axes, labels, inits, borders, action):
    result = []
    for n, ax in enumerate(axes):
        result.append(Slider(ax, labels[n], valinit=inits[n], valmin=borders[n]
                      [0], valstep=borders[n][1], valmax=borders[n][2]))
        result[n].on_changed(action)

    return result


def button_set(axis, label, action):
    result = Button(axis, label)
    result.on_clicked(action)

    return result


def radio_set(axis, labels, action):
    result = RadioButtons(axis, labels, active=0)
    result.on_clicked(action)

    return result


def check_set(axis, labels, action):
    result = CheckButtons(axis, labels, actives=None)
    result.on_clicked(action)

    return result


def textboxs_set(axes, labels, inits, action):
    result = []
    for n, ax in enumerate(axes):
        result.append(TextBox(ax, labels[n], initial=inits[n]))
        result[n].on_submit(action)

    return result


def plot_data_set(figure, plots, plot_data):
    for row, plot_row in enumerate(plots):
        for col, plot in enumerate(plot_row):
            data = plot_data[row][col][::-1]
            plot.set_array(data)
            plot.set_clim(vmin=np.min(data), vmax=np.max(data))

    figure.canvas.draw_idle()
