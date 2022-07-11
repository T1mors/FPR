# Headers start with #. So read this line and give the <header> vector to pd.read_csv(names=header)
import numpy as np

def get_header(filename):
    with open(filename, "r") as f:
        for line in f:
            if line.startswith("#"):
                header = line
            else:
                break  # stop when there are no more #

        header = header[1:].strip().split()
    return header


def na(array):
    return np.array(array)


def quadratic(x, a):
    y = a * x ** 2
    return y


def polynomiell(x, b, c):
    y = b * x + c * x ** 2
    return y


def exponential(x, a, b, c):
    y = a * np.exp(b * x) + c
    return y


def fehler_quadrat(N_model, N):
    sum_fehler2 = 0
    for i in np.arange(len(N)):
        sum_fehler2 = (N_model[i] - N[i]) ** 2
    return sum_fehler2
    # return np.sum(((N_model-N)**2)/N)


def RMSE(N_model, N):
    sum_fehler2 = 0
    for i in np.arange(len(N)):
        sum_fehler2 = (N_model[i] - N[i]) ** 2
    return np.sqrt(sum_fehler2 / len(N))


# For PC in university, if conda does not want to install a package, do it by hand
# conda install -n myEnv --use-local myPackageFromAnaconda.tar.bz2
# https://github.com/conda/conda/issues/7245

# Force install for pip
# /bin/python3 -m pip install ipykernel -U --user --force-reinstall
# /bin/python3.6 -m pip install numpy -U --user
