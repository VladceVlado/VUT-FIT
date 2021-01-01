# Pig Script Executor

The executor runs Pig scripts on [an embedded Pig server](https://wiki.apache.org/pig/EmbeddedPig) in a Hadoop (DFS) cluster.

The Pig scripts can be [parametrized](https://wiki.apache.org/pig/ParameterSubstitution):

*	`$INPUT` is an HDFS directory which contains all input files uploaded from the local filesystem
*	`$INPUT1`, `$INPUT2`, etc. are the individual input files in the input HDFS directory
*	`$OUTPUT` is an HDFS directory for the results that will be later downloaded to the local filesystem

## Running in DFS Mini Cluster

In the case of DFS Mini cluster, the Pig script, all input files, and also the output directory should be in the local filesystem.
The input files are uploaded into a HDFS of the mini-cluster and results are downloaded back to the local filesystem.

~~~shell
./gradlew build
java -cp ./build/libs/pig-executor-1.0-SNAPSHOT.jar dist_app_environment.pig_executor.ServerMini <local_pig_script> <local_in> [<local_in>...] <local_out>
~~~

or

~~~shell
./gradlew run --args="<pig_script> <local_in> [<local_in>...] <local_out>"
~~~

## Running in Hadoop Cluster

~~~shell
./gradlew build
hadoop jar ./build/libs/pig-executor-1.0-SNAPSHOT.jar <hdfs_pig_script> <hdfs_in> [<hdfs_in>...] <hdfs_out>
~~~

## Examples

*	[Programming Pig by Alan F. Gates](https://github.com/alanfgates/programmingpig)
*	[Sounder](https://github.com/thedatachef/sounder)
