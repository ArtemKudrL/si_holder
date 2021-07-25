SIZES = [100, 100]

LENGTH = 200.0

TO_DEGREE = [False, True, True, False, True, True, True, True]

NAMES = ['S_W', 'S_TH', 'D_TH', 'H_Z', 'H_A', 'H_B', 'S_DS', 'S_DS']

LABELS = [r'$w, mm$',
          r'$\theta_s, deg$',
          r'$\theta_d, deg$',
          r'$h, mm$',
          r'$\alpha, deg$',
          r'$\beta, deg$',
          r'$DS, deg$',
          r'$SS, deg$']

VARS = [0.0, 15.0, 15.0, 0.0,
        0.0, 0.0, 0.5, 2.5]

BORDERS = [(-6.0, 0.5,  6.0),
           (0.0, 0.5, 80.0),
           (0.0, 0.5, 80.0),
           (-5.0, 0.1, 3.0),
           (0.0, 0.5, 80.0),
           (0.0, 1.0, 360.0),
           (0.05, 0.01, 1.5),
           (0.05, 0.01, 5.0)]

LIMS = [(-10.0, 10.0,
         -10.0, 10.0),

        (-4.0, 4.0,
         -30.0, 30.0)]

PLOT_LIMS = [(-10.0, 10.0,
              -10.0, 10.0),

             (-4.0, 4.0,
              0, 1200)]

PARAMS = {'scale': 'lin',
          'lambda filter': False,
          'rows': 100,
          'cols': 100,
          'lmin': 0.0,
          'lmax': 10.0}
