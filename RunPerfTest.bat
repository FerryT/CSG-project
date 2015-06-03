@echo off

rem run the tests
set cluster_output_file=%1
set cluster_queries=%2
set cluster_updates=%3
shift
shift
shift

rem echo|set /p=%cluster_queries%:%cluster_updates%,>> "%cluster_output_file%"
echo|set /p=%cluster_queries%,>> "%cluster_output_file%"
echo "%cluster_application%" %* -CA StructuralSampler 100 -CT Throughput %cluster_queries% %cluster_updates% -CO "%cluster_output_file%"
"%cluster_application%" %* -CA StructuralSampler 100 -CT Throughput %cluster_queries% %cluster_updates% -CO "%cluster_output_file%"
rem echo|set /p=,>> "%cluster_output_file%"
rem echo "%cluster_application%" %* -CA Metis 100 -CT Throughput %cluster_queries% %cluster_updates% -CO "%cluster_output_file%"
rem "%cluster_application%" %* -CA Metis 100 -CT Throughput %cluster_queries% %cluster_updates% -CO "%cluster_output_file%"
echo , >> "%cluster_output_file%"