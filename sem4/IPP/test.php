<?php

// VUT FIT
// IPP 2017/2018
// Project: Sada skriptu pro interpretaci nestrukturovaneho imperativniho jazyka IPPcode18
// Author: Vladimir Dusek, xdusek27 (2BIT)
// Date: 14/3/2018
// File: test.php

// Extension:
//

// Execute:
// ./test.php --help
// ./test.php [--directory="path"] [--recursive] [--parse-script="file"] [--int-script="file"]

// Errors
// 10 ... Chybejici parametr skriptu (je-li treba) nebo pouziti zakazane kombinace parametru.
// 11 ... Chyba pri otevreni vstupnich souboru.

##########################################################################################################

/**
* Constants for error return codes
*/
const RET_ARG     = 10;
const RET_IN_FILE = 11;

/**
* Global variables
*/
$help = false;
$recursive = false;
$directory    = ".";
$parse_script = "./parse.php";
$int_script   = "./interpret.py";
$src_files = array();
$src_paths;
$test_results = array();

/**
* Global array for printing error messages
*/
$error = array(
"ERR_ARGS"    => "error: unrecognized option\ntry \"./test.php --help\" for more information\n",
"ERR_IN_FILE" => "error: cannot open input file\n",
"ERR_SYS"     => "error: system error\n"
);

##########################################################################################################

/**
 * @brief   Prints the help and correctly exit program.
 * @return  void
 */
function print_help()
{
	echo "Script test.php (in PHP 5.6) tests scripts parse.php and interpret.py.\n";
	echo "Usage:\n" .
	"  ./test.php --help \n" .
	"  ./test.php [--directory=\"path\"] [--recursive] [--parse-script=\"file\"] [--int-script=\"file\"]\n";
	exit(0);
}

/**
 * @brief Recursive variant of glob() function
 *        viz: https://stackoverflow.com/questions/17160696/php-glob-scan-in-subfolders-for-a-file
 * @return  $array  Array of all file_names
 */
function rglob($pattern, $flags = 0)
{
	$files = glob($pattern, $flags);
	foreach (glob(dirname($pattern).'/*', GLOB_ONLYDIR|GLOB_NOSORT) as $dir)
		$files = array_merge($files, rglob($dir.'/'.basename($pattern), $flags));
	return $files;
}

/**
 * @brief   Prints the error message and exit the program.
 * @param   string  $err_idx   The error type
 * @param   int     $ret_val   The return value
 * @return  int    			   The return code of the program
 */
function call_error($err_idx, $ret_val)
{
	global $error;
	fwrite(STDERR, $error[$err_idx]);
	exit($ret_val);
}

/**
 * @brief   Tests correctness of the command line arguments.
 * @param   string  $argv  The array of arguments.
 * @return  void
 */
function parse_options($argv)
{
	global $help;
	global $recursive;
	global $directory;
	global $parse_script;
	global $int_script;

	$arg_directory = false;
	$arg_parse_script = false;
	$arg_int_script = false;

	foreach ($argv as $value) {
		switch ($value) {

			case "--help":
				if ($help)
					call_error("ERR_ARGS", RET_ARG);
				else
					$help = true;
				break;

			case "--recursive":
				if ($recursive)
					call_error("ERR_ARGS", RET_ARG);
				else
					$recursive = true;
				break;

			default:
				if ($value == $argv[0])
					continue;

				else if (preg_match("/^--directory=(.*)$/", $value)) {
					if ($arg_directory)
						call_error("ERR_ARGS", RET_ARG);
					else {
						$arg_directory = true;
						$directory = substr($value, 12);
						if (substr($directory, -1) == "/")
							$directory = substr($directory, 0, -1);
					}
				}

				else if (preg_match("/^--parse-script=(.*)$/", $value)) {
					if ($arg_parse_script)
						call_error("ERR_ARGS", RET_ARG);
					else {
						$arg_parse_script = true;
						$parse_script = substr($value, 15);
						if (substr($parse_script, -1) == "/")
							$parse_script = substr($parse_script, 0, -1);
					}
				}

				else if (preg_match("/^--int-script=(.*)$/", $value)) {
					if ($arg_int_script)
						call_error("ERR_ARGS", RET_ARG);
					else {
						$arg_int_script = true;
						$int_script = substr($value, 13);
						if (substr($int_script, -1) == "/")
							$int_script = substr($int_script, 0, -1);
					}
				}

				else
					call_error("ERR_ARGS", RET_ARG);
				break;
		}
	}
}

/**
 * @brief   Set path for every test
 * @param   string  $recursive  Recursive way finding in directory
 * @return  void
 */
function set_paths_and_files($recursive)
{
	global $directory;
	global $src_files;
	global $src_paths;

	if ($recursive)
		$src_paths = rglob($directory . '/*.src');
	else
		$src_paths = glob($directory . '/*.src');

	natsort($src_paths);
	foreach ($src_paths as $path) {
		$path_arr = explode('/', $path);
		$file = array_values(array_slice($path_arr, -1))[0];
		array_push($src_files, $file);
	}
}

/**
 * @brief   If *.in, *.out or *.rc files don't exist, create them with default content.
 * @param   string  $in_path   *.in file
 * @param   string  $out_path  *.out file
 * @param   string  $rc_path   *.rc file
 * @return  void
 */
function create_non_existing_files($in_path, $out_path, $rc_path)
{
	if (!file_exists($in_path))
		file_put_contents($in_path, "");

	if (!file_exists($out_path))
		file_put_contents($out_path, "");

	if (!file_exists($rc_path))
		file_put_contents($rc_path, "0");
}

/**
 * @brief   Run parse.php and interpret.py scripts and compare outputs and
 * 			return codes with reference results.
 * @param      string  $src_file  The source file
 * @param      string  $in_file   In file
 * @param      string  $out_file  The out file
 * @param      int     $ref_rc    The reference return code
 * @return     string  Test result
 */
function evaluate_tests($src_file, $in_file, $out_file, $ref_rc)
{
	global $parse_script;
	global $int_script;

	exec("php7.0 " . $parse_script . " < " . $src_file . " > parse-tmp.xml 2> /dev/null", $out, $parse_rc);

	if ($parse_rc != 0) {
		if ($parse_rc == $ref_rc)
			$result = "OK";
		else
			$result = "FAIL - returned: " . $parse_rc . ", expected: " . $ref_rc;
	}

	else {
		exec("python3.6 " . $int_script . " --source=parse-tmp.xml < " . $in_file . " > int-tmp.out 2> /dev/null", $out, $int_rc);

		if ($int_rc == $ref_rc) {
			$diff_arr = array();
			exec("diff " . $out_file . " int-tmp.out", $diff_arr, $diff_rc);

			if ($diff_rc == 0)
				$result = "OK";
			else {
				$diff_out = "";
				foreach ($diff_arr as $elem)
					$diff_out = $diff_out . $elem . "\n";
				$result = "FAIL - incorrect output (diff: " . $diff_out . ")";
			}
		}
		else
			$result = "FAIL - returned: " . $int_rc . ", expected: " . $ref_rc;
	}

	return $result;
}

/**
 * @brief   Run all tests
 * @param   int   $num_of_tests  Number of tests
 * @return  void
 */
function run_tests($num_of_tests)
{
	global $src_paths;
	global $test_results;

	$ref_rc = array();

	for ($i = 0; $i < $num_of_tests; $i += 1) {
		$src_path = $src_paths[$i];
		$in_path = substr($src_path, 0, count($src_path) - 4) . "in";
		$out_path = substr($src_path, 0, count($src_path) - 4) . "out";
		$rc_path = substr($src_path, 0, count($src_path) - 4) . "rc";

		create_non_existing_files($in_path, $out_path, $rc_path);
		$ref_rc[$i] = file_get_contents("./" . $rc_path);
		$test_results[$i] = evaluate_tests($src_path, $in_path, $out_path, $ref_rc[$i]);
	}

	if (file_exists("parse-tmp.xml"))
		exec("rm parse-tmp.xml");
	if (file_exists("int-tmp.out"))
		exec("rm int-tmp.out");
}

/**
 * @brief   Generate result page in HTML
 * @param   string  $directory     Directory with tests
 * @param   int     $num_of_tests  Number of tests
 * @param   string  $src_paths     Path to tests
 * @param   array   $test_results  Results of all tests
 * @return  void
 */
function generate_html($directory, $num_of_tests, $src_paths, $test_results)
{
	echo '<!DOCTYPE html>
	<html>
		<head>
			<style>
				.OK {background-color: #ADFF2F;}
				.FAIL {background-color: #FF6347;}

				table, th, td {
					margin: 32px;
					line-height: 1.25;
					border: 2px solid black;
					border-collapse: collapse;
					padding: 8px;
				    border-spacing: 4px;
					background-color: #DCDCDC
				}

				th {
					text-align:center;
					font-size: 32px;
					background-color: #ccffff
				}

				table {
					text-align:left;
					font-size: 24px;
					width: 60%;
				}

				td {
					font-size: 24px;
					text-overflow: ellipsis;
				}

				body {
					font-size: 24px;
				}
			</style>
		</head>
		<body>
			<center>
				<div id="results">
					<h1> Test results for parse.php and interpret.py </h1>
					<b> Test directory: </b> ' . $directory . ' <br>
					<b> Number of tests: </b> ' . $num_of_tests . '
				</div>
				<table>
					<tr>
						<th> Test </th>
						<th> Result </th>
					</tr>';

	natsort($src_paths);
	foreach ($src_paths as $id => $test) {
		$name = substr($test, 0, count($test) - 5);
		$name = substr($name, strlen($directory) + 1);

		if ($test_results[$id] == "OK")
			$class = "OK";
		else
			$class = "FAIL";

		echo '<tr><td>' . $name . '</td>' . '<td class=' . $class . '>' . $test_results[$id] . '</td></tr>';
	}

	echo '		</table>
			</center>
		</body>
	</html>';
}

##########################################################################################################

	// Parse command line arguments
	parse_options($argv);

	// Print help if was required
	if ($help)
		print_help();

	// Set paths and file names of src files
	set_paths_and_files($recursive);

	// Test if files exists
	if (!file_exists($parse_script) or !file_exists($int_script))
		call_error("ERR_IN_FILE", RET_IN_FILE);

	// Run tests
	run_tests(count($src_files));

	// Generate html result page
	generate_html($directory, count($src_files), $src_paths, $test_results);

	exit(0);
?>
