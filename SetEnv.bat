@echo off

set cluster_application=Release\StructuralSampler.exe
set cluster_output=output

set /p cluster_datasets=Where are the datasets located (without leading slash)? 
set /p cluster_application=Where is the application? [Release\StructuralSampler.exe] 
set /p cluster_output=Where does the results have to be writen too (without leading slash)? [output] 

echo Datasets are at the folowing location %cluster_datasets%
echo The application that is run in the test is as follows %cluster_application%
echo The results will be in %cluster_output%

@echo on