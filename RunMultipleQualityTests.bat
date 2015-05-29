@echo off

set cluster_output_file=%1
set cluster_snapshot_size=%2
shift
shift

rem output header to the CSV
echo ClusterSize,StructuralSampler,Metis,>> "%cluster_output_file%"

rem run the tests
rem higher cluster sizes
CALL RunQualityTest %cluster_output_file% 10 "%cluster_snapshot_size%" %*
CALL RunQualityTest %cluster_output_file% 30 "%cluster_snapshot_size%" %*
CALL RunQualityTest %cluster_output_file% 50 "%cluster_snapshot_size%" %*
CALL RunQualityTest %cluster_output_file% 70 "%cluster_snapshot_size%" %*
CALL RunQualityTest %cluster_output_file% 90 "%cluster_snapshot_size%" %*