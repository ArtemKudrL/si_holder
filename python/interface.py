from variables import *
from linux_cpp_work import *
from data_work import *
from gui_work import *
from lattice import gen_lattice


compile()


def run(event=None):
    global SIZES, LENGTH, VARS, TO_DEGREE, LV
    gen_lattice(LV)
    put_data(SIZES, LENGTH, VARS, TO_DEGREE)
    run_cpp()
    global FIG, PLOTS, PLOT_AXES, PARAMS, LIMS
    plot_data_set(FIG, PLOTS, get_plot_data(PARAMS, LIMS), PLOT_LIMS)


def keypress(event):
    global CURRENT_SLIDER, SLIDERS, RADIO, BORDERS, CHECK
    slider = SLIDERS[CURRENT_SLIDER]
    border = BORDERS[CURRENT_SLIDER]
    if event.key == 'c':
        compile()
    elif event.key == 'r':
        run()
    elif event.key == 'h':
        slider.set_val(slider.val - border[1])
        if slider.val < border[0]:
            slider.set_val(border[0])
    elif event.key == 'j':
        CURRENT_SLIDER += 1
        if CURRENT_SLIDER > len(SLIDERS):
            CURRENT_SLIDER = len(SLIDERS) - 1
    elif event.key == 'k':
        CURRENT_SLIDER -= 1
        if CURRENT_SLIDER < 0:
            CURRENT_SLIDER = 0
    elif event.key == 'l':
        slider.set_val(slider.val + border[1])
        if slider.val > border[2]:
            slider.set_val(border[2])
    elif event.key == 'f':
        PARAMS['lambda filter'] = not PARAMS['lambda filter']
        CHECK.set_active(0)
    elif event.key == 'm':
        PARAMS['detector mask'] = not PARAMS['detector mask']
        CHECK.set_active(1)
    elif event.key == 's':
        if PARAMS['scale'] == 'lin':
            PARAMS['scale'] = 'sqrt'
            RADIO.set_active(1)
        elif PARAMS['scale'] == 'sqrt':
            PARAMS['scale'] = 'log'
            RADIO.set_active(2)
        elif PARAMS['scale'] == 'log':
            PARAMS['scale'] = 'lin'
            RADIO.set_active(0)
    elif event.key == 'a':
        PARAMS['autorun'] = not PARAMS['autorun']
        CHECK.set_active(2)

    if PARAMS['autorun']:
        run()


def update_vars(val=None):
    global VARS, SLIDERS, PARAMS
    for n, slider in enumerate(SLIDERS):
        VARS[n] = slider.val
    if PARAMS['autorun']:
        run()


def update_scale(val=None):
    global PARAMS, RADIO
    PARAMS['scale'] = RADIO.value_selected
    if PARAMS['autorun']:
        run()


def update_params(val=None):
    global PARAMS, CHECK
    PARAMS['lambda filter'] = CHECK.get_status()[0]
    PARAMS['detector mask'] = CHECK.get_status()[1]
    PARAMS['autorun'] = CHECK.get_status()[2]
    if PARAMS['autorun']:
        run()


def update_textboxes(val=None):
    global PARAMS, TEXTBOX, LV
    PARAMS['lmin'] = float(TEXTBOX[0].text_disp.get_text())
    PARAMS['lmax'] = float(TEXTBOX[1].text_disp.get_text())
    LV = TEXTBOX[2].text_disp.get_text()
    if PARAMS['autorun']:
        run()


FIG = fig_set()

PLOT_AXES = plot_axes_set(FIG)
SLIDER_AXES = slider_axes_set(FIG)
RADIO_AXIS = radio_axis_set(FIG)
CHECK_AXIS = check_axis_set(FIG)
TEXTBOX_AXES = textbox_axes_set(FIG)

keyboard_action(FIG, keypress)

PLOTS = plots_set(PLOT_AXES, PLOT_LIMS)
SLIDERS = sliders_set(SLIDER_AXES, LABELS, VARS, BORDERS, update_vars)
RADIO = radio_set(RADIO_AXIS, ("lin", "sqrt", "log"), update_scale)
CHECK = check_set(CHECK_AXIS, (r"$\lambda filt$",
                  "det mask", "autorun"), update_params)
TEXTBOX = textboxs_set(TEXTBOX_AXES, [r"$\lambda_{min}$", r"$\lambda_{max}$", r"$LV$"], [
    0.0, 10.0, "1,1,1;"], update_textboxes)

plt.show()
