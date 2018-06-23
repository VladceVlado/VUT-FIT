**IPP-proj**  

VUT FIT 2BIT 2017/2018  
IPP (Principy programovacích jazyků a objektově-orientovaného programování)  
Projekt: Návrh, implementace, dokumentace a testování sady skriptů pro interpretaci nestrukturovaného imperativního jazyka IPPcode18.  

**Struktura repositáře**  

doc/  ... Dokumentace  
task/ ... Zadání  
test/ ... Adresář s testy

**Spuštění**  

	$ php7.0 parse.php --help
	$ php7.0 parse.php [--stats="file"] [--comments] [--loc]

	$ python3.6 interpret.py --help
	$ python3.6 interpret.py --source="file" [--stats="file"] [--insts] [--vars]

	$ php7.0 test.php --help
	$ php7.0 test.php [--directory="path"] [--recursive] [--parse-script="file"] [--int-script="file"]

**Implementovaná rozšíření**  

STATP  
STATI

