@echo off

set cluster_output_file=%1
shift

rem output header to the CSV
echo Input,StructuralSampler,Metis,>> "%cluster_output_file%"

rem run the tests
rem higher cluster sizes
CALL RunPerfTest %cluster_output_file% 1 100 0.01 %*
CALL RunPerfTest %cluster_output_file% 1 20 0.05 %*
CALL RunPerfTest %cluster_output_file% 1 10 0.1 %*
CALL RunPerfTest %cluster_output_file% 1 2 0.5 %*
CALL RunPerfTest %cluster_output_file% 1 1 1 %*
CALL RunPerfTest %cluster_output_file% 5 1 5 %*