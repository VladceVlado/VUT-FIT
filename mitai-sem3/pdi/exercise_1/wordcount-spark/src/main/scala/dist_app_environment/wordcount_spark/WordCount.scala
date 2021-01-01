package dist_app_environment.wordcount_spark

import java.io.File

import org.apache.commons.io.FileUtils
import org.apache.spark.launcher.SparkLauncher
import org.apache.spark.sql.SparkSession

object WordCount {
  def main(args: Array[String]) {
    if (args.length < 2) {
      System.err.println("Usage: wordcount <hdfs_in> [<hdfs_in>...] <hdfs_out>\n"
        + "hdfs_* args should start with hdfs:// for an HDFS filesystem or file:// for a local filesystem")
      System.exit(2)
    }
    // environment variable of Spark master node defaults to local[*] if the the master URL is absent
    if (System.getProperty(SparkLauncher.SPARK_MASTER) == null) {
      System.setProperty(SparkLauncher.SPARK_MASTER, "local[*]")
    }
    // create a Spark session for the job with respect to the environment variable above
    val spark = SparkSession.builder.appName(WordCount.getClass.getSimpleName).getOrCreate()
    // read files (based on "fs.defaultFS" for absent schema, "hdfs://..." if set HADOOP_CONF_DIR env-var, or "file://")
    val input = spark.sparkContext.textFile(args.dropRight(1).mkString(","))
    // parse inputs into a word-list
    val words = input.flatMap(line => line.split(" "))
    // apply MapReduce
    val counts = words.map(word => (word, 1)).reduceByKey(_ + _)
    // dump results
    FileUtils.deleteDirectory(new File(args.last))
    counts.saveAsTextFile(args.last)
  }
}
