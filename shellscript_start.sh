#!/bin/bash

# To run on terminal: ./shellscript_start.sh

fileName0="README.txt"
fileName1="result_report-cpu.txt"
fileName2="result_report-problem_size.txt"
fileName3a="result_report-serie-runtime.txt"
fileName3b="result_report-serieRand-runtime.txt"
fileName4="result_report-parallel-cores.txt"
fileName5="result_report-parallel-runtime.txt"
fileName6="result_report-speedup.txt"
fileName7="result_report-efficiency.txt"

fileHeader1="\n/* \n * CPU Report                                        \n */\n"
fileHeader2="\n/* \n * Problem Size Report                               \n */  "
fileHeader3a="\n/* \n * Serie Runtime Report In Seconds                  \n */  "
fileHeader3b="\n/* \n * Line Rand Matrix Serie Runtime Report In Seconds \n */  "
fileHeader4="\n/* \n * Parallel Cores Report                             \n */  "
fileHeader5="\n/* \n * Parallel Runtime Report In Seconds                \n */  "
fileHeader6="\n/* \n * Speedup Report                                    \n */  "
fileHeader7="\n/* \n * Efficiency Report                                 \n */  "

echo -e "$fileHeader1" >> $fileName1
echo -e "$fileHeader2" >> $fileName2
echo -e "$fileHeader3a" >> $fileName3a
echo -e "$fileHeader3b" >> $fileName3b
echo -e "$fileHeader4" >> $fileName4
echo -e "$fileHeader5" >> $fileName5
echo -e "$fileHeader6" >> $fileName6
echo -e "$fileHeader7" >> $fileName7

#serie compiler
g++ -O0 -g -W -ansi -pedantic -std=c++11 -o matrix-multiplication-serial matrix-multiplication-serial.cpp
g++ -O0 -g -W -ansi -pedantic -std=c++11 -o matrix-multiplication-serialRand matrix-multiplication-serialRand.cpp
#parallel compiler
mpic++ -g -Wall matrix-multiplication-parallel.cpp -o matrix-multiplication-parallel -lm
#analysis compiler
g++ -O0 -g -W -ansi -pedantic -std=c++11 -o calculates-serie-parallel-analysis calculates-serie-parallel-analysis.cpp

insertCPUInfo(){
	cat /proc/cpuinfo | grep "$2" | uniq >> $1	
}
insertCPUInfo $fileName1 'model name'
insertCPUInfo $fileName1 'vendor'
insertCPUInfo $fileName1 'cpu cores'
insertCPUInfo $fileName1 'siblings'
insertCPUInfo $fileName1 'cache size'

#attempts by number of cores and size
attempts=5
for cores in 2 4 6 8
do 
	for sizeProblem in 3 4
	do 
		echo -e "" >> $fileName2
		echo -e "" >> $fileName3a
		echo -e "" >> $fileName3b
		echo -e "" >> $fileName4
		echo -e "" >> $fileName5
		echo -e "" >> $fileName6
		echo -e "" >> $fileName7
		for attempt in $(seq $attempts)
		do
            echo -e ":$sizeProblem\c" >> $fileName2
            echo -e ":$cores\c      " >> $fileName4
			#serie execute
			./matrix-multiplication-serial $sizeProblem
			./matrix-multiplication-serialRand $sizeProblem
			#parallel execute
			mpirun -np $cores --oversubscribe ./matrix-multiplication-parallel $sizeProblem
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
showOnTerminal $fileName1
showOnTerminal $fileName2
showOnTerminal $fileName3a
showOnTerminal $fileName3b
showOnTerminal $fileName4
showOnTerminal $fileName5
showOnTerminal $fileName6
showOnTerminal $fileName7
echo -e

txt2pdf(){
    vim $1 -c "hardcopy > $1.ps | q";ps2pdf $1.ps; rm $1.ps
}
txt2pdf $fileName0
txt2pdf $fileName1
txt2pdf $fileName2
txt2pdf $fileName3a
txt2pdf $fileName3b
txt2pdf $fileName4
txt2pdf $fileName5
txt2pdf $fileName6
txt2pdf $fileName7

pdfunite $fileName0.pdf $fileName1.pdf $fileName2.pdf $fileName3a.pdf $fileName3b.pdf $fileName4.pdf $fileName5.pdf $fileName6.pdf $fileName7.pdf report.pdf

rm $fileName0.pdf $fileName1.pdf $fileName2.pdf $fileName3a.pdf $fileName3b.pdf $fileName4.pdf $fileName5.pdf $fileName6.pdf $fileName7.pdf
rm $fileName1 $fileName2 $fileName3a $fileName3b $fileName4 $fileName5 $fileName6 $fileName7
rm matrix-multiplication-serial matrix-multiplication-serialRand
rm matrix-multiplication-parallel
rm calculates-serie-parallel-analysis

exit