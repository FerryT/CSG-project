@echo off

echo Datasets are at the folowing location %cluster_datasets%
echo The application that is run in the test is as follows %cluster_application%
echo The results will be in %cluster_output%

echo Removing old results
rd /s /q "%cluster_output%"
echo creating dir
mkdir "%cluster_output%"

echo running Poisson with different p values
rem todo more updates and nodes
call RunMultipleQualityTests "%cluster_output%\QualityClusterSizePoission1000.csv" 100 -CI Poisson 100 0.85 300
echo running BA model with different p values
call RunMultipleQualityTests "%cluster_output%\QualityClusterSizeBAModel100.csv" 100 -CI BAModel 100
echo running fruits with different p values
call RunMultipleQualityTests "%cluster_output%\QualityClusterSizeFruits.csv" 100 -CI mgraph "%cluster_datasets%\fruits"

rem todo uncomment these so they run
echo running Cit-HepPh with different p values
rem call RunMultipleQualityTests "%cluster_output%\QualityClusterSizeCit.csv" 10000 -CI graph "%cluster_datasets%\Cit-HepPh.txt" -CS SlideWindow 10000
echo running higgs-reply_network with different p values
rem call RunMultipleQualityTests "%cluster_output%\QualityClusterSizehiggs.csv" 10000 -CI graph "%cluster_datasets%\higgs-reply_network.edgelist" -CS SlideWindow 10000
echo running web-NotreDame with different p values
rem call RunMultipleQualityTests "%cluster_output%\QualityClusterSizeNotreDame.csv" 10000 -CI graph "%cluster_datasets%\web-NotreDame.txt" -CS SlideWindow 10000


echo running Poisson with different p values
rem todo more updates and nodes
call RunMultiplePerfTests "%cluster_output%\PerfPoission1000.csv" -CI Poisson 100 1 300
echo running BA model with different p values
call RunMultiplePerfTests "%cluster_output%\PerfBAModel100.csv" -CI BAModel 100
echo running fruits with different p values
call RunMultiplePerfTests "%cluster_output%\PerfFruits.csv" -CI mgraph "%cluster_datasets%\fruits"

rem todo uncomment these so they run
echo running Cit-HepPh with different p values
rem call RunMultiplePerfTests "%cluster_output%\QualityClusterSizeCit.csv" 10000 -CI graph "%cluster_datasets%\Cit-HepPh.txt"
echo running higgs-reply_network with different p values
rem call RunMultiplePerfTests "%cluster_output%\QualityClusterSizehiggs.csv" 10000 -CI graph "%cluster_datasets%\higgs-reply_network.edgelist"
echo running web-NotreDame with different p values
rem call RunMultiplePerfTests "%cluster_output%\QualityClusterSizeNotreDame.csv" -CI graph "%cluster_datasets%\web-NotreDame.txt" -CS SlideWindow 10000



echo Plotting
%cluster_latex_exe% "-include-directory=%cluster_output%" "-aux-directory=%cluster_output%" "-output-directory=%cluster_output%" -interaction=nonstopmode -quiet "%cluster_latex_plot%"
echo Done