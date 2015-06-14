@echo off

set cluster_output_file=%1
set cluster_snapshot_size=%2
shift
shift

rem output header to the CSV
echo Input,StructuralSampler,Metis,>> "%cluster_output_file%"

rem run the tests
rem higher cluster sizes
CALL RunQualityTest %cluster_output_file% 10 100 %*
CALL RunQualityTest %cluster_output_file% 50 500 %*
CALL RunQualityTest %cluster_output_file% 100 1000 %*
CALL RunQualityTest %cluster_output_file% 500 5000 %*
