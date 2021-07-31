from linux_cpp_work import get_data
import numpy as np
from matplotlib import cm
from variables import VARS, PLOT_LIMS


def get_plot_data(params, lims):
    global HDATA, DDATA, DATA
    HDATA = get_data("h_out.dat")
    DDATA = get_data("d_out.dat")
    DATA = [HDATA, DDATA]

    rows = params['rows']
    cols = params['cols']

    result = np.ndarray((2, 4, rows, cols))
    mask = np.ndarray((2, 4, rows, cols))

    for row in range(2):
        for col in range(4):

            temp = [[[] for _ in range(cols)]
                    for _ in range(rows)]

            xmin = lims[row][2]
            xstep = (lims[row][3] - lims[row][2]) / rows
            ymin = lims[row][0]
            ystep = (lims[row][1] - lims[row][0]) / cols
            for n, data in enumerate(DATA[row]):
                i = int((data[2] - xmin) / xstep)
                j = int((data[3] - ymin) / ystep)

                if data[0] != 0.0 \
                    and 0 <= i < rows and 0 <= j < cols \
                        and ((not params['lambda filter']) or params['lmin'] <= data[1] <= params['lmax']) \
                        and ((not params['detector mask']) or (lims[1][2] <= DATA[1][n][2] <= lims[1][3] and lims[1][0] <= DATA[1][n][3] <= lims[1][1])):
                    if col == 0:
                        if params['scale'] == 'lin':
                            temp[i][j].append(data[0])
                        elif params['scale'] == 'sqrt':
                            temp[i][j].append(np.sqrt(data[0]))
                        elif params['scale'] == 'log':
                            temp[i][j].append(np.log10(data[0]))
                    elif col == 1:
                        temp[i][j].append(data[col])
                    elif col <= 3:
                        temp[i][j].append(DATA[1 - row][n][col])

            for i in range(rows):
                for j in range(cols):
                    vals = temp[i][j]
                    if len(vals) == 0:
                        mask[row, col, i, j] = 0.0
                        result[row, col, i, j] = 0.0
                    else:
                        mask[row, col, i, j] = 1.0
                        result[row, col, i, j] = sum(vals) / len(vals)

    global VARS, PLOT_LIMS
    PLOT_LIMS[1][2] = VARS[1] + VARS[2] - 8.58
    PLOT_LIMS[1][3] = VARS[1] + VARS[2] + 8.58
    result[0, 2] = VARS[1] + VARS[2] + \
        (17.16 * result[0, 2] / (lims[1][3] - lims[1][2]))

    return result, mask
