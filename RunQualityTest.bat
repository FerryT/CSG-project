@echo off

rem run the tests
set cluster_output_file=%1
set cluster_size=%2
set cluster_snapshot_size=%3
shift
shift
shift

echo|set /p=%cluster_size%,>> "%cluster_output_file%"
"%cluster_application%" %* -CA StructuralSampler "%cluster_size%" -CA Metis "%cluster_size%" -CT Quality 100 -CO "%cluster_output_file%"