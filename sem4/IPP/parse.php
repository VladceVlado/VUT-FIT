<?php

// VUT FIT
// IPP 2017/2018
// Project: Sada skriptu pro interpretaci nestrukturovaneho imperativniho jazyka IPPcode18
// Author: Vladimir Dusek, xdusek27 (2BIT)
// Date: 14/3/2018
// File: parse.php

// Extension:
// STATP

// Execute:
// ./parse.php --help
// ./parse.php [--stats="file"] [--comments] [--loc]

// Errors
// 10 ... Chybejici parametr skriptu (je-li treba) nebo pouziti zakazane kombinace parametru.
// 12 ... Chyba pri otevreni vystupnich souboru pro zapis (napr. nedostatecne opravneni).
// 21 ... Lexikalni nebo syntakticka chyba zdrojoveho kodu zapsaneho v IPPcode18.
// 99 ... Interni chyba (Neovlivnena vstupnimi soubory ci parametry prikazove radky. Napr. chyba
//        alokace pameti).

##########################################################################################################

/**
* Constants for error return codes
*/
const RET_ARG      = 10;
const RET_OUT_FILE = 12;
const RET_SYN_LEX  = 21;
const RET_SYS      = 99;

/**
* Global variables for check command line arguments
*/
$v_help       = false;
$v_stats_comm = false;
$v_stats_loc  = false;
$stats_file   = "";

/**
* Global variables for counting comments and instructions
*/
$loc_cnt  = 0;
$comm_cnt = 0;

/**
* Global array for printing error messages
*/
$error = array(
"ERR_INTRO"      => "error: wrong introductury line of the source code\n",
"ERR_ARGS"       => "error: unrecognized option\ntry \"./parse.php --help\" for more information\n",
"ERR_UNK_INS"    => "error: unknown instruction\n",
"ERR_NUM_INS"    => "error: invalid number of arguments of instruction\n",
"ERR_VAR"        => "error: invalid format of variable\n",
"ERR_SYM"        => "error: invalid format of symbol\n",
"ERR_LABEL"      => "error: invalid format of label\n",
"ERR_TYPE"       => "error: invalid format of type\n",
"ERR_UNEXPECTED" => "error: unexpected error\n",
"ERR_OUT_FILE"   => "error: cannot write into output file\n",
"ERR_SYS"        => "error: system error\n"
);

##########################################################################################################

/**
 * @brief   Prints the help and correctly exit program.
 * @return  void
 */
function call_help()
{
	echo "Script parse.php (in PHP 5.6) reads from stdin source code in IPPcode18, " .
	"checks lexical and syntax correctness of the code and prints on stdout XML " .
	"representation of the program. \n";
	echo "Usage: \n" .
	"  ./parse.php --help \n" .
	"  ./parse.php [--stats=\"file\"] [--comments] [--loc] \n";
	exit(0);
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
function test_args($argv)
{
	global $v_help;
	global $v_stats_comm;
	global $v_stats_loc;
	global $stats_file;

	$arg_help = false;
	$arg_stats = false;
	$arg_loc = false;
	$arg_comm = false;

	foreach ($argv as $value) {
			switch ($value) {
			case "--help":
				if ($arg_help)
					call_error("ERR_ARGS", RET_ARG);
				else
					$arg_help = true;
				break;

			case "--loc":
				if ($arg_loc)
					call_error("ERR_ARGS", RET_ARG);
				else
					$arg_loc = true;
				break;

			case "--comments":
				if ($arg_comm)
					call_error("ERR_ARGS", RET_ARG);
				else
					$arg_comm = true;
				break;

			default:
				if ($value == $argv[0])
					continue;
				else if (preg_match("/^--stats=(.*)$/", $value)) {
					if ($arg_stats)
						call_error("ERR_ARGS", RET_ARG);
					else {
						$arg_stats = true;
						$stats_file = $value;
					}
				}
				else
					call_error("ERR_ARGS", RET_ARG);
				break;
		}
	}

	// Prints help, "--help" has to be the only one argument
	if ($arg_help && !$arg_stats && !$arg_comm && !$arg_loc)
		$v_help = true;

	// Default variant, if there is "--stats" but not "--loc" neither "--comments"
	// don't print any statistics, behave as a default variant
	else if (!$arg_help && !$arg_comm && !$arg_loc)
		;

	// Extension, into statistic file prints both number of comments and
	// instructions (order will find out later)
	else if (!$arg_help && $arg_stats && $arg_comm && $arg_loc) {
		$v_stats_comm = true;
		$v_stats_loc = true;
	}

	// Extension, into statistic file prints number of comments
	else if (!$arg_help && $arg_stats && $arg_comm && !$arg_loc)
		$v_stats_comm = true;

	// Extension, into statistic file prints number of instructions
	else if (!$arg_help && $arg_stats && !$arg_comm && $arg_loc)
		$v_stats_loc = true;

	// Error, invalid arguments
	else
		call_error("ERR_ARGS", RET_ARG);
}

/**
 * @brief   Tests the correctness of introductury line (case insensitive).
 *          Ignores all whitespaces and comments before and behind the ".IPPcode18".
 * @return  void
 */
function test_intro()
{
	global $comm_cnt;
	$cont = true;

	while ($cont) {
		$line = trim(fgets(STDIN));

		if (feof(STDIN))
			call_error("ERR_INTRO", RET_SYN_LEX);

		if (empty($line))
			continue;

		if ($line[0] == "#") {
			$comm_cnt += 1;
			continue;
		}

		if (strpos($line, '#') == true) {
			$line = trim(substr($line, 0, strpos($line, "#")));
			$comm_cnt += 1;
		}

		if (strtolower($line) == ".ippcode18")
			$cont = false;
		else
			call_error("ERR_INTRO", RET_SYN_LEX);
	}
}

/**
 * @brief   Tests number of arguments of an instruction, if it's not correct, it ends the program.
 * @param   int   $expected  The expected number of args
 * @param   int   $real      The real number of args
 * @return  void
 */
function test_num_args_inst($expected, $real)
{
	if ($expected != $real)
		call_error("ERR_NUM_INS", RET_SYN_LEX);
}

/**
 * @brief   Tests lexical correctness of a variable.
 * @param   string   $str  The string to be test as a valid variable
 * @return  boolean
 */
function is_var($str)
{
	if (preg_match("/^(LF|TF|GF)@([a-zA-Z]|_|-|\\$|&|%|\*)([a-zA-Z]|[0-9]|_|-|\\$|&|%|\*)*\z/", $str))
		return true;
	else
		return false;
}

/**
 * @brief   Tests lexical correctness of a label.
 * @param   string   $str  The string to be test as a valid label
 * @return  boolean
 */
function is_label($str)
{
	if (preg_match("/^([a-zA-Z]|_|-|\\$|&|%|\*)([a-zA-Z]|[0-9]|_|-|\\$|&|%|\*)*\z/", $str))
		return true;
	else
		return false;
}

/**
 * @brief   Tests lexical correctness of a type (int, bool, string).
 * @param   string   $str  The string to be test as a valid type
 * @return  boolean
 */
function is_type($str)
{
	if ($str == "int" || $str == "bool" || $str == "string")
		return true;
	else
		return false;
}

/**
 * @brief   Tests lexical correctness of an int symbol.
 * @param   string  $str  The string to be test as a valid int symbol
 * @return  boolean
 */
function is_int_const($str)
{
	if (preg_match("/^int@.*$/", $str))
		return true;
	else
		return false;
}

/**
 * @brief   Tests lexical correctness of a bool symbol.
 * @param   string  $str  The string to be test as a valid bool symbol
 * @return  boolean
 */
function is_bool_const($str)
{
	if (preg_match("/^bool@(true|false)$/", $str))
		return true;
	else
		return false;
}

/**
 * @brief   Tests lexical correctness of a string symbol.
 * @param   string  $str  The string to be test as a valid string symbol
 * @return  boolean
 */
function is_string_const($str)
{
	if (preg_match('/^string@([^\\\]|\\\[0-9]{3})*$/', $str))
		return true;
	else
		return false;
}

/**
 * @brief   Tests lexical correctness of a variable, if it's not correct, it ends the program.
 * @param   string  $str  The string to be test as a valid variable
 * @return  void
 */
function test_var($str)
{
	if (!is_var($str))
		call_error("ERR_VAR", RET_SYN_LEX);
}

/**
 * @brief   Tests lexical correctness of a symbol (constant or variable), if it's not correct, it ends the program.
 * @param   string  $str  The string to be test as a valid symbol
 * @return  void
 */
function test_sym($str)
{
	if (!is_var($str) && !is_int_const($str) && !is_bool_const($str) && !is_string_const($str))
		call_error("ERR_SYM", RET_SYN_LEX);
}

/**
 * @brief   Tests lexical correctness of a label, if it's not correct, it ends the program.
 * @param   string  $str  The string to be test as a valid label
 * @return  void
 */
function test_label($str)
{
	if (!is_label($str))
		call_error("ERR_LABEL", RET_SYN_LEX);
}

/**
 * @brief   Tests if string is a type, if it's not, it ends the program.
 * @param   string  $str  The string to be test as a type
 * @return  void
 */
function test_type($str)
{
	if (!is_type($str))
		call_error("ERR_TYPE", RET_SYN_LEX);
}

/**
 * @brief   Generate XML for the instruction.
 * @param   string  $opcode  The opcode of instruction
 * @param   int     $order   The order of instruction
 * @param   array   $args    The array of arguments of instruction
 * @return  void
 */
function generate_xml($opcode, $order, $args)
{
	global $xmlwriter;

	if (xmlwriter_start_element($xmlwriter, "instruction") == false)
		call_error("ERR_SYS", RET_SYS);
	if (xmlwriter_write_attribute($xmlwriter, "order", $order) == false)
		call_error("ERR_SYS", RET_SYS);
	if (xmlwriter_write_attribute($xmlwriter, "opcode", $opcode) == false)
		call_error("ERR_SYS", RET_SYS);

	for ($i = 1; $i <= count($args); $i++) {
		if (is_type($args[$i-1]))
			$type = "type";
		else if (is_var($args[$i-1]))
			$type = "var";
		else if (is_label($args[$i-1]))
			$type = "label";
		else if (is_int_const($args[$i-1])) {
			$type = "int";
			$args[$i-1] = substr($args[$i-1], 4);
		}
		else if (is_bool_const($args[$i-1])) {
			$type = "bool";
			$args[$i-1] = substr($args[$i-1], 5);
		}
		else if (is_string_const($args[$i-1])) {
			$type = "string";
			$args[$i-1] = substr($args[$i-1], 7);
		}
		else
			call_error("ERR_UNEXPECTED", RET_SYN_LEX);

		if (xmlwriter_start_element($xmlwriter, "arg$i") == false)
			call_error("ERR_SYS", RET_SYS);
		if (xmlwriter_write_attribute($xmlwriter, "type", $type) == false)
			call_error("ERR_SYS", RET_SYS);
		if (xmlwriter_text($xmlwriter, $args[$i-1]) == false)
			call_error("ERR_SYS", RET_SYS);
		if (xmlwriter_end_element($xmlwriter) == false)
			call_error("ERR_SYS", RET_SYS);
	}

	if (xmlwriter_end_element($xmlwriter) == false)
		call_error("ERR_SYS", RET_SYS);
}

##########################################################################################################

// Checks command line arguments
test_args($argv);

// If "--help"
if ($v_help)
	call_help();

// Checks the introductury line of source code
test_intro();

// XML output starts
$xmlwriter = xmlwriter_open_uri("php://output");
if ($xmlwriter == false)
	call_error("ERR_SYS", RET_SYS);
if (xmlwriter_set_indent($xmlwriter , true) == false)
	call_error("ERR_SYS", RET_SYS);
if (xmlwriter_set_indent_string($xmlwriter, '  ') == false)
	call_error("ERR_SYS", RET_SYS);
if (xmlwriter_start_document($xmlwriter, "1.0", "UTF-8") == false)
	call_error("ERR_SYS", RET_SYS);
if (xmlwriter_start_element($xmlwriter, "program") == false)
	call_error("ERR_SYS", RET_SYS);
if (xmlwriter_write_attribute($xmlwriter, "language", "IPPcode18") == false)
	call_error("ERR_SYS", RET_SYS);

// Reads input line by line
while ($line = fgets(STDIN)) {

	// Deletes comments
	if ((strpos($line, '#') == true) || ($line[0] == "#")) {
		$line = trim(substr($line, 0, strpos($line, "#")));
		$comm_cnt += 1;
	}

	// Replace multiple whitespaces with a single space
	$line = trim(preg_replace('!\s+!', ' ', $line));

	// If empty line, read next
	if ($line == NULL)
		continue;

	// Increment of instruction counter
	$loc_cnt += 1;

	// Seperate "words" and store them into array
	$arr_line = preg_split("/[\s]+/", $line);

	// Store opcode of instruction into variable
	$inst = strtoupper($arr_line[0]);

	// Delete instruction from array, only arguments remain
	array_shift($arr_line);

	// Identify instruction and checks the arguments
	switch ($inst) {
		// <var> ... variable
		// <symb> ... variable or constant
		// <label> ... label
		// <type> ... type (int | bool | string)

		// MOVE <var> <symb>
		case 'MOVE':
			test_num_args_inst(2, count($arr_line));
			test_var($arr_line[0]);
			test_sym($arr_line[1]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// CREATEFRAME
		case 'CREATEFRAME':
			test_num_args_inst(0, count($arr_line));
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// PUSHFRAME
		case 'PUSHFRAME':
			test_num_args_inst(0, count($arr_line));
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// POPFRAME
		case 'POPFRAME':
			test_num_args_inst(0, count($arr_line));
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// DEFVAR <var>
		case 'DEFVAR':
			test_num_args_inst(1, count($arr_line));
			test_var($arr_line[0]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// CALL <label>
		case 'CALL':
			test_num_args_inst(1, count($arr_line));
			test_label($arr_line[0]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// RETURN
		case 'RETURN':
			test_num_args_inst(0, count($arr_line));
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// PUSHS <symb>
		case 'PUSHS':
			test_num_args_inst(1, count($arr_line));
			test_sym($arr_line[0]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// POPS <var>
		case 'POPS':
			test_num_args_inst(1, count($arr_line));
			test_var($arr_line[0]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// ADD <var> <symb1> <symb2>
		case 'ADD':
			test_num_args_inst(3, count($arr_line));
			test_var($arr_line[0]);
			test_sym($arr_line[1]);
			test_sym($arr_line[2]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// SUB <var> <symb1> <symb2>
		case 'SUB':
			test_num_args_inst(3, count($arr_line));
			test_var($arr_line[0]);
			test_sym($arr_line[1]);
			test_sym($arr_line[2]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// MUL <var> <symb1> <symb2>
		case 'MUL':
			test_num_args_inst(3, count($arr_line));
			test_var($arr_line[0]);
			test_sym($arr_line[1]);
			test_sym($arr_line[2]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// IDIV <var> <symb1> <symb2>
		case 'IDIV':
			test_num_args_inst(3, count($arr_line));
			test_var($arr_line[0]);
			test_sym($arr_line[1]);
			test_sym($arr_line[2]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// LT <var> <symb1> <symb2>
		case 'LT':
			test_num_args_inst(3, count($arr_line));
			test_var($arr_line[0]);
			test_sym($arr_line[1]);
			test_sym($arr_line[2]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// GT <var> <symb1> <symb2>
		case 'GT':
			test_num_args_inst(3, count($arr_line));
			test_var($arr_line[0]);
			test_sym($arr_line[1]);
			test_sym($arr_line[2]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// EQ <var> <symb1> <symb2>
		case 'EQ':
			test_num_args_inst(3, count($arr_line));
			test_var($arr_line[0]);
			test_sym($arr_line[1]);
			test_sym($arr_line[2]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// AND <var> <symb1> <symb2>
		case 'AND':
			test_num_args_inst(3, count($arr_line));
			test_var($arr_line[0]);
			test_sym($arr_line[1]);
			test_sym($arr_line[2]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// OR <var> <symb1> <symb2>
		case 'OR':
			test_num_args_inst(3, count($arr_line));
			test_var($arr_line[0]);
			test_sym($arr_line[1]);
			test_sym($arr_line[2]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// NOT <var> <symb1>
		case 'NOT':
			test_num_args_inst(2, count($arr_line));
			test_var($arr_line[0]);
			test_sym($arr_line[1]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// INT2CHAR <var> <symb>
		case 'INT2CHAR':
			test_num_args_inst(2, count($arr_line));
			test_var($arr_line[0]);
			test_sym($arr_line[1]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// STRI2INT <var> <symb1> <symb2>
		case 'STRI2INT':
			test_num_args_inst(3, count($arr_line));
			test_var($arr_line[0]);
			test_sym($arr_line[1]);
			test_sym($arr_line[2]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// READ <var> <type>
		case 'READ':
			test_num_args_inst(2, count($arr_line));
			test_var($arr_line[0]);
			test_type($arr_line[1]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// WRITE <symb>
		case 'WRITE':
			test_num_args_inst(1, count($arr_line));
			test_sym($arr_line[0]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// CONCAT <var> <symb1> <symb2>
		case 'CONCAT':
			test_num_args_inst(3, count($arr_line));
			test_var($arr_line[0]);
			test_sym($arr_line[1]);
			test_sym($arr_line[2]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// STRLEN <var> <symb>
		case 'STRLEN':
			test_num_args_inst(2, count($arr_line));
			test_var($arr_line[0]);
			test_sym($arr_line[1]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// GETCHAR <var> <symb1> <symb2>
		case 'GETCHAR':
			test_num_args_inst(3, count($arr_line));
			test_var($arr_line[0]);
			test_sym($arr_line[1]);
			test_sym($arr_line[2]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// SETCHAR <var> <symb1> <symb2>
		case 'SETCHAR':
			test_num_args_inst(3, count($arr_line));
			test_var($arr_line[0]);
			test_sym($arr_line[1]);
			test_sym($arr_line[2]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// TYPE <var> <symb>
		case 'TYPE':
			test_num_args_inst(2, count($arr_line));
			test_var($arr_line[0]);
			test_sym($arr_line[1]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// LABEL <label>
		case 'LABEL':
			test_num_args_inst(1, count($arr_line));
			test_label($arr_line[0]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// JUMP <label>
		case 'JUMP':
			test_num_args_inst(1, count($arr_line));
			test_label($arr_line[0]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// JUMPIFEQ <label> <symb1> <symb2>
		case 'JUMPIFEQ':
			test_num_args_inst(3, count($arr_line));
			test_label($arr_line[0]);
			test_sym($arr_line[1]);
			test_sym($arr_line[2]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// JUMPIFNEQ <label> <symb1> <symb2>
		case 'JUMPIFNEQ':
			test_num_args_inst(3, count($arr_line));
			test_label($arr_line[0]);
			test_sym($arr_line[1]);
			test_sym($arr_line[2]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// DPRINT <symb>
		case 'DPRINT':
			test_num_args_inst(1, count($arr_line));
			test_sym($arr_line[0]);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// BREAK
		case 'BREAK':
			test_num_args_inst(0, count($arr_line));
			generate_xml($inst, $loc_cnt, $arr_line);
			break;

		// Unknown instruction => syntax error
		default:
			call_error("ERR_UNK_INS", RET_SYN_LEX);
			generate_xml($inst, $loc_cnt, $arr_line);
			break;
	}
}

// XML output ends
if (xmlwriter_end_element($xmlwriter) == false)
	call_error("ERR_SYS", RET_SYS);
if (xmlwriter_end_document($xmlwriter) == false)
	call_error("ERR_SYS", RET_SYS);


// Extension
// Into statistic file prints only number of comments
if ($v_stats_comm) {
	$stats_file = substr($stats_file, 8);
	if ($stats_file == false)
		call_error("ERR_SYS", RET_SYS);
	if (file_put_contents($stats_file, $comm_cnt) == false)
		call_error("ERR_OUT_FILE", RET_OUT_FILE);
}

// Into statistic file prints only number of instructions
else if ($v_stats_loc) {
	$stats_file = substr($stats_file, 8);
	if ($stats_file == false)
		call_error("ERR_SYS", RET_SYS);
	if (file_put_contents($stats_file, $loc_cnt) == false)
		call_error("ERR_OUT_FILE", RET_OUT_FILE);
}

// Into statistic file prints number of comments and instructions (in the specified order)
else if ($v_stats_comm && $v_stats_loc) {
	$stats_file = substr($stats_file, 8);
	if ($stats_file == false)
		call_error("ERR_SYS", RET_SYS);
	if (array_search("--comments", $argv) > array_search("--loc", $argv)) {
		if (file_put_contents($stats_file, $loc_cnt) == false)
			call_error("ERR_OUT_FILE", RET_OUT_FILE);
		if (file_put_contents($stats_file, "\n", FILE_APPEND) == false)
			call_error("ERR_OUT_FILE", RET_OUT_FILE);
		if (file_put_contents($stats_file, $comm_cnt, FILE_APPEND) == false)
			call_error("ERR_OUT_FILE", RET_OUT_FILE);
	}
	else {
		if (file_put_contents($stats_file, $comm_cnt) == false)
			call_error("ERR_OUT_FILE", RET_OUT_FILE);
		if (file_put_contents($stats_file, "\n", FILE_APPEND) == false)
			call_error("ERR_OUT_FILE", RET_OUT_FILE);
		if (file_put_contents($stats_file, $loc_cnt, FILE_APPEND) == false)
			call_error("ERR_OUT_FILE", RET_OUT_FILE);
	}
}

// No statistics
else
	;

?>
