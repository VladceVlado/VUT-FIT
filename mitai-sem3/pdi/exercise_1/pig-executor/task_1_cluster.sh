#!/bin/sh

# input and output directories are in HDFS, not in a local filesystem (use hdfs dfs -put/-get to upload and download them)
exec pig -f sample-scripts/cetnost-jmena-dnar-2016.pig -p INPUT1=cetnost-jmena-dnar-2016/cetnost-jmena-dnar.csv -p OUTPUT=cetnost-jmena-dnar-2016.pig_output
