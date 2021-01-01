import matplotlib.pyplot as plt
import numpy as np
import scipy.stats as ss

fig, ax = plt.subplots()

ax.set(xlabel='time [hod]', ylabel='λ',
       title='About as simple as it gets, folks')
ax.grid()


def plot_exponential(x_range, mu=0, sigma=1, **kwargs):
    """
    Plots the exponential distribution function for a given x range
    If mu and sigma are not provided, standard exponential is plotted
    If cdf=True cumulative distribution is plotted
    Passes any keyword arguments to matplotlib plot function
    """
    x = x_range
    y = ss.expon.pdf(x, mu, sigma)
    ax.plot(x, y, **kwargs)


x = np.linspace(0, 5, 5000)

plot_exponential(x, 0, 1 / (6 * 0.05), color='red', label='výpočetní výkon 5%', linewidth=2)
plot_exponential(x, 0, 1 / (6 * 0.15), color='blue', label='výpočetní výkon 15%', linewidth=2)
plot_exponential(x, 0, 1 / (6 * 0.3), color='green', label='výpočetní výkon 30%', linewidth=2)

plt.legend()

fig.savefig('test.png')

plt.show()
