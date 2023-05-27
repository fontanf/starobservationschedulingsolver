# Star observation scheduling solver

A solver for star observation scheduling problems.

![verylargetelescope](img/very_large_telescope.jpg?raw=true "Very Large Telescope")

[image source](https://commons.wikimedia.org/wiki/File:Paranal_and_the_Pacific_at_sunset_(dsc4088,_retouched,_cropped).jpg)

## Single-night star observation scheduling problem

This is the most fundamental variant of the problem. It consists in selecting a subset of targets to observe during a given night.

Problem description:
* Input:
  * `n` targets with profit `wⱼ`, time-window `[rⱼ, dⱼ]` and duration `pⱼ` such that `2 pⱼ ≥ dⱼ - rⱼ` (`j = 1..n`)
* Problem: select a list of targets and their starting dates sⱼ such that:
  * A target is observed at most once
  * Observations do not overlap
  * Starting dates satisfy the time-windows, i.e. `rⱼ <= sⱼ` and `sⱼ + pⱼ <= dⱼ`
* Objective: maximize the overall profit of the selected targets

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
* Problem: select a list of observations and their starting dates sᵢⱼ such that:
  * A target is observed at most once
  * Observations do not overlap
  * Starting dates satisfy the time-windows, i.e. `rᵢⱼ <= sᵢⱼ` and `sᵢⱼ + pᵢⱼ <= dᵢⱼ`
* Objective: maximize the overall profit of the selected observations
