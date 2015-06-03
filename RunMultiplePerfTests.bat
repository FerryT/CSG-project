@echo off

set cluster_output_file=%1
shift

rem output header to the CSV
echo Input,StructuralSampler,Metis,>> "%cluster_output_file%"

rem run the tests
rem higher cluster sizes
CALL RunPerfTest %cluster_output_file% 1 100 %*
CALL RunPerfTest %cluster_output_file% 1 20 %*
CALL RunPerfTest %cluster_output_file% 1 10 %*
CALL RunPerfTest %cluster_output_file% 1 2 %*
CALL RunPerfTest %cluster_output_file% 1 1 %*
CALL RunPerfTest %cluster_output_file% 5 1 %*