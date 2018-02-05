# uncertain-infmax
Implementation of paper [Stability of Influence Maximization](http://arxiv.org/abs/1501.04579)

## Usage
The main algorithms are implemented with c++ and built with Bazel. The experiments can be run with the following steps.

1. Install [bazel](https://docs.bazel.build/versions/master/install.html)
2. Build code by running ```bazel build :all```. The code uses openmp. Make sure it is enabled before building the code.
3. Create two folders ```result``` and ```log```.
3. Run ICDet experiment with ```./bazel-bin/uncertainty_infmax_exp ICDet GraphName```. Graph is one of the graphs in the ```graph/``` foleder.
4. Run Reg experiment on regular graph with ```./bazel-bin/uncertainty_infmax_exp ICDet Degree```. The dergee d must correpond to a graph with name ```reg_d.graph``` in the graph folder.


## Graph Format
The format of the input graph is as follows: 

1. The first line contains two integer n and m seperated by space as the number of nodes and number of edges. The graph is treated as a directed graph. 
2. Each of the following m lines contains three integers seperated by space as the start node id of the edge, end node if of the id and the  multiplicity of the edge. 

## Parse and Visualize Experiment Result
1. Runnign the experiment generates a ```GraphName.result``` file in the result folder.
2. Process the result file to csv: ```python result_process/result_parser.py EXP_NAME```