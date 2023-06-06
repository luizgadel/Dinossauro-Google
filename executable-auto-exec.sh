#!/bin/bash
# executes a file given by input $contador times
execution_times=20

count=0
file_to_execute=$1

while [ $count -lt $execution_times ]; do
	$((count += 1)) # Incrementa o contador
	echo "---- Execução n° $count ----"
	./$file_to_execute
	echo "--------- Fim ---------"

done
