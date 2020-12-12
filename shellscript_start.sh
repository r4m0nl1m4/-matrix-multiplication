#!/bin/bash

# To run on terminal: ./shellscript_start.sh

fileName00="README.txt"
fileName01="result_report-cpu.txt"
fileName02="result_report-problem_size.txt"
fileName03a="result_report-serie-runtime.txt"
fileName03b="result_report-serieRand-runtime.txt"
fileName04="result_report-parallel-cores.txt"
fileName05="result_report-parallel-runtime.txt"
fileName06="result_report-parallel-pthreads-runtime.txt"
fileName07="result_report-speedup.txt"
fileName08="result_report-parallel-pthreads-speedup.txt"
fileName09="result_report-efficiency.txt"
fileName10="result_report-parallel-pthreads-efficiency.txt"

fileHeader01="\n/* \n * CPU Report                                                                        \n */\n"
fileHeader02="\n/* \n * Problem Size Report                                                               \n */  "
fileHeader03a="\n/* \n * Matrix Multiplication using Serie Process Runtime Report In Seconds              \n */  "
fileHeader03b="\n/* \n * Matrix Multiplication using Line Rand Matrix Serie Runtime Report In Seconds     \n */  "
fileHeader04="\n/* \n * CPU Cores Report                                                                  \n */  "
fileHeader05="\n/* \n * Matrix Multiplication using Parallel Process (MPI) Runtime Report In Seconds      \n */  "
fileHeader06="\n/* \n * Matrix Multiplication using Parallel Process (pthreads) Runtime Report In Seconds \n */  "
fileHeader07="\n/* \n * Matrix Multiplication using Parallel Process (MPI) Speedup Report                 \n */  "
fileHeader08="\n/* \n * Matrix Multiplication using Parallel Process (pthreads) Speedup Report            \n */  "
fileHeader09="\n/* \n * Matrix Multiplication using Parallel Process (MPI) Efficiency Report              \n */  "
fileHeader10="\n/* \n * Matrix Multiplication using Parallel Process (pthreads) Efficiency Report         \n */  "

echo -e "$fileHeader01" >> $fileName01
echo -e "$fileHeader02" >> $fileName02
echo -e "$fileHeader03a" >> $fileName03a
echo -e "$fileHeader03b" >> $fileName03b
echo -e "$fileHeader04" >> $fileName04
echo -e "$fileHeader05" >> $fileName05
echo -e "$fileHeader06" >> $fileName06
echo -e "$fileHeader07" >> $fileName07
echo -e "$fileHeader08" >> $fileName08
echo -e "$fileHeader09" >> $fileName09
echo -e "$fileHeader10" >> $fileName10

#serie compiler
g++ -O0 -g -W -ansi -pedantic -std=c++11 -o matrix-multiplication-serial matrix-multiplication-serial.cpp
g++ -O0 -g -W -ansi -pedantic -std=c++11 -o matrix-multiplication-serialRand matrix-multiplication-serialRand.cpp
#parallel compiler
mpic++ -g -Wall matrix-multiplication-parallel.cpp -o matrix-multiplication-parallel -lm
g++ -std=c++11 matrix-multiplication-parallel-pthreads.cpp -o matrix-multiplication-parallel-pthreads -lpthread
#analysis compiler
g++ -O0 -g -W -ansi -pedantic -std=c++11 -o calculates-serie-parallel-analysis calculates-serie-parallel-analysis.cpp

insertCPUInfo(){
	cat /proc/cpuinfo | grep "$2" | uniq >> $1	
}
insertCPUInfo $fileName01 'model name'
insertCPUInfo $fileName01 'vendor'
insertCPUInfo $fileName01 'cpu cores'
insertCPUInfo $fileName01 'siblings'
insertCPUInfo $fileName01 'cache size'

#attempts by number of cores and size
attempts=5
for cores in 2 4 6 8
do 
	for sizeProblem in 3 4
	do 
		echo -e "" >> $fileName02
		echo -e "" >> $fileName03a
		echo -e "" >> $fileName03b
		echo -e "" >> $fileName04
		echo -e "" >> $fileName05
		echo -e "" >> $fileName06
		echo -e "" >> $fileName07
		echo -e "" >> $fileName08
		echo -e "" >> $fileName09
		echo -e "" >> $fileName10
		for attempt in $(seq $attempts)
		do
            echo -e ":$sizeProblem\c" >> $fileName02
            echo -e ":$cores\c      " >> $fileName04
			#serie execute
			./matrix-multiplication-serial $sizeProblem
			./matrix-multiplication-serialRand $sizeProblem
			#parallel execute
			mpirun -np $cores --oversubscribe ./matrix-multiplication-parallel $sizeProblem
			./matrix-multiplication-parallel-pthreads 4
			#analysis execute
            ./calculates-serie-parallel-analysis
		done
	done
done

showOnTerminal(){
	while IFS= read -r line
	do
	    echo "$line"
	done <"$1"
}
showOnTerminal $fileName01
showOnTerminal $fileName02
showOnTerminal $fileName03a
showOnTerminal $fileName03b
showOnTerminal $fileName04
showOnTerminal $fileName05
showOnTerminal $fileName06
showOnTerminal $fileName07
showOnTerminal $fileName08
showOnTerminal $fileName09
showOnTerminal $fileName10
echo -e

txt2pdf(){
    vim $1 -c "hardcopy > $1.ps | q";ps2pdf $1.ps; rm $1.ps
}
txt2pdf $fileName00
txt2pdf $fileName01
txt2pdf $fileName02
txt2pdf $fileName03a
txt2pdf $fileName03b
txt2pdf $fileName04
txt2pdf $fileName05
txt2pdf $fileName06
txt2pdf $fileName07
txt2pdf $fileName08
txt2pdf $fileName09
txt2pdf $fileName10

pdfunite $fileName00.pdf $fileName01.pdf $fileName02.pdf $fileName03a.pdf $fileName03b.pdf $fileName04.pdf $fileName05.pdf $fileName06.pdf $fileName07.pdf $fileName08.pdf $fileName09.pdf $fileName10.pdf report.pdf

rm $fileName00.pdf $fileName01.pdf $fileName02.pdf $fileName03a.pdf $fileName03b.pdf $fileName04.pdf $fileName05.pdf $fileName06.pdf $fileName07.pdf $fileName08.pdf $fileName09.pdf $fileName10.pdf
rm $fileName01 $fileName02 $fileName03a $fileName03b $fileName04 $fileName05 $fileName06 $fileName07 $fileName08 $fileName09 $fileName10
rm matrix-multiplication-serial matrix-multiplication-serialRand
rm matrix-multiplication-parallel
rm matrix-multiplication-parallel-pthreads
rm calculates-serie-parallel-analysis

exit