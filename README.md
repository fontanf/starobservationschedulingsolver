# StarObservationSchedulingSolver

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
* Dynamic programming `-a dynamic-programming`

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
* Column generation heuristic implemented with [fontanf/columngenerationsolver](https://github.com/fontanf/columngenerationsolver) `-a column-generation`
* Benders decomposition `-a benders-decomposition`

## Flexible star observation scheduling problems

In these variants, the duration of an observation is flexible. The maximum duration provides the best image quality, but the observation time can be reduced leading to an image of lower quality. It might be worth if then more observations can be scheduled.

For each possible observation, instead of a single observation time and profit, a list of pairs of observation time and profit is given.

### Flexible single-night star observation scheduling problem

Implemented algorithms:
* Dynamic programming `-a dynamic-programming`

### Flexible star observation scheduling problem

Implemented algorithms:
* Column generation heuristic implemented with [fontanf/columngenerationsolver](https://github.com/fontanf/columngenerationsolver) `-a column-generation`

![scheduleflexibleexample](img/schedule_flexible_example.png?raw=true "Flexible schedule example")

## Usage (command line)

Compile:
```shell
bazel build -- //...
```

Download data:
```shell
python3 scripts/download_data.py
```

Examples:

```shell
./bazel-bin/starobservationschedulingsolver/starobservationscheduling/main -v 1 -i ./data/starobservationscheduling/catusse2016/real.txt -a column-generation -c solution.txt
```
```
=======================================
    StarObservationSchedulingSolver    
=======================================

Problem
-------
Star observation scheduling problem

Instance
--------
Number of nights:       142
Number of targets:      800
Number of observables:  42929

Algorithm
---------
Column generation heuristic - greedy

Parameters
----------
Time limit:            inf
Messages
    Verbosity level:   1
    Standard output:   1
    File path:         
    # streams:         0
Logger
    Has logger:        0
    Standard error:    0
    File path:         

    Time (s)       Value       Bound         Gap     Gap (%)                 Comment
    --------       -----       -----         ---     -------                 -------
       0.000           0       20000       20000      100.00                        
      11.514           0       18620       18620      100.00                        
      11.515        4530       18620       14090       75.67                        
      12.511        4720       18620       13900       74.65                        
      15.522        5210       18620       13410       72.02                        
      17.972        5320       18620       13300       71.43                        
      20.389        5510       18620       13110       70.41                        
      32.015        6180       18620       12440       66.81                        
      33.000        6600       18620       12020       64.55                        
      36.976        6770       18620       11850       63.64                        
      37.732        7340       18620       11280       60.58                        
      38.249        8110       18620       10510       56.44                        
      41.658        8540       18620       10080       54.14                        
      43.536        8770       18620        9850       52.90                        
      43.782        8790       18620        9830       52.79                        
      44.027        9700       18620        8920       47.91                        
      44.327        9860       18620        8760       47.05                        
      44.722       10170       18620        8450       45.38                        
      45.696       10530       18620        8090       43.45                        
      46.446       10920       18620        7700       41.35                        
      46.923       11120       18620        7500       40.28                        
      48.092       11370       18620        7250       38.94                        
      48.334       11470       18620        7150       38.40                        
      48.501       11830       18620        6790       36.47                        
      48.825       11970       18620        6650       35.71                        
      48.918       12240       18620        6380       34.26                        
      50.015       12310       18620        6310       33.89                        
      50.360       12700       18620        5920       31.79                        
      50.856       12710       18620        5910       31.74                        
      50.990       13280       18620        5340       28.68                        
      51.179       13320       18620        5300       28.46                        
      51.228       13580       18620        5040       27.07                        
      51.465       13740       18620        4880       26.21                        
      51.573       13790       18620        4830       25.94                        
      51.693       14130       18620        4490       24.11                        
      51.730       14260       18620        4360       23.42                        
      51.913       15060       18620        3560       19.12                        
      52.058       15150       18620        3470       18.64                        
      52.173       15770       18620        2850       15.31                        
      52.209       15780       18620        2840       15.25                        
      52.305       15970       18620        2650       14.23                        
      52.381       16130       18620        2490       13.37                        
      52.420       16410       18620        2210       11.87                        
      52.477       17250       18620        1370        7.36                        
      52.537       17340       18620        1280        6.87                        
      52.552       17510       18620        1110        5.96                        
      52.567       17630       18620         990        5.32                        
      52.578       17820       18620         800        4.30                        
      52.587       17930       18620         690        3.71                        
      52.596       18220       18620         400        2.15                        
      52.602       18620       18620           0        0.00                        

Final statistics
----------------
Value:                        18620
Bound:                        18620
Absolute optimality gap:      0
Relative optimality gap (%):  0
Time (s):                     52.6039

Solution
--------
Number of targets:  702 / 800 (87.75%)
Feasible:           1
Profit:             18620 / 20000 (93.1%)
```

Visualize:
```
python3 starobservationschedulingsolver/starobservationscheduling/visualizer.py solution.txt
```
