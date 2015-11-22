LOAD xsmall FROM 'xsmall.del' WITH INDEX
SELECT COUNT(*) FROM xsmall
SELECT * FROM xsmall WHERE key < 2500

LOAD small FROM 'small.del' WITH INDEX
SELECT COUNT(*) FROM small
SELECT * FROM small WHERE key > 100 AND key < 500

LOAD medium FROM 'medium.del' WITH INDEX
SELECT COUNT(*) FROM medium
SELECT * FROM medium WHERE key = 489
