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
      26.443           0       18620       18620      100.00                        
      26.444         110       18620       18510       99.41                        
      29.321         160       18620       18460       99.14                        
      32.288         260       18620       18360       98.60                        
      35.042         370       18620       18250       98.01                        
      37.831         480       18620       18140       97.42                        
      40.557         570       18620       18050       96.94                        
      43.566         650       18620       17970       96.51                        
      46.431         790       18620       17830       95.76                        
      49.007         920       18620       17700       95.06                        
      51.808        1070       18620       17550       94.25                        
      54.393        1240       18620       17380       93.34                        
      56.880        1300       18620       17320       93.02                        
      59.287        1470       18620       17150       92.11                        
      61.795        1570       18620       17050       91.57                        
      63.862        1710       18620       16910       90.82                        
      66.075        1850       18620       16770       90.06                        
      68.075        1960       18620       16660       89.47                        
      70.054        2040       18620       16580       89.04                        
      72.060        2090       18620       16530       88.78                        
      73.972        2170       18620       16450       88.35                        
      75.984        2290       18620       16330       87.70                        
      78.105        2410       18620       16210       87.06                        
      79.722        2480       18620       16140       86.68                        
      81.394        2580       18620       16040       86.14                        
      82.933        2670       18620       15950       85.66                        
      84.684        2790       18620       15830       85.02                        
      86.322        2900       18620       15720       84.43                        
      87.926        3090       18620       15530       83.40                        
      89.528        3280       18620       15340       82.38                        
      90.883        3410       18620       15210       81.69                        
      92.285        3550       18620       15070       80.93                        
      93.776        3710       18620       14910       80.08                        
      94.908        3800       18620       14820       79.59                        
      96.170        3940       18620       14680       78.84                        
      97.358        4050       18620       14570       78.25                        
      98.693        4180       18620       14440       77.55                        
      99.769        4280       18620       14340       77.01                        
     100.772        4420       18620       14200       76.26                        
     101.841        4510       18620       14110       75.78                        
     102.902        4660       18620       13960       74.97                        
     103.911        4820       18620       13800       74.11                        
     104.937        4960       18620       13660       73.36                        
     105.795        5040       18620       13580       72.93                        
     106.623        5190       18620       13430       72.13                        
     107.491        5280       18620       13340       71.64                        
     108.393        5440       18620       13180       70.78                        
     109.246        5550       18620       13070       70.19                        
     110.016        5650       18620       12970       69.66                        
     110.790        5770       18620       12850       69.01                        
     111.564        5890       18620       12730       68.37                        
     112.357        6000       18620       12620       67.78                        
     113.193        6100       18620       12520       67.24                        
     113.850        6180       18620       12440       66.81                        
     114.516        6330       18620       12290       66.00                        
     115.187        6440       18620       12180       65.41                        
     115.736        6510       18620       12110       65.04                        
     116.321        6600       18620       12020       64.55                        
     116.910        6740       18620       11880       63.80                        
     117.536        6860       18620       11760       63.16                        
     118.071        6940       18620       11680       62.73                        
     118.719        7040       18620       11580       62.19                        
     119.227        7180       18620       11440       61.44                        
     119.773        7300       18620       11320       60.79                        
     120.212        7440       18620       11180       60.04                        
     120.662        7500       18620       11120       59.72                        
     121.102        7630       18620       10990       59.02                        
     121.523        7750       18620       10870       58.38                        
     121.907        7880       18620       10740       57.68                        
     122.302        8020       18620       10600       56.93                        
     122.754        8170       18620       10450       56.12                        
     123.056        8360       18620       10260       55.10                        
     123.364        8490       18620       10130       54.40                        
     123.621        8620       18620       10000       53.71                        
     123.901        8750       18620        9870       53.01                        
     124.176        8920       18620        9700       52.09                        
     124.453        9010       18620        9610       51.61                        
     124.720        9190       18620        9430       50.64                        
     124.944        9340       18620        9280       49.84                        
     125.216        9540       18620        9080       48.76                        
     125.410        9670       18620        8950       48.07                        
     125.622        9790       18620        8830       47.42                        
     125.832        9930       18620        8690       46.67                        
     126.036       10100       18620        8520       45.76                        
     126.201       10240       18620        8380       45.01                        
     126.409       10410       18620        8210       44.09                        
     126.584       10560       18620        8060       43.29                        
     126.704       10670       18620        7950       42.70                        
     126.867       10840       18620        7780       41.78                        
     127.010       10930       18620        7690       41.30                        
     127.205       11070       18620        7550       40.55                        
     127.362       11220       18620        7400       39.74                        
     127.552       11400       18620        7220       38.78                        
     127.707       11540       18620        7080       38.02                        
     127.881       11720       18620        6900       37.06                        
     127.993       11910       18620        6710       36.04                        
     128.120       12020       18620        6600       35.45                        
     128.201       12250       18620        6370       34.21                        
     128.300       12330       18620        6290       33.78                        
     128.381       12540       18620        6080       32.65                        
     128.502       12690       18620        5930       31.85                        
     128.566       12880       18620        5740       30.83                        
     128.644       13080       18620        5540       29.75                        
     128.731       13250       18620        5370       28.84                        
     128.814       13360       18620        5260       28.25                        
     128.896       13510       18620        5110       27.44                        
     128.954       13640       18620        4980       26.75                        
     128.995       13830       18620        4790       25.73                        
     129.064       14000       18620        4620       24.81                        
     129.133       14140       18620        4480       24.06                        
     129.177       14380       18620        4240       22.77                        
     129.246       14530       18620        4090       21.97                        
     129.295       14710       18620        3910       21.00                        
     129.360       14840       18620        3780       20.30                        
     129.391       15020       18620        3600       19.33                        
     129.436       15230       18620        3390       18.21                        
     129.471       15400       18620        3220       17.29                        
     129.494       15570       18620        3050       16.38                        
     129.510       15720       18620        2900       15.57                        
     129.527       15920       18620        2700       14.50                        
     129.546       16050       18620        2570       13.80                        
     129.556       16240       18620        2380       12.78                        
     129.568       16370       18620        2250       12.08                        
     129.576       16530       18620        2090       11.22                        
     129.587       16670       18620        1950       10.47                        
     129.597       16810       18620        1810        9.72                        
     129.607       16930       18620        1690        9.08                        
     129.616       17060       18620        1560        8.38                        
     129.624       17170       18620        1450        7.79                        
     129.631       17350       18620        1270        6.82                        
     129.637       17500       18620        1120        6.02                        
     129.645       17650       18620         970        5.21                        
     129.651       17750       18620         870        4.67                        
     129.656       17930       18620         690        3.71                        
     129.665       18090       18620         530        2.85                        
     129.672       18300       18620         320        1.72                        
     129.677       18440       18620         180        0.97                        
     129.681       18480       18620         140        0.75                        
     129.685       18540       18620          80        0.43                        
     129.689       18620       18620           0        0.00                        

Final statistics
----------------
Value:                        18620
Bound:                        18620
Absolute optimality gap:      0
Relative optimality gap (%):  0
Time (s):                     129.693

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
