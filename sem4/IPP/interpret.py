
# VUT FIT
# IPP 2017/2018
# Project: Sada skriptu pro interpretaci nestrukturovaneho imperativniho jazyka IPPcode18
# Author: Vladimir Dusek, xdusek27 (2BIT)
# Date: 15/4/2018
# File: interpret.py

# Extension:
# STATI

# Execute
# ./interpret.py --help
# ./interpret.py --source="file" [--stats="file"] [--insts] [--vars]

# Errors
# 10 ... Chybejici parametr skriptu (je-li treba) nebo pouziti zakazane kombinace parametru.
# 11 ... Chyba pri otevirani vstupnich souboru (napr. neexistence, nedostatecne opravneni).
# 12 ... Chyba pri otevreni vystupnich souboru pro zapis (napr. nedostatecne opravneni).
# 31 ... Chybny XML format ve vstupnim souboru (soubor neni tzv. dobre formatovany,
#        nebo nema ocekavanou strukturu).
# 32 ... Chyba lexikalni nebo syntakticke analyzy textovych elementu a atributu ve vstupnim XML
#        souboru (napr. chybny lexem pro retezcovy literal, neznamy operacni kod apod.).
# 52 ... Chyba pri semantickych kontrolach vstupniho kodu v IPPcode18.
# 53 ... Behova chyba interpretace – spatne typy operandu.
# 54 ... Behova chyba interpretace – pristup k neexistujici promenne (ramec existuje).
# 55 ... Behova chyba interpretace – ramec neexistuje (napr. cteni z prazdneho zasobniku ramcu).
# 56 ... Behova chyba interpretace – chybejici hodnota (v promenne, na datovem zasobniku,
#        nebo v zasobniku volani).
# 57 ... Behova chyba interpretace – deleni nulou.
# 58 ... Behova chyba interpretace – chybna prace s retezcem.
# 99 ... Interni chyba (Neovlivnena vstupnimi soubory ci parametry prikazove radky. Napr. chyba
#        alokace pameti).

##########################################################################################################

import sys
import re
import getopt
import xml.etree.ElementTree as ET

# Constants for error return codes
CONST_RET_ARG      = 10
CONST_RET_IN_FILE  = 11
CONST_RET_OUT_FILE = 12
CONST_RET_XML      = 31
CONST_RET_LEX_SYN  = 32
CONST_RET_SEM      = 52
CONST_RET_OPERAND  = 53
CONST_RET_VAR      = 54
CONST_RET_FRAME    = 55
CONST_RET_VALUE    = 56
CONST_RET_DIV      = 57
CONST_RET_STRING   = 58
CONST_RET_SYS      = 99

# Global variables for counting instructions and variables (extension stats)
insts_cnt = 0
vars_cnt  = 0

##########################################################################################################

def print_help():
	"""
		Print help and correctly exit program
	"""
	print("Script interpret.py (in Python 3.6) reads from source file (specified " + \
	"as parameter --source=\"file_name\") XML representation of a program " + \
	"written in IPPcode18, checks lexical, syntax and semantic correctness " + \
	"of the code and execute individual commands. ")
	print("Usage: " + \
	"\n  ./interpret.py --help " + \
	"\n  ./interpret.py --source=\"file\" [--stats=\"file\"] [--insts] [--vars] ")
	exit(0)


def error(err_type, ret_code):
	"""
		Print error message and exit script with relevant error code
	"""
	err_msg = {"ERR_ARG"  : "error: unrecognized option\ntry \"./interpret.py --help\" for more information\n",
			"ERR_IN_FILE" : "error: input file cannot be opened\n",
			"ERR_OUT_FILE": "error: output file cannot be opened\n",
			"ERR_XML"     : "error: invalid format of input XML file\n",
			"ERR_LEX_SYN" : "lexical or syntax error of source code\n",
			"ERR_SEM"     : "semantic error of source code\n",
			"ERR_OPERAND" : "error: wrong types of operand\n",
			"ERR_VAR"     : "error: variable does not exist\n",
			"ERR_FRAME"   : "error: frame does not exist\n",
			"ERR_VALUE"   : "error: missing value\n",
			"ERR_DIV"     : "error: division by zero\n",
			"ERR_STRING"  : "error: invalid work with string\n",
			"ERR_SYS"     : "error: system error\n"}
	sys.stderr.write(err_msg[err_type])
	exit(ret_code)

##########################################################################################################

class Interpret:
	"""
		Execute all instructions and do the rest of syntactic and semantic controls
	"""
	# GF: dict
	# TF: dict
	# LF: dict
	# list_insts: list
	# list_labels: list
	# stack_frames: list
	# stack_data: list
	# stack_calls: list

	def __init__(self, list_insts):
		"""
			Initialization of attributes
		"""
		self.GF = dict()
		self.TF = None
		self.LF = None
		self.list_insts = list_insts
		self.list_labels = list()
		self.stack_frames = list()
		self.stack_data = list()
		self.stack_calls = list()
		for inst in self.list_insts:
			if inst.opcode == "LABEL":
				self.list_labels.append(inst)

	def test_frame_def(self, frame):
		"""
			Test if frame is defined
		"""
		if frame == "TF":
			if self.TF == None:
				error("ERR_FRAME", CONST_RET_FRAME)
		if frame == "LF":
			if self.LF == None:
				error("ERR_FRAME", CONST_RET_FRAME)

	def test_var_def(self, frame, identifier):
		"""
			Test if variable is defined in the frame
		"""
		if frame == "GF":
			if identifier not in self.GF.keys():
				error("ERR_VAR", CONST_RET_VAR)
		elif frame == "TF":
			if identifier not in self.TF.keys():
				error("ERR_VAR", CONST_RET_VAR)
		elif frame == "LF":
			if identifier not in self.LF.keys():
				error("ERR_VAR", CONST_RET_VAR)

	def test_sym_int(self, arg):
		"""
			Test if argument is an integer symbol (variable of integer
			type or integer constant)
		"""
		if arg.is_var():
			if self.get_type_from_var(arg.content[:2], arg.content[3:]) != "int":
				error("ERR_OPERAND", CONST_RET_OPERAND)
		elif not arg.is_int():
			error("ERR_OPERAND", CONST_RET_OPERAND)

	def test_sym_bool(self, arg):
		"""
			Test if argument is a bool symbol (variable of bool
			type or bool constant)
		"""
		if arg.is_var():
			if self.get_type_from_var(arg.content[:2], arg.content[3:]) != "bool":
				error("ERR_OPERAND", CONST_RET_OPERAND)
		elif not arg.is_bool():
			error("ERR_OPERAND", CONST_RET_OPERAND)

	def test_sym_string(self, arg):
		"""
			Test if argument is a string symbol (variable of string
			type or string constant)
		"""
		if arg.is_var():
			if self.get_type_from_var(arg.content[:2], arg.content[3:]) != "string":
				error("ERR_OPERAND", CONST_RET_OPERAND)
		elif not arg.is_string():
			error("ERR_OPERAND", CONST_RET_OPERAND)

	def get_val_from_var(self, frame, identifier):
		"""
			Return value of the variable (frame[identifier])
		"""
		try:
			if frame == "GF":
				return self.GF[identifier].value
			elif frame == "TF":
				return self.TF[identifier].value
			elif frame == "LF":
				return self.LF[identifier].value
		except KeyError:
			error("ERR_SEM", CONST_RET_SEM)
		except TypeError:
			return ""

	def get_type_from_var(self, frame, identifier):
		"""
			Return type of the variable (frame[identifier])
		"""
		try:
			if frame == "GF":
				return self.GF[identifier].type
			elif frame == "TF":
				return self.TF[identifier].type
			elif frame == "LF":
				return self.LF[identifier].type
		except KeyError:
			error("ERR_SEM", CONST_RET_SEM)
		except TypeError:
			return None

	def get_val_from_sym(self, arg):
		"""
			Return value of the symbol (variable or constant)
		"""
		if arg.is_var():
			return str(self.get_val_from_var(arg.content[:2], arg.content[3:]))
		elif arg.is_int() or arg.is_string() or arg.is_bool():
			return str(arg.content)
		else:
			error("ERR_LEX_SYN", CONST_RET_LEX_SYN)

	def get_type_from_sym(self, arg):
		"""
			Return type (int, bool, string) of the symbol (variable or constant)
		"""
		if arg.is_var():
			return self.get_type_from_var(arg.content[:2], arg.content[3:])
		elif arg.is_int():
			return "int"
		elif arg.is_bool():
		 	return "bool"
		elif arg.is_string():
			return "string"
		else:
			error("ERR_LEX_SYN", CONST_RET_LEX_SYN)

	def store_val_into_var(self, frame, identifier, value, type):
		"""
			Store value of the type into the variable in the frame
		"""
		if frame == "GF":
			self.GF[identifier].value = value
			self.GF[identifier].type = type
		elif frame == "TF":
			self.TF[identifier].value = value
			self.TF[identifier].type = type
		elif frame == "LF":
			self.LF[identifier].value = value
			self.LF[identifier].type = type
		else:
			error("ERR_SEM", CONST_RET_SEM)

	def get_inst_by_order(self, order):
		"""
			Return instruction of the order
		"""
		for inst in self.list_insts:
			if inst.order == order:
				return inst

	def MOVE(self, inst):
		inst.test_num_args(2)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg1.test_var()
		arg2.test_sym()
		arg2_id = arg2.content[3:]
		arg2_frame = arg2.content[:2]
		if arg2.is_var():
			self.test_frame_def(arg2_frame)
			self.test_var_def(arg2_frame, arg2_id)
			value = self.get_val_from_var(arg2_frame, arg2_id)
			type = self.get_type_from_var(arg2_frame, arg2_id)
		else:
			value = arg2.content
			type = arg2.type
		arg1_id = arg1.content[3:]
		arg1_frame = arg1.content[:2]
		self.test_frame_def(arg1_frame)
		self.test_var_def(arg1_frame, arg1_id)
		self.store_val_into_var(arg1_frame, arg1_id, str(value), type)
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def CREATEFRAME(self, inst):
		inst.test_num_args(0)
		self.TF = dict()
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def PUSHFRAME(self, inst):
		inst.test_num_args(0)
		if self.TF != None:
			self.stack_frames.append(self.TF)
			self.LF = self.stack_frames[-1]
			self.TF = None
		else:
			error("ERR_FRAME", CONST_RET_FRAME)
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def POPFRAME(self, inst):
		inst.test_num_args(0)
		try:
			self.TF = self.stack_frames.pop()
		except IndexError:
			error("ERR_FRAME", CONST_RET_FRAME)
		try:
			self.LF = self.stack_frames[-1]
		except IndexError:
			self.LF = None
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def DEFVAR(self, inst):
		inst.test_num_args(1)
		arg1 = inst.args[0]
		arg1.test_var()
		var = Variable(None, None)
		arg1_id = arg1.content[3:]
		arg1_frame = arg1.content[:2]
		self.test_frame_def(arg1_frame)
		if arg1.is_gf_var():
			self.GF[arg1_id] = var
		elif arg1.is_tf_var():
			self.TF[arg1_id] = var
		elif arg1.is_lf_var():
			self.LF[arg1_id] = var
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def CALL(self, inst):
		inst.test_num_args(1)
		arg1 = inst.args[0]
		arg1.test_label()
		self.stack_calls.append(int(inst.order) + 1)
		for inst_label in self.list_labels:
			if inst_label.args[0].content == arg1.content:
				return self.get_inst_by_order(str(int(inst_label.order) + 1))
		error("ERR_SEM", CONST_RET_SEM)

	def RETURN(self, inst):
		inst.test_num_args(0)
		try:
			order = self.stack_calls.pop()
		except IndexError:
			error("ERR_VALUE", CONST_RET_VALUE)
		return self.get_inst_by_order(str(order))

	def PUSHS(self, inst):
		inst.test_num_args(1)
		arg1 = inst.args[0]
		arg1.test_sym()
		arg1_value = self.get_val_from_sym(arg1)
		arg1_type = self.get_type_from_sym(arg1)
		elem = Variable(arg1_type, arg1_value)
		self.stack_data.append(elem)
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def POPS(self, inst):
		inst.test_num_args(1)
		if not self.stack_data:
			error("ERR_VALUE", CONST_RET_VALUE)
		arg1 = inst.args[0]
		arg1.test_var()
		arg1_id = arg1.content[3:]
		arg1_frame = arg1.content[:2]
		elem = self.stack_data.pop()
		self.store_val_into_var(arg1_frame, arg1_id, elem.value, elem.type)
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def ADD(self, inst):
		inst.test_num_args(3)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg3 = inst.args[2]
		arg1.test_var()
		self.test_sym_int(arg2)
		self.test_sym_int(arg3)
		a = int(self.get_val_from_sym(arg2))
		b = int(self.get_val_from_sym(arg3))
		arg1_id = arg1.content[3:]
		arg1_frame = arg1.content[:2]
		self.test_frame_def(arg1_frame)
		self.test_var_def(arg1_frame, arg1_id)
		self.store_val_into_var(arg1_frame, arg1_id, str(a + b), "int")
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def SUB(self, inst):
		inst.test_num_args(3)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg3 = inst.args[2]
		arg1.test_var()
		self.test_sym_int(arg2)
		self.test_sym_int(arg3)
		a = int(self.get_val_from_sym(arg2))
		b = int(self.get_val_from_sym(arg3))
		arg1_id = arg1.content[3:]
		arg1_frame = arg1.content[:2]
		self.test_frame_def(arg1_frame)
		self.test_var_def(arg1_frame, arg1_id)
		self.store_val_into_var(arg1_frame, arg1_id, str(a - b), "int")
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def MUL(self, inst):
		inst.test_num_args(3)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg3 = inst.args[2]
		arg1.test_var()
		self.test_sym_int(arg2)
		self.test_sym_int(arg3)
		a = int(self.get_val_from_sym(arg2))
		b = int(self.get_val_from_sym(arg3))
		arg1_id = arg1.content[3:]
		arg1_frame = arg1.content[:2]
		self.test_frame_def(arg1_frame)
		self.test_var_def(arg1_frame, arg1_id)
		self.store_val_into_var(arg1_frame, arg1_id, str(a * b), "int")
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def IDIV(self, inst):
		inst.test_num_args(3)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg3 = inst.args[2]
		arg1.test_var()
		self.test_sym_int(arg2)
		self.test_sym_int(arg3)
		a = int(self.get_val_from_sym(arg2))
		b = int(self.get_val_from_sym(arg3))
		arg1_id = arg1.content[3:]
		arg1_frame = arg1.content[:2]
		self.test_frame_def(arg1_frame)
		self.test_var_def(arg1_frame, arg1_id)
		try:
			self.store_val_into_var(arg1_frame, arg1_id, str(a // b), "int")
		except ZeroDivisionError:
			error("ERR_DIV", CONST_RET_DIV)
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def LT(self, inst):
		inst.test_num_args(3)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg3 = inst.args[2]
		arg1.test_var()
		arg2.test_sym()
		arg3.test_sym()
		arg2_val = self.get_val_from_sym(arg2)
		arg2_type = self.get_type_from_sym(arg2)
		arg3_val = self.get_val_from_sym(arg3)
		arg3_type = self.get_type_from_sym(arg3)
		arg1_frame = arg1.content[:2]
		arg1_id = arg1.content[3:]
		if arg2_type != arg3_type:
			error("ERR_OPERAND", CONST_RET_OPERAND)
		if arg2_type == "int":
			arg2_val = int(arg2_val)
			arg3_val = int(arg3_val)
		self.test_frame_def(arg1_frame)
		self.test_var_def(arg1_frame, arg1_id)
		self.store_val_into_var(arg1_frame, arg1_id, str(arg2_val < arg3_val).lower(), "bool")
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def GT(self, inst):
		inst.test_num_args(3)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg3 = inst.args[2]
		arg1.test_var()
		arg2.test_sym()
		arg3.test_sym()
		arg2_val = self.get_val_from_sym(arg2)
		arg2_type = self.get_type_from_sym(arg2)
		arg3_val = self.get_val_from_sym(arg3)
		arg3_type = self.get_type_from_sym(arg3)
		arg1_frame = arg1.content[:2]
		arg1_id = arg1.content[3:]
		if arg2_type != arg3_type:
			error("ERR_OPERAND", CONST_RET_OPERAND)
		if arg2_type == "int":
			arg2_val = int(arg2_val)
			arg3_val = int(arg3_val)
		self.test_frame_def(arg1_frame)
		self.test_var_def(arg1_frame, arg1_id)
		self.store_val_into_var(arg1_frame, arg1_id, str(arg2_val > arg3_val).lower(), "bool")
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def EQ(self, inst):
		inst.test_num_args(3)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg3 = inst.args[2]
		arg1.test_var()
		arg2.test_sym()
		arg3.test_sym()
		arg2_val = self.get_val_from_sym(arg2)
		arg2_type = self.get_type_from_sym(arg2)
		arg3_val = self.get_val_from_sym(arg3)
		arg3_type = self.get_type_from_sym(arg3)
		arg1_frame = arg1.content[:2]
		arg1_id = arg1.content[3:]
		if arg2_type != arg3_type:
			error("ERR_OPERAND", CONST_RET_OPERAND)
		if arg2_type == "int":
			arg2_val = int(arg2_val)
			arg3_val = int(arg3_val)
		self.test_frame_def(arg1_frame)
		self.test_var_def(arg1_frame, arg1_id)
		self.store_val_into_var(arg1_frame, arg1_id, str(arg2_val == arg3_val).lower(), "bool")
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def AND(self, inst):
		inst.test_num_args(3)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg3 = inst.args[2]
		arg1.test_var()
		arg2.test_sym()
		arg3.test_sym()
		self.test_sym_bool(arg2)
		self.test_sym_bool(arg3)
		arg2_val = self.get_val_from_sym(arg2)
		arg3_val = self.get_val_from_sym(arg3)
		if arg2_val.lower() == "true":
			arg2_val = True
		else:
			arg2_val = False
		if arg3_val.lower() == "true":
			arg3_val = True
		else:
			arg3_val = False
		arg1_frame = arg1.content[:2]
		arg1_id = arg1.content[3:]
		self.test_frame_def(arg1_frame)
		self.test_var_def(arg1_frame, arg1_id)
		self.store_val_into_var(arg1_frame, arg1_id, str(arg2_val and arg3_val).lower(), "bool")
		return self.get_inst_by_order(str(int(inst.order) + 1))


	def OR(self, inst):
		inst.test_num_args(3)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg3 = inst.args[2]
		arg1.test_var()
		arg2.test_sym()
		arg3.test_sym()
		self.test_sym_bool(arg2)
		self.test_sym_bool(arg3)
		arg2_val = self.get_val_from_sym(arg2)
		arg3_val = self.get_val_from_sym(arg3)
		if arg2_val.lower() == "true":
			arg2_val = True
		else:
			arg2_val = False
		if arg3_val.lower() == "true":
			arg3_val = True
		else:
			arg3_val = False
		arg1_frame = arg1.content[:2]
		arg1_id = arg1.content[3:]
		self.test_frame_def(arg1_frame)
		self.test_var_def(arg1_frame, arg1_id)
		self.store_val_into_var(arg1_frame, arg1_id, str(arg2_val or arg3_val).lower(), "bool")
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def NOT(self, inst):
		inst.test_num_args(2)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg1.test_var()
		arg2.test_sym()
		self.test_sym_bool(arg2)
		arg2_val = self.get_val_from_sym(arg2)
		if arg2_val.lower() == "true":
			arg2_val = True
		else:
			arg2_val = False
		arg1_frame = arg1.content[:2]
		arg1_id = arg1.content[3:]
		self.test_frame_def(arg1_frame)
		self.test_var_def(arg1_frame, arg1_id)
		self.store_val_into_var(arg1_frame, arg1_id, str(not arg2_val).lower(), "bool")
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def INT2CHAR(self, inst):
		inst.test_num_args(2)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg1.test_var()
		arg2.test_sym()
		self.test_sym_int(arg2)
		arg2_val = self.get_val_from_sym(arg2)
		arg1_frame = arg1.content[:2]
		arg1_id = arg1.content[3:]
		self.test_frame_def(arg1_frame)
		self.test_var_def(arg1_frame, arg1_id)
		try:
			self.store_val_into_var(arg1_frame, arg1_id, str(chr(int(arg2_val))), "string")
		except ValueError:
			error("ERR_STRING", CONST_RET_STRING)
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def STRI2INT(self, inst):
		inst.test_num_args(3)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg3 = inst.args[2]
		arg1.test_var()
		arg2.test_sym()
		arg3.test_sym()
		self.test_sym_string(arg2)
		self.test_sym_int(arg3)
		string = self.get_val_from_sym(arg2)
		idx = int(self.get_val_from_sym(arg3))
		arg1_frame = arg1.content[:2]
		arg1_id = arg1.content[3:]
		self.test_frame_def(arg1_frame)
		self.test_var_def(arg1_frame, arg1_id)
		try:
			self.store_val_into_var(arg1_frame, arg1_id, str(ord(string[idx])), "int")
		except IndexError:
			error("ERR_STRING", CONST_RET_STRING)
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def READ(self, inst):
		inst.test_num_args(2)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg1.test_var()
		arg2.test_type()
		input_type = arg2.content
		if (input_type == "int"):
			try:
				input_text = input()
				value = int(input_text)
			except (ValueError, EOFError):
				value = 0
		elif (input_type == "string"):
			try:
				input_text = input()
				value = str(input_text)
			except (ValueError, EOFError):
				value = ""
		elif (input_type == "bool"):
			try:
				input_text = input()
				if (input_text.lower() == "true"):
					value = "true"
				else:
					value = "false"
			except (ValueError, EOFError):
				value = "false"
		arg1_frame = arg1.content[:2]
		arg1_id = arg1.content[3:]
		self.test_frame_def(arg1_frame)
		self.test_var_def(arg1_frame, arg1_id)
		self.store_val_into_var(arg1_frame, arg1_id, str(value), input_type)
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def WRITE(self, inst):
		inst.test_num_args(1)
		arg1 = inst.args[0]
		arg1.test_sym()
		if self.get_type_from_sym(arg1) == "int":
				print(int(self.get_val_from_sym(arg1)))
		else:
			print(self.get_val_from_sym(arg1))
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def CONCAT(self, inst):
		inst.test_num_args(3)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg3 = inst.args[2]
		arg1.test_var()
		arg2.test_sym()
		arg3.test_sym()
		self.test_sym_string(arg2)
		self.test_sym_string(arg3)
		str1 = self.get_val_from_sym(arg2)
		str2 = self.get_val_from_sym(arg3)
		arg1_frame = arg1.content[:2]
		arg1_id = arg1.content[3:]
		self.test_frame_def(arg1_frame)
		self.test_var_def(arg1_frame, arg1_id)
		self.store_val_into_var(arg1_frame, arg1_id, str(str1 + str2), "string")
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def STRLEN(self, inst):
		inst.test_num_args(2)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg1.test_var()
		arg2.test_sym()
		self.test_sym_string(arg2)
		string = self.get_val_from_sym(arg2)
		arg1_frame = arg1.content[:2]
		arg1_id = arg1.content[3:]
		self.test_frame_def(arg1_frame)
		self.test_var_def(arg1_frame, arg1_id)
		self.store_val_into_var(arg1_frame, arg1_id, len(string), "int")
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def GETCHAR(self, inst):
		inst.test_num_args(3)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg3 = inst.args[2]
		arg1.test_var()
		arg2.test_sym()
		arg3.test_sym()
		self.test_sym_string(arg2)
		self.test_sym_int(arg3)
		string = self.get_val_from_sym(arg2)
		idx = int(self.get_val_from_sym(arg3))
		arg1_frame = arg1.content[:2]
		arg1_id = arg1.content[3:]
		self.test_frame_def(arg1_frame)
		self.test_var_def(arg1_frame, arg1_id)
		try:
			self.store_val_into_var(arg1_frame, arg1_id, str(string[idx]), "string")
		except IndexError:
			error("ERR_STRING", CONST_RET_STRING)
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def SETCHAR(self, inst):
		inst.test_num_args(3)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg3 = inst.args[2]
		arg1.test_var()
		arg2.test_sym()
		arg3.test_sym()
		self.test_sym_string(arg1)
		self.test_sym_int(arg2)
		self.test_sym_string(arg3)
		try:
			arg1_frame = arg1.content[:2]
			arg1_id = arg1.content[3:]
			self.test_frame_def(arg1_frame)
			self.test_var_def(arg1_frame, arg1_id)
			string = self.get_val_from_sym(arg1)
			idx = int(self.get_val_from_sym(arg2))
			char = self.get_val_from_sym(arg3)
			string = list(string)
			string[idx] = char[0]
			string = "".join(string)
			self.store_val_into_var(arg1_frame, arg1_id, string, "string")
		except IndexError:
			error("ERR_STRING", CONST_RET_STRING)
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def TYPE(self, inst):
		inst.test_num_args(2)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg1.test_var()
		arg2.test_sym()
		data_type = self.get_type_from_sym(arg2)
		if data_type == None:
			data_type = ""
		arg1_frame = arg1.content[:2]
		arg1_id = arg1.content[3:]
		self.test_frame_def(arg1_frame)
		self.test_var_def(arg1_frame, arg1_id)
		self.store_val_into_var(arg1_frame, arg1_id, data_type, "string")
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def LABEL(self, inst):
		inst.test_num_args(1)
		arg1 = inst.args[0]
		arg1.test_label()
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def JUMP(self, inst):
		inst.test_num_args(1)
		arg1 = inst.args[0]
		arg1.test_label()
		for inst_label in self.list_labels:
			if inst_label.args[0].content == arg1.content:
				return self.get_inst_by_order(str(int(inst_label.order) + 1))
		error("ERR_SEM", CONST_RET_SEM)

	def JUMPIFEQ(self, inst):
		inst.test_num_args(3)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg3 = inst.args[2]
		arg1.test_label()
		arg2.test_sym()
		arg3.test_sym()
		arg2_val = self.get_val_from_sym(arg2)
		arg2_type = self.get_type_from_sym(arg2)
		arg3_val = self.get_val_from_sym(arg3)
		arg3_type = self.get_type_from_sym(arg3)
		if arg2_type != arg3_type:
			error("ERR_OPERAND", CONST_RET_OPERAND)
		if arg2_type == "int":
			arg2_val = int(arg2_val)
			arg3_val = int(arg3_val)
		if arg2_type == "bool":
			arg2_val = arg2_val.lower()
			arg3_val = arg3_val.lower()
		if arg2_val == arg3_val:
			for inst_label in self.list_labels:
				if inst_label.args[0].content == arg1.content:
					return self.get_inst_by_order(str(int(inst_label.order) + 1))
			error("ERR_SEM", CONST_RET_SEM)
		else:
			return self.get_inst_by_order(str(int(inst.order) + 1))

	def JUMPIFNEQ(self, inst):
		inst.test_num_args(3)
		arg1 = inst.args[0]
		arg2 = inst.args[1]
		arg3 = inst.args[2]
		arg1.test_label()
		arg2.test_sym()
		arg3.test_sym()
		arg2_val = self.get_val_from_sym(arg2)
		arg2_type = self.get_type_from_sym(arg2)
		arg3_val = self.get_val_from_sym(arg3)
		arg3_type = self.get_type_from_sym(arg3)
		if arg2_type != arg3_type:
			error("ERR_OPERAND", CONST_RET_OPERAND)
		if arg2_type == "int":
			arg2_val = int(arg2_val)
			arg3_val = int(arg3_val)
		if arg2_type == "bool":
			arg2_val = arg2_val.lower()
			arg3_val = arg3_val.lower()
		if arg2_val != arg3_val:
			for inst_label in self.list_labels:
				if inst_label.args[0].content == arg1.content:
					return self.get_inst_by_order(str(int(inst_label.order) + 1))
			error("ERR_SEM", CONST_RET_SEM)
		else:
			return self.get_inst_by_order(str(int(inst.order) + 1))

	def DPRINT(self, inst):
		inst.test_num_args(1)
		arg1 = inst.args[0]
		arg1.test_sym()
		value = self.get_val_from_sym(arg1)
		sys.stderr.write(value)
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def BREAK(self, inst):
		inst.test_num_args(0)
		sys.stderr.write("order = " + inst.order + "\n")
		sys.stderr.write("GF = " + str(self.GF) + "\n")
		sys.stderr.write("TF = " + str(self.TF) + "\n")
		sys.stderr.write("LF = " + str(self.LF) + "\n")
		return self.get_inst_by_order(str(int(inst.order) + 1))

	def execute(self):
		"""
			Execute interpret for every instruction and count statistics
		"""
		global insts_cnt
		global vars_cnt
		local_vars_cnt = 0
		try:
			inst = list_insts[0]
		except IndexError:
			return
		while inst != None:
			try:
				inst = getattr(self, inst.opcode)(inst)
			except AttributeError:
				error("ERR_LEX_SYN", CONST_RET_LEX_SYN)
			insts_cnt += 1
			try:
				for var in self.GF:
					val = self.get_val_from_var("GF", var)
					if val != None:
						local_vars_cnt += 1
				for var in self.TF:
					val = self.get_val_from_var("TF", var)
					if val != None:
						local_vars_cnt += 1
				for var in self.GF:
					val = self.get_val_from_var("LF", var)
					if val != None:
						local_vars_cnt += 1
			except TypeError:
				...
			if local_vars_cnt > vars_cnt:
				vars_cnt = local_vars_cnt
			local_vars_cnt = 0


class ParserXML:
	"""
		Parse XML file into list of instructions
	"""
	# order_cnt: int
	# list_insts: list
	# file: str

	def __init__(self, file):
		"""
			Initialization of attributes
		"""
		self.order_cnt = 1
		self.list_insts = []
		self.file = file

	def test_read_file(self):
		"""
			Test if input XML file can be open for reading
		"""
		try:
			f = open(self.file, "r")
			f.closed
		except IOError:
			error("ERR_IN_FILE", CONST_RET_IN_FILE)

	def test_syntax_xml(self):
		"""
			Test syntax of input XML and return tree
		"""
		try:
			tree = ET.parse(self.file)
			return tree
		except ET.ParseError as code:
			error("ERR_XML", CONST_RET_XML)

	def get_root(self, tree):
		"""
			Return root element
		"""
		root = tree.getroot()
		if root.tag != 'program':
			error("ERR_XML", CONST_RET_XML)
		return root

	def test_root_attr(self, root):
		"""
			Test root element attributes
		"""
		for elem_arg in root.keys():
			if elem_arg == 'language':
				if root.attrib['language'] != 'IPPcode18':
					error("ERR_XML", CONST_RET_XML)
			elif (elem_arg == 'name') or (elem_arg == 'description'):
				continue
			else:
				error("ERR_XML", CONST_RET_XML)
		if 'language' not in root.keys():
			error("ERR_XML", CONST_RET_XML)

	def get_inst(self, child):
		"""
			Test validity of instruction
		"""
		if child.tag == 'instruction':
			args = []
			for grand_child in child:
				for elem_arg in grand_child.keys():
					if elem_arg != 'type':
						error("ERR_XML", CONST_RET_XML)
				if 'type' not in grand_child.keys():
					error("ERR_XML", CONST_RET_XML)
				if grand_child.attrib['type'] == "string":
					string = grand_child.text
					if string == None:
						string = ""
					match_object = re.finditer(r'\\[0-9]{3}', string)
					for elem in match_object:
						idx = elem.span()
						thing = elem.group()
						string = string.replace(thing, chr(int(thing[1:])))
					grand_child.text = string
				if grand_child.tag == 'arg1':
					arg = Argument(type = grand_child.attrib['type'], content = grand_child.text)
					args.append(arg)
				elif grand_child.tag == 'arg2':
					arg = Argument(type = grand_child.attrib['type'], content = grand_child.text)
					args.append(arg)
				elif grand_child.tag == 'arg3':
					arg = Argument(type = grand_child.attrib['type'], content = grand_child.text)
					args.append(arg)
				else:
					error("ERR_XML", CONST_RET_XML)
			for elem_arg in child.keys():
				if (elem_arg != 'order') and (elem_arg != 'opcode'):
					error("ERR_XML", CONST_RET_XML)
			if ('order' not in child.keys()) or ('opcode' not in child.keys()):
				error("ERR_XML", CONST_RET_XML)
			if (self.order_cnt != int(child.attrib['order'])):
				error("ERR_XML", CONST_RET_XML)
			self.order_cnt += 1
			inst = Instruction(child.attrib['order'], child.attrib['opcode'], args)
			return inst
		else:
			error("ERR_XML", CONST_RET_XML)

	def get_list_insts(self, root):
		"""
			Return list of instructions
		"""
		list_insts = []
		for child in root:
			inst = self.get_inst(child)
			list_insts.append(inst)
		return list_insts

	def parse_xml(self):
		"""
			Parse XML file into list of instruction
		"""
		self.test_read_file()
		tree = self.test_syntax_xml()
		root = self.get_root(tree)
		self.test_root_attr(root)
		list_insts = self.get_list_insts(root)
		return list_insts


class Config:
	"""
		Class for configuration of the script
	"""
	# help: bool
	# source: bool
	# stats: bool
	# insts: bool
	# vars: bool
	# source_file: str
	# stats_file: str
	# stats_order: list

	def __init__(self):
		"""
			Initialization of attributes
		"""
		self.help = False
		self.source = False
		self.stats = False
		self.insts = False
		self.vars = False
		self.source_file = ""
		self.stats_file = ""
		self.stats_order = list()

	def parse_options(self, argv):
		"""
			Tests correctness of the command line arguments
		"""
		shortopts = ""
		longopts = 'help source= stats= insts vars'.split()

		# Catch getopt errors
		try:
			opts = getopt.gnu_getopt(argv, shortopts, longopts)
		except getopt.GetoptError as err:
			error("ERR_ARG", CONST_RET_ARG)

		# # Catch invalid arguments
		# if opts[1] != ['interpret.py']:
		# 	error("ERR_ARG", CONST_RET_ARG)

		# Catch duplicity arguments
		for elem in opts[0]:
			if elem[0] == '--help':
				if self.help:
					error("ERR_ARG", CONST_RET_ARG)
				self.help = True

			elif elem[0] == '--source':
				if self.source:
					error("ERR_ARG", CONST_RET_ARG)
				self.source = True
				self.source_file = elem[1]

			elif elem[0] == '--stats':
				if self.stats:
					error("ERR_ARG", CONST_RET_ARG)
				self.stats = True
				self.stats_file = elem[1]

			elif elem[0] == '--insts':
				if self.insts:
					error("ERR_ARG", CONST_RET_ARG)
				self.insts = True
				self.stats_order.append('--insts')

			elif elem[0] == '--vars':
				if self.vars:
					error("ERR_ARG", CONST_RET_ARG)
				self.vars = True
				self.stats_order.append('--vars')

		# When '--insts' or '--vars' is set, there has to be '--stats="file"'
		if (self.insts or self.vars) and not self.stats:
			error("ERR_ARG", CONST_RET_ARG)

		# There has to be '--source="file"'
		if (not self.source) and (not self.help):
			error("ERR_ARG", CONST_RET_ARG)

		# When there is '--help' it has to be alone
		if self.help and (len(opts[0]) != 1):
			error("ERR_ARG", CONST_RET_ARG)

	def print_stati(self):
		"""
			Print statistics
		"""
		global insts_cnt
		global vars_cnt
		try:
			f = open(self.stats_file, 'w')
			if self.insts and self.vars:
				if self.stats_order[0] == "--insts":
					f.write(str(insts_cnt) + "\n")
					f.write(str(vars_cnt) + "\n")
				else:
					f.write(str(vars_cnt) + "\n")
					f.write(str(insts_cnt) + "\n")
			elif self.insts:
				f.write(str(insts_cnt) + "\n")
			elif self.vars:
				f.write(str(vars_cnt) + "\n")
			f.closed
		except IOError:
			error("ERR_OUT_FILE", CONST_RET_OUT_FILE)

	def __str__(self):
		"""
			Define string conversion (for debugging)
		"""
		return "CONFIGURATION:" + \
		"\nhelp_exists = " + str(self.help) + \
		"\nsource_exists = " + str(self.source) + \
		"\nstats_exists = " + str(self.stats) + \
		"\ninsts_exists = " + str(self.insts) + \
		"\nvars_exists = " + str(self.vars) + \
		"\nsource_file = " + str(self.source_file) + \
		"\nstats_file = " + str(self.stats_file) + \
		"\nstats_order = " + str(self.stats_order) + "\n"


class Variable:
	"""
		Class for representing variable
	"""
	# type: str
	# value: str

	def __init__(self, type, value):
		"""
			Initialization of attributes
		"""
		self.type = type
		self.value = value

	def __str__(self):
		"""
			Define string conversion (for debugging)
		"""
		return "VARIABLE:" + \
		"\ntype = " + str(self.type) + \
		"\nvalue = " + str(self.value) + "\n"


class Argument:
	"""
		Class for representing argument of instruction
	"""
	# type: str
	# content: str

	def __init__(self, type="", content=""):
		"""
			Initialization of attributes
		"""
		self.type = type
		if content == None:
			content = ""
		self.content = content

	def is_var(self):
		"""
			Return True if argument is variable, otherwise return False
		"""
		if self.type != 'var':
			return False
		if not re.match(r'^(LF|TF|GF)@([a-zA-Z]|_|-|\$|&|%|\*)([a-zA-Z]|[0-9]|_|-|\$|&|%|\*)*$', self.content):
			return False
		return True

	def is_label(self):
		"""
			Return True if argument is label, otherwise return False
		"""
		if self.type != 'label':
			return False
		if not re.match(r'^([a-zA-Z]|_|-|\$|&|%|\*)([a-zA-Z]|[0-9]|_|-|\$|&|%|\*)*$', self.content):
			return False
		return True

	def is_type(self):
		"""
			Return True if argument is type, otherwise return False
		"""
		if self.type != 'type':
			return False
		if not re.match(r'^(int|bool|string)$', self.content):
			return False
		return True

	def is_int(self):
		"""
			Return True if argument is int const, otherwise return False
		"""
		if self.type != 'int':
			return False
		if not re.match(r'^(-|\+|0|[1-9])[0-9]*$', self.content):
			return False
		return True

	def is_bool(self):
		"""
			Return True if argument is bool const, otherwise return False
		"""
		if self.type != 'bool':
			return False
		if not re.match(r'^(true|false)$', self.content):
			return False
		return True

	def is_string(self):
		"""
			Return True if argument is string const, otherwise return False
		"""
		if self.type != 'string':
			return False
		# if not re.match(r'^([^\\]|\\([0-9]){3})*$', self.content): # ToDo: kontrolovat format stringu?
		# 	return False
		return True

	def is_gf_var(self):
		"""
			Return True if variable is in global frame, otherwise return False
		"""
		if re.match(r'^GF@*', self.content):
			return True
		else:
			return False

	def is_tf_var(self):
		"""
			Return True if variable is in temporary frame, otherwise return False
		"""
		if re.match(r'^TF@*', self.content):
			return True
		else:
			return False

	def is_lf_var(self):
		"""
			Return True if variable is in local frame, otherwise return False
		"""
		if re.match(r'^LF@*', self.content):
			return True
		else:
			return False

	def test_var(self):
		"""
			Test if argument of instruction is a variable
		"""
		if not self.is_var():
			error("ERR_LEX_SYN", CONST_RET_LEX_SYN)

	def test_sym(self):
		"""
			Test if argument of instruction is a symbol (variable or const)
		"""
		if not (self.is_var() or self.is_int() or self.is_string() or self.is_bool()):
			error("ERR_LEX_SYN", CONST_RET_LEX_SYN)

	def test_label(self):
		"""
			Test if argument of instruction is a label
		"""
		if not self.is_label():
			error("ERR_LEX_SYN", CONST_RET_LEX_SYN)

	def test_type(self):
		"""
			Test if argument of instruction is a type (int, string, bool)
		"""
		if not self.is_type():
			error("ERR_LEX_SYN", CONST_RET_LEX_SYN)

	def __str__(self):
		"""
			Define string conversion (for debugging)
		"""
		return "ARGUMENT:" + \
		"\ntype = " + str(self.type) + \
		"\ncontent = " + str(self.content) + "\n"


class Instruction:
	"""
		Class for representing instruction
	"""
	# order: int
	# opcode: str
	# args: list

	def __init__(self, order, opcode, args):
		"""
			Initialization of attributes
		"""
		self.order  = order
		self.opcode = opcode
		self.args   = args

	def test_num_args(self, expected):
		"""
			Test number of arguments of instruction
		"""
		if expected != len(self.args):
			error("ERR_LEX_SYN", CONST_RET_LEX_SYN)

	def __str__(self):
		"""
			Define string conversion (for debugging)
		"""
		args_of_inst = ""
		for arg in self.args:
			args_of_inst += str(arg)
		return "INSTRUCTION:" + \
		"\norder = " + str(self.order) + \
		"\nopcode = " + str(self.opcode) + \
		"\n" + args_of_inst

##########################################################################################################

if __name__ == '__main__':

	config = Config()
	config.parse_options(sys.argv)

	if config.help:
		print_help()

	parserXML = ParserXML(config.source_file)
	list_insts = parserXML.parse_xml()

	interpret = Interpret(list_insts)
	interpret.execute()

	if config.stats:
		config.print_stati()

	exit(0)
