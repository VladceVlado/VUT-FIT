package dist_app_environment.pig_executor;

import com.google.common.io.Files;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.CommonConfigurationKeysPublic;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hdfs.HdfsConfiguration;
import org.apache.hadoop.hdfs.MiniDFSCluster;
import org.apache.hadoop.mapreduce.MRConfig;
import org.apache.hadoop.util.GenericOptionsParser;
import org.apache.pig.ExecType;
import org.apache.pig.PigServer;

import java.io.File;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

public class ServerMini {

    private final static File MINIDFS_BASEDIR = Files.createTempDir();
    private final static Path DFS_INPUT_DIR = new Path("pig/input");
    private final static Path DFS_OUTPUT_DIR = new Path("pig/output");

    public static void main(String[] args) throws Exception {
        // disable IPv6: not supported by Hadoop and results into error "Relative path in absolute URI" for Block Pool
        // `hostname` must have an IPv4 address, check it by: host -vt A `hostname`
        System.setProperty("java.net.preferIPv4Stack", "true");
        // conf
        final Configuration conf = new HdfsConfiguration();
        final String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
        if (otherArgs.length < 3) {
            System.err.println("Usage: pig-executor <local_pig_script> <local_in> [<local_in>...] <local_out>\n"
                    + "the local files will be availabe in HDFS as Pig script parameters: $INPUT/$OUTPUT directories and $INPUT1... files");
            System.exit(2);
        }
        // cluster start
        conf.set(MiniDFSCluster.HDFS_MINIDFS_BASEDIR, MINIDFS_BASEDIR.getAbsolutePath());
        final MiniDFSCluster miniDFSCluster = new MiniDFSCluster.Builder(conf).build();
        final FileSystem distributedFileSystem = miniDFSCluster.getFileSystem();
        final FileSystem localFileSystem = FileSystem.getLocal(conf);
        // HDFS init directories
        distributedFileSystem.delete(DFS_INPUT_DIR, true);
        distributedFileSystem.delete(DFS_OUTPUT_DIR, true);
        distributedFileSystem.mkdirs(DFS_INPUT_DIR);
        // HDFS copy input files from the local filesystem
        final Path[] srcPaths;
        // in Java 8:
        //srcPaths = IntStream.range(0, otherArgs.length - 1).mapToObj(i -> new Path(otherArgs[i])).toArray(Path[]::new);
        // in Java 7:
        final List<Path> srcPathsList = new LinkedList<>();
        for (int i = 0; i < otherArgs.length - 1; i++) {
            srcPathsList.add(new Path(otherArgs[i]));
        }
        srcPaths = new Path[srcPathsList.size()];
        srcPathsList.toArray(srcPaths);
        //
        distributedFileSystem.copyFromLocalFile(false, true, srcPaths, DFS_INPUT_DIR);
        // pig script parameters
        Map<String, String> parameters = new HashMap<>();
        parameters.put("INPUT", DFS_INPUT_DIR.toString());
        parameters.put("OUTPUT", DFS_OUTPUT_DIR.toString());
        for (int i = 1; i <= otherArgs.length - 1; i++) {
            final String baseName = new File(otherArgs[i]).getName();
            parameters.put("INPUT" + i, new Path(DFS_INPUT_DIR, baseName).toString());
        }
        // pig server with the job
        System.setProperty(CommonConfigurationKeysPublic.FS_DEFAULT_NAME_KEY,
                "hdfs://" + miniDFSCluster.getNameNode().getClientNamenodeAddress());
        if (System.getProperty(MRConfig.MASTER_ADDRESS) == null) {
            System.setProperty(MRConfig.MASTER_ADDRESS, "local");
        }
        final PigServer pigServer = new PigServer(ExecType.MAPREDUCE, System.getProperties());
        pigServer.registerScript(otherArgs[0], parameters);
        pigServer.shutdown();
        // HDFS dump resulting files to the local filesystem
        final Path outputPath = new Path(otherArgs[otherArgs.length - 1]);
        localFileSystem.delete(outputPath, true);
        localFileSystem.mkdirs(outputPath);
        for (FileStatus file : distributedFileSystem.listStatus(DFS_OUTPUT_DIR)) {
            final Path filePath = file.getPath();
            distributedFileSystem.copyToLocalFile(true, filePath, new Path(outputPath, filePath.getName()));
        }
        // cluster stop
        miniDFSCluster.shutdown(true);
    }

}
