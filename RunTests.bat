@echo off

echo Datasets are at the folowing location %cluster_datasets%
echo The application that is run in the test is as follows %cluster_application%
echo The results will be in %cluster_output%

echo Removing old results
rd /s /q %cluster_output%
echo creating dir
mkdir %cluster_output%


echo running Poisson with different p values

echo name;10;30;50;70;90;>> "%cluster_output%\QualityClusterSize.csv"
echo|set /p=Poisson;>> "%cluster_output%\QualityClusterSize.csv"
%cluster_application% -CI Poisson 100 0.90 500 -CA StructuralSampler 10 -CA StructuralSampler 30 -CA StructuralSampler 50 -CA StructuralSampler 70 -CA StructuralSampler 90 -CT Quality 100 -CO "%cluster_output%\QualityClusterSize.csv"
echo|set /p=Metis;>> "%cluster_output%\QualityClusterSize.csv"
%cluster_application% -CI Poisson 100 0.90 500 -CA Metis 10 -CA Metis 30 -CA Metis 50 -CA Metis 70 -CA Metis 90 -CT Quality 100 -CO "%cluster_output%\QualityClusterSize.csv"

@echo on