# tabusearch-on-OPHS
Tabu search algorithm on team orienting problem with hotel selection.
<br/>
The starting hotel is 0 and the ending hotel is 1.
--contact me for more info on benchmarks and what they contain. <br/>
The benchmarks are located at <a href="AA-benchmark.rar">AA-benchmark </a> .<br/>
The local search implemented here is just a small local search. it can be improved a lot.<br/>
After initialization of the file, there is a random hotel selection function for the trips so that we would get to the last hotel in time.<br/>
then, a random selection func is called so the places are selected for each trip. then, a local search function is applied <br/>
to make the randomly built solution better. the tabu lists are used in the local search function.
