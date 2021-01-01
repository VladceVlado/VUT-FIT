-- Load the data in comma-separated-values format with a given schema
fielding =
	LOAD '$INPUT1'
	USING org.apache.pig.piggybank.storage.CSVExcelStorage
		(',', 'NO_MULTILINE', 'NOCHANGE', 'SKIP_INPUT_HEADER')
	AS (
		playerID : chararray, yearID : int, stint : int,
		teamID : chararray, lgID : chararray,
		POS : chararray, G : int, GS : int,
		InnOuts : int,
		PO : int, A : int, E : int, DP : int,
		PB : int, WP : int, SB : int, CS : int,
		ZR : double
	);

-- Filter the previously loaded data
fielding95 = FILTER fielding BY yearID >= 1995;

-- Arrange the data into groups according to given column(s)
groupedByTeam = GROUP fielding95 BY (playerID, teamID);

-- Apply an aggregate function on each group (flatten multicolumn group to n-tuple)
results = FOREACH groupedByTeam GENERATE FLATTEN(group) AS (playerID, teamID),
	AVG(fielding95.G) AS avgGames, SUM(fielding95.G) AS sumGames;

-- Filter the resulting data
results10ge = FILTER results BY avgGames >= 10;

-- Execute all the statements and ...
-- show output
--DUMP results10ge;
-- store the output into the output directory
STORE results10ge INTO '$OUTPUT'
	USING org.apache.pig.piggybank.storage.CSVExcelStorage
		(',', 'NO_MULTILINE', 'NOCHANGE', 'WRITE_OUTPUT_HEADER');
