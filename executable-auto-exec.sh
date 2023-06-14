#!/bin/bash
# executes a file given by input $contador times
crossoverArgs=("0.85")
mutationArgs=("0.05" "0.07")
elitismArgs=("0.02")

count=0
execution_times=20
input_file="main_input-c85.txt"
for crossover in ${crossoverArgs[@]}; do
	for mutation in ${mutationArgs[@]}; do
		for elitism in ${elitismArgs[@]}; do
			printf "%s\n%s\n%s\n" $crossover $mutation $elitism > $input_file
			while [ $count -lt $execution_times ]; do
				$((count += 1)) # Incrementa o contador
				echo "---- Execução n° $count ----"
				cat $input_file | ./main
				echo "--------- Fim ---------"
			done
			count=0
		done
	done
done
