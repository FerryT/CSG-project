@echo off

set cluster_application=Release\StructuralSampler.exe
set cluster_output=output
set cluster_latex_plot=documents\Plots\Plots.tex
set cluster_latex_exe=pdflatex

set /p cluster_datasets=Where are the datasets located (without leading slash)? 
set /p cluster_application=Where is the application? [Release\StructuralSampler.exe] 
set /p cluster_output=Where does the results have to be writen too (without leading slash)? [output] 
set /p cluster_latex_plot=What is the plot latex file? [documents\Plots\Plots.tex]
set /p cluster_latex_exe=What latex executable has to be used? [pdflatex]

echo Datasets are at the folowing location %cluster_datasets%
echo The application that is run in the test is as follows %cluster_application%
echo The results will be in %cluster_output%
echo This file will plot everything %cluster_latex_plot% with the following application %cluster_latex_exe%

@echo on