@echo off

echo Datasets are at the folowing location %cluster_datasets%
echo The application that is run in the test is as follows %cluster_application%
echo The results will be in %cluster_output%

echo Removing old results
rd /s /q "%cluster_output%"
echo creating dir
mkdir "%cluster_output%"

call RunMultipleQualityTests "%cluster_output%\QualityClusterSizePoission1000.csv" 100 -CI Poisson 100 1 1000
call RunMultipleQualityTests "%cluster_output%\QualityClusterSizeBAModel100.csv" 100 -CI BAModel 100
call RunMultipleQualityTests "%cluster_output%\QualityClusterSizeFruits.csv" 100 -CI mgraph "%cluster_datasets%\fruits"