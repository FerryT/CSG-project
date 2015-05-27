@echo off

echo Datasets are at the folowing location %cluster_datasets%
echo The application that is run in the test is as follows %cluster_application%
echo The results will be in %cluster_output%

echo Removing old results
rd /s /q %cluster_output%
echo creating dir
mkdir %cluster_output%


echo running Poisson with different p values
%cluster_application% -CI Poisson 100 0.90 1000 -CS FilterEdges 0.80 -CA StructuralSampler 10 -CT VisualizeResult 100 1 -CO "%cluster_output%\PoissonP08C10.png"
%cluster_application% -CI Poisson 100 0.90 1000 -CS FilterEdges 0.60 -CA StructuralSampler 10 -CT VisualizeResult 100 1 -CO "%cluster_output%\PoissonP06C10.png"
%cluster_application% -CI Poisson 100 0.90 1000 -CS FilterEdges 0.40 -CA StructuralSampler 10 -CT VisualizeResult 100 1 -CO "%cluster_output%\PoissonP04C10.png"
%cluster_application% -CI Poisson 100 0.90 1000 -CS FilterEdges 0.20 -CA StructuralSampler 10 -CT VisualizeResult 100 1 -CO "%cluster_output%\PoissonP02C10.png"
%cluster_application% -CI Poisson 100 0.90 1000 -CS FilterEdges 0.00 -CA StructuralSampler 10 -CT VisualizeResult 100 1 -CO "%cluster_output%\PoissonP00C10.png"

echo running BAModel with different p values
%cluster_application% -CI BAModel 100 -CS FilterEdges 0.80 -CA StructuralSampler 10 -CT VisualizeResult 100 1 -CO "%cluster_output%\BAModelP08C10.png"
%cluster_application% -CI BAModel 100 -CS FilterEdges 0.60 -CA StructuralSampler 10 -CT VisualizeResult 100 1 -CO "%cluster_output%\BAModelP06C10.png"
%cluster_application% -CI BAModel 100 -CS FilterEdges 0.40 -CA StructuralSampler 10 -CT VisualizeResult 100 1 -CO "%cluster_output%\BAModelP04C10.png"
%cluster_application% -CI BAModel 100 -CS FilterEdges 0.20 -CA StructuralSampler 10 -CT VisualizeResult 100 1 -CO "%cluster_output%\BAModelP02C10.png"
%cluster_application% -CI BAModel 100 -CS FilterEdges 0.00 -CA StructuralSampler 10 -CT VisualizeResult 100 1 -CO "%cluster_output%\BAModelP00C10.png"

echo running fruits with different p values
%cluster_application% -CI mgraph "%cluster_datasets%\fruits" -CS FilterEdges 0.80 -CA StructuralSampler 10 -CT VisualizeResult 100 1 -CO "%cluster_output%\fruitsP08C10.png"
%cluster_application% -CI mgraph "%cluster_datasets%\fruits" -CS FilterEdges 0.60 -CA StructuralSampler 10 -CT VisualizeResult 100 1 -CO "%cluster_output%\fruitsP06C10.png"
%cluster_application% -CI mgraph "%cluster_datasets%\fruits" -CS FilterEdges 0.40 -CA StructuralSampler 10 -CT VisualizeResult 100 1 -CO "%cluster_output%\fruitsP04C10.png"
%cluster_application% -CI mgraph "%cluster_datasets%\fruits" -CS FilterEdges 0.20 -CA StructuralSampler 10 -CT VisualizeResult 100 1 -CO "%cluster_output%\fruitsP02C10.png"
%cluster_application% -CI mgraph "%cluster_datasets%\fruits" -CS FilterEdges 0.00 -CA StructuralSampler 10 -CT VisualizeResult 100 1 -CO "%cluster_output%\fruitsP00C10.png"

@echo on