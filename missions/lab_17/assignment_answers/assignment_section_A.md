Assignment Section A
====================

## Annealing Schedule
The cooling factor is the factor k applied to the Boltzman probability in the simulated annealing. By turning up the cooling factor you increase the probability of accepting a 'jump' to a higher energy state, followingly by turning it down you decrease the probability of accepting such a 'jump'. This results in a parameter that allows us to tune how sensitive our system is to leave local solutions and must be choosen carefully to achieve best possible results.

Alpha denotes the propagation decay for the ocean front. By increasing the search interval for the parameter alpha we see large fluctuations in performance. After running the same simulation multiple times it is clear that the parameter does not tend to its true value. This comes as a consequence of alpha being a very hard parameter to estimate, especially with only vehicle to perform estimation and the lawnmower pattern for surveying. 

By setting the concurrent parameter false we ensure that the annealer is run after all measurements are collected. After running multiple simulations it becomes clear that setting the concurrent parameter false benefits the parameter estimation, with a caveat of using more time for estimating. However, overall it seems to yield a higher score when comparing to running with concurrent set true. Thus this feature can be used to save time, but must be handled with care when using it with low cooling factors to avoid parameters getting 'stuck' before reaching areas providing good information.

## Parameter Sensitivity


## Dynamic Front


## Survey Pattern
 
