Assignment Section A
====================

## Annealing Schedule
The cooling factor is the factor k applied to the Boltzman probability in the simulated annealing. By turning up the cooling factor you increase the probability of accepting a 'jump' to a higher energy state, followingly by turning it down you decrease the probability of accepting such a 'jump'. This results in a parameter that allows us to tune how sensitive our system is to leave local solutions and must be choosen carefully to achieve best possible results.

Alpha denotes the propagation decay for the ocean front. By increasing the search interval for the parameter alpha we see large fluctuations in performance. After running the same simulation multiple times it is clear that the parameter does not tend to its true value. This comes as a consequence of alpha being a very hard parameter to estimate, especially with only vehicle to perform estimation and the lawnmower pattern for surveying. 

By setting the concurrent parameter false we ensure that the annealer is run after all measurements are collected. After running multiple simulations it becomes clear that setting the concurrent parameter false benefits the parameter estimation, with a caveat of using more time for estimating. However, overall it seems to yield a higher score when comparing to running with concurrent set true. Thus this feature can be used to save time, but must be handled with care when using it with low cooling factors to avoid parameters getting 'stuck' before reaching areas providing good information.

## Parameter Sensitivity

#### Simulation results:

| Run | Error  | Score | Est. Wavelength | Wavelength |
| --- |:------:| -----:| ---------------:| ----------:|
| 1   | 0.011  | 8798  | 219.2           | 212.0      | 
| 2   | 0.009  | 10352 | 205.9           | 212.0      | 
| 3   | 0.003  | 32798 | 210.0           | 212.0      | 
| 4   | 0.019  | 5178  | 199.7           | 212.0      | 
| 5   | 0.009  | 11732 | 206.6           | 212.0      | 

## Dynamic Front

### Simulation results:

| Run | Error  | Score | Est. Wavelength | Wavelength | Est. Period | Period |
| --- |:------:| -----:| ---------------:| ----------:| -----------:| ------:|
| 1   | 0.105  | 946.5 | 145.2           | 212.0      | 206.5       | 200.0  |
| 2   | 0.049  | 2036  | 185.4           | 212.0      | 184.6       | 200.0  |
| 3   | 0.035  | 2841  | 230.8           | 212.0      | 211.5       | 200.0  |
| 4   | 0.021  | 4655  | 198.4           | 212.0      | 198.8       | 200.0  |
| 5   | 0.482  | 207.4 | 500.0           | 212.0      | 100.5       | 200.0  |

### Discussion
By analyzing the simulation results we observe a drop in wavelength estimation performance, when comparing the two parameter (wavelength and period) estimation case to the single (wavelength) estimation case. This is expected as estimating a single parameter is an easier task, and by introducing more unknown parameters one also increases the estimation difficulty. 

## Survey Pattern
 
