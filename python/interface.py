from variables import *
from cpp_work import *
from data_work import *
from gui_work import *


def run(event=None):
    global SIZES, LENGTH, VARS, TO_DEGREE
    put_data(SIZES, LENGTH, VARS, TO_DEGREE)
    run_cpp()
    global FIG, PLOTS, PARAMS, LIMS
    plot_data_set(FIG, PLOTS, get_plot_data(PARAMS, LIMS))


def update_vars(val=None):
    global VARS, SLIDERS
    for n, slider in enumerate(SLIDERS):
        VARS[n] = slider.val


def update_scale(val=None):
    global PARAMS, RADIO
    PARAMS['scale'] = RADIO.value_selected


def update_params(val=None):
    global PARAMS, CHECK
    PARAMS['lambda filter'] = CHECK.get_status()[0]


def update_lambda_filter(val=None):
    global PARAMS, TEXTBOX
    PARAMS['lmin'] = float(TEXTBOX[0].text_disp.get_text())
    PARAMS['lmax'] = float(TEXTBOX[1].text_disp.get_text())


compile()

FIG = fig_set()

PLOT_AXES = plot_axes_set(FIG)
SLIDER_AXES = slider_axes_set(FIG)
BUTTON_AXIS = button_axis_set(FIG)
RADIO_AXIS = radio_axis_set(FIG)
CHECK_AXIS = check_axis_set(FIG)
TEXTBOX_AXES = textbox_axes_set(FIG)

PLOTS = plots_set(PLOT_AXES, PLOT_LIMS)
SLIDERS = sliders_set(SLIDER_AXES, LABELS, VARS, BORDERS, update_vars)
BUTTON = button_set(BUTTON_AXIS, "Run", run)
RADIO = radio_set(RADIO_AXIS, ("lin", "sqrt", "log"), update_scale)
CHECK = check_set(CHECK_AXIS, ("lambda filter",), update_params)
TEXTBOX = textboxs_set(TEXTBOX_AXES, ["l min", "l max"], [
    0.0, 10.0], update_lambda_filter)

plt.show()
