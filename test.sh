#!/bin/bash

times_ans=0
perf_ans=0


### CHECK FOR USER INPUT ON TIMING TESTS
while true; do
	read -p "Do you wish to run timing tests? " ans
	case $ans in
		[Yy]* ) let times_ans=1; break;;
		[Nn]* ) break;;
		* ) echo "Please answer y/N.";;
	esac
done

### CHECK FOR USER INPUT ON PERF TESTS
while true; do
	read -p "Do you wish to run perf tests? " ans
	case $ans in
		[Yy]* ) let perf_ans=1; break;;
		[Nn]* ) break;;
		* ) echo "Please answer y/N.";;
	esac
done

valid_ans=0

## CHECK FOR USER INPUT ON VALIDITY TESTS
while true; do
	read -p "Do you wish to run validity tests? " ans
	case $ans in
		[Yy]* ) let valid_ans=1; break;;
		[Nn]* ) break;;
		* ) echo "Please answer y/N.";;
	esac
done

############## PERF AND TIME TEST #############
if [ $times_ans -eq 1 ] || [ $perf_ans -eq 1 ]
then
	printf "\n***************TIMING AND PERF TESTS****************\n"

	### COMPILATION FOR TIMING
	gcc -c test/test_times_parallel.c
	gcc -c src/marching_squares_parallel.c
	gcc -c src/marching_squares.c
	gcc -c test/test_times.c
	gcc -c test/test_blobs.c
	gcc -c test/test_blobs_parallel.c
	gcc -Wall -Werror -Wunused-variable marching_squares.o test_times.o -o test_times
	gcc -Wall -Werror -Wunused-variable -pthread marching_squares_parallel.o test_times_parallel.o -o test_times_parallel
	gcc -Wall -Werror -Wunused-variable marching_squares.o test_blobs.o -o test_blobs
	gcc -Wall -Werror -Wunused-variable -pthread marching_squares_parallel.o test_blobs_parallel.o -o test_blobs_parallel

	for f in test/time/*.in 
	### For every file in the test time block loop through them an
	do
		let len=${#f}-2 		# Get the length of file without type
		fname=${f:10:len}		# Get the name of the file
		if [ $times_ans -eq 1 ]		# If user selected to test times
		then
			while IFS= read -r line
			do
				echo "Test Single Thread: $fname"
				time ./test_times $line

				echo "Test Parallel: $fname"
				time ./test_times_parallel $line
			done < "$f"

		fi

				
		############ RUN PERF TESTS FOR SINGLE THREAD ##########
		if [ $perf_ans -eq 1 ]
		then
			while IFS= read -r line
			do
				sudo perf record -F 100 -a -g -- ./test_times $line
				sudo perf script > "out/perf/$fname.perf"
				./stackcollapse-perf.pl "out/perf/$fname.perf" > "out/folded/$fname.folded"
				./flamegraph.pl --hash "out/folded/$fname.folded" > "out/svg/$fname.svg"

			done < "$f"
		fi
		rm -f perf_data.old
		rm -f perf_data
	done

	# Running blob tests
	printf "\nTiming Blob Tests\n"
	echo "Testing Blob 1 Single Thread"
	time ./test_blobs 1

	echo "Testing Blob 2 Single Thread"
	time ./test_blobs 2

	echo "Testing Blob 1 Multi Thread"
	time ./test_blobs_parallel 1

	echo "Testing Blob 2 Multi Thread"
	time ./test_blobs_parallel 2

	# Clean up the file after
	rm test_times
	rm test_times_parallel
	rm marching_squares_parallel.o
	rm test_times_parallel.o
	rm test_blobs.o
	rm marching_squares.o
	rm test_blobs_parallel.o
	rm test_blobs
	rm test_blobs_parallel	
fi

##### TEST VALIDITY ########
if [ $valid_ans -eq 1 ]
then
	printf "\n************VALIDITY TESTS***************\n"
	### Compile the testing programs ###
	gcc -Wall -Werror -Wunused-variable test/test_valid.c -o test_valid
	gcc -c src/marching_squares_parallel.c
	gcc -c test/test_valid_parallel.c
	gcc -Wall -Werror -Wunused-variable -pthread marching_squares_parallel.o test_valid_parallel.o -o test_valid_parallel

	echo "Testing single thread"
	for f in test/valid/*.in
	do
		./test_valid < $f
	done

	echo "Testing multi threaded"
	for f in test/valid/*.in
	do
		./test_valid_parallel < $f
	done

	# Clean up the file after
	rm test_valid
	rm test_valid_parallel.o
	rm marching_squares_parallel.o
	rm test_valid_parallel
fi

