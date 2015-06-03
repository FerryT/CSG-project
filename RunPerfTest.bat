@echo off

rem run the tests
set cluster_output_file=%1
set cluster_queries=%2
set cluster_updates=%3
set cluster_rule_value=%4
shift
shift
shift
shift

rem echo|set /p=%cluster_queries%:%cluster_updates%,>> "%cluster_output_file%"
echo|set /p=%cluster_rule_value%,>> "%cluster_output_file%"
echo "%cluster_application%" %* -CA StructuralSampler 100 -CT Throughput %cluster_queries% %cluster_updates% -CO "%cluster_output_file%"
"%cluster_application%" %* -CA StructuralSampler 100 -CT Throughput %cluster_queries% %cluster_updates% -CO "%cluster_output_file%"
echo|set /p=,>> "%cluster_output_file%"
echo "%cluster_application%" %* -CA Metis 100 -CT Throughput %cluster_queries% %cluster_updates% -CO "%cluster_output_file%"
"%cluster_application%" %* -CA Metis 100 -CT Throughput %cluster_queries% %cluster_updates% -CO "%cluster_output_file%"
echo , >> "%cluster_output_file%"