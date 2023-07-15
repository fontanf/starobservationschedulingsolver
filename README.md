# Star observation scheduling solver

A solver for star observation scheduling problems.

These are problems I studied during my PhD, which was a collaboration between the [G-SCOP laboratory](https://g-scop.grenoble-inp.fr/en) and the [Institute for Planetary sciences and Astrophysics from Grenoble (IPAG)](https://ipag.osug.fr/?lang=en). The study was motivated by the scheduling problems solved daily by the astrophysicists to schedule their observations on the [Very Large Telescope](https://en.wikipedia.org/wiki/Very_Large_Telescope).

![verylargetelescope](img/very_large_telescope.jpg?raw=true "Very Large Telescope")

[image source](https://commons.wikimedia.org/wiki/File:Paranal_and_the_Pacific_at_sunset_(dsc4088,_retouched,_cropped).jpg)

![scheduleexample](img/schedule_example.png?raw=true "Schedule example")

## Single-night star observation scheduling problem

This is the most fundamental variant of the problem. It consists in selecting a subset of targets to observe during a given night.

Problem description:
* Input:
  * `n` targets with profit `wⱼ`, time-window `[rⱼ, dⱼ]` and duration `pⱼ` such that `2 pⱼ ≥ dⱼ - rⱼ` (`j = 1..n`)
* Problem: select a list of targets and their starting dates `sⱼ` such that:
  * A target is observed at most once
  * Observations do not overlap
  * Starting dates satisfy the time-windows, i.e. `rⱼ <= sⱼ` and `sⱼ + pⱼ <= dⱼ`
* Objective: maximize the overall profit of the selected targets

Implemented algorithms:
* Dynamic programming `-a dynamic_programming`

## Star observation scheduling problem

This variant considers the problem for multiple nights. Compared to the single night variant:
* A target might be visible in multiple nights
* The time-windows and observation times of a same target in different nights might differ
* A target doesn't need to be observed in more than one night

Problem description:
* Input:
  * `m` nights
  * `n` targets; for each target `j = 1..n`, a profit `wⱼ`
  * A list of possible observations. An observation is associated to a night `i` and a target `j` and has a time-window `[rᵢⱼ, dᵢⱼ]` and a duration `pᵢⱼ` such that `2 pⱼᵢ ≥ dⱼᵢ - rⱼᵢ`
* Problem: select a list of observations and their starting dates `sᵢⱼ` such that:
  * A target is observed at most once
  * Observations do not overlap
  * Starting dates satisfy the time-windows, i.e. `rᵢⱼ <= sᵢⱼ` and `sᵢⱼ + pᵢⱼ <= dᵢⱼ`
* Objective: maximize the overall profit of the selected observations

Implemented algorithms:
* Column generation heuristic implemented with [fontanf/columngenerationsolver](https://github.com/fontanf/columngenerationsolver) `-a column_generation`
* Benders decomposition `-a benders_decomposition`

## Flexible star observation scheduling problems

In these variants, the duration of an observation is flexible. The maximum duration provides the best image quality, but the observation time can be reduced leading to an image of lower quality. It might be worth if then more observations can be scheduled.

For each possible observation, instead of a single observation time and profit, a list of pairs of observation time and profit is given.

### Flexible single-night star observation scheduling problem

Implemented algorithms:
* Dynamic programming `-a dynamic_programming`

### Flexible star observation scheduling problem

Implemented algorithms:
* Column generation heuristic implemented with [fontanf/columngenerationsolver](https://github.com/fontanf/columngenerationsolver) `-a column_generation`

![scheduleflexibleexample](img/schedule_flexible_example.png?raw=true "Flexible schedule example")

## Usage (command line)

Compile:
```shell
bazel build -- //...
```

Examples:

```shell
./bazel-bin/starobservationschedulingsolver/main -v 1 -i ./data/starobservationscheduling/catusse2016/real.txt -a column_generation -c solution.txt
```
```
======================================
  Star observation scheduling solver  
======================================

Instance
--------
Number of nights:       142
Number of targets:      800
Number of observables:  42929

Algorithm
---------
Column Generation Heuristic - Greedy

Parameters
----------
Linear programming solver:  CLP

       T (s)              LB              UB             GAP     GAP (%)                 Comment
       -----              --              --             ---     -------                 -------
       0.000               0           20000           20000      100.00                        
     117.133               0           18620           18620      100.00                        
     117.133           18620           18620               0        0.00                        

Final statistics
----------------
Value:                        18620
Bound:                        18620
Absolute optimality gap:      0
Relative optimality gap (%):  0
Time (s):                     117.133

Solution
--------
Number of targets:  702 / 800 (87.75%)
Feasible:           1
Profit:             18620 / 20000 (93.1%)
```

Visualize:
```
python3 starobservationschedulingsolver/visualizer.py solution.txt
```

