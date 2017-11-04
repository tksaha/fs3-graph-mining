# FSCube

Mining labeled subgraph is a popular research task in data mining because of
its potential application in many different scientific domains. All the
existing methods for this task explicitly or implicitly solve the subgraph
isomorphism task which is computationally expensive, so they suffer from the
lack of scalability problem when the graphs in the input database are large. In
this work, we propose FS^3, which is a sampling based method. It mines a small
collection of subgraphs that are most frequent in the probabilistic sense.
FS^3 performs a Markov chain Monte Carlo (MCMC) sampling over the space of a
fixed-size subgraphs such that the potentially frequent
subgraphs are sampled more often. Besides, FS^3 is equipped 
with an innovative queue manager. It stores the sampled subgraph 
in a finite queue over the course of mining in such
a manner that the top-k positions in the queue contain the most frequent
subgraphs.  Our experiments on database of large graphs show that FS^3 is
efficient, and it obtains subgraphs that are the most frequent amongst the
subgraphs of a given size.


# Installation

Run the following command in randommining folder:

```
make
```

A Sample Run: 

```
./randomminer -d mutagen_2.interactive (data set)  -i 100 (number of iteration)  -s 6 (subgraph size) -q  100000 (queue size)
```
