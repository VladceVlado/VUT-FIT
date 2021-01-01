# Spark WordCount Example

The example runs a [WordCount example](https://github.com/apache/spark/blob/master/examples/src/main/java/org/apache/spark/examples/JavaWordCount.java) in Scala in a Spark cluster.

## Running in Spark local Mode

~~~shell
./gradlew build
java -jar ./build/libs/wordcount-spark-1.0-SNAPSHOT-all.jar <local_in> [<local_in>...] <local_out>
~~~

or

~~~shell
./gradlew run --args="<local_in> [<local_in>...] <local_out>"
~~~

## Running in Spark Cluster

The application can be [launched by the `spark-submit` script](https://spark.apache.org/docs/latest/submitting-applications.html) in Spark's `bin` directory.

E.g., to run on a Spark standalone cluster in cluster deploy mode with supervise, use:

~~~shell
./gradlew build
spark-submit \
  --class dist_app_environment.wordcount_spark.WordCount \
  --master spark://HOST:PORT \
  --deploy-mode cluster \
  --supervise \
  ./build/libs/wordcount-spark-1.0-SNAPSHOT-all.jar \
  <hdfs_in> [<hdfs_in>...] <hdfs_out>
~~~
