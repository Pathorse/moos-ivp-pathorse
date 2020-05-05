import numpy as np
import matplotlib.pyplot as plt
import pdb
# Data
error = np.array([0.011, 0.009, 0.003, 0.019, 0.009])
score = np.array([8798, 10352, 32798, 5178, 11732])
eps   = np.array([7.2, 6.1, 2, 12.3, 5.4])

# Curve fit to 2 order polynomial
z = np.polyfit(score, eps, 2)

# Convert to poly1d object
p = np.poly1d(z)

# Plot
plt.figure()
epsp = np.linspace(0, 33000, 100)
plt.plot(score, eps, 'b.', epsp, p(epsp), 'r-')
plt.xlabel('Score')
plt.ylabel('Estimated Wavelength error')
plt.title('Parameter Sensitivity')
plt.show()
