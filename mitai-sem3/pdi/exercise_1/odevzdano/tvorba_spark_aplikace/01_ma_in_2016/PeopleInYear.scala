package dist_app_environment.peopleinyear_spark

import java.io.File

import org.apache.commons.io.FileUtils
import org.apache.spark.launcher.SparkLauncher
import org.apache.spark.sql.functions.{col, sum}
import org.apache.spark.sql.{Column, DataFrame, SparkSession}

object PeopleInYear {

    def main(args: Array[String]) {

        if (args.length < 2) {
            System.err.println("Usage: peopleinyear <hdfs_in> [<hdfs_in>...] <hdfs_out>\n"
                + "hdfs_* args should start with hdfs:// for an HDFS filesystem or file:// for a local filesystem")
            System.exit(2)
        }

        // environment variable of Spark master node defaults to local[*] if the the master URL is absent
        if (System.getProperty(SparkLauncher.SPARK_MASTER) == null) {
            System.setProperty(SparkLauncher.SPARK_MASTER, "local[*]")
        }

        // create a Spark session for the job with respect to the environment variable above
        val spark: SparkSession = SparkSession.builder.appName(PeopleInYear.getClass.getSimpleName).getOrCreate()


        /* load and process as DataFrame */

        // read files (based on "fs.defaultFS" for absent schema, "hdfs://..." if set HADOOP_CONF_DIR env-var, or "file://")
        val inputDf: DataFrame = spark.read.format("csv")
            .option("header", "true") // uses the first line as names of columns
            .option("inferSchema", "true") // infers the input schema automatically from data (one extra pass over the data)
            .load(args.dropRight(1): _*)

        // filter jmena starts with MA
        var dataFrame: DataFrame = inputDf.filter(inputDf("JMÉNO").startsWith("MA"))

        // select column 2016
        dataFrame = dataFrame.select(col = "2016")

        // sum the columns
        val sum_cols: Array[Column] = dataFrame.columns.map(x => sum(col(x)))

        // get data frame of sums
        dataFrame = dataFrame.agg(sum_cols.head, sum_cols.tail: _*)

        // dump output
        val outputPathDf: String = args.last + "/as-dataframe"
        FileUtils.deleteDirectory(new File(outputPathDf))
        dataFrame.write.format("csv").option("header", "true").save(outputPathDf)
    }
}
