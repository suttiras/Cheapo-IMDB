

INSERT INTO Review VALUES("Roger Ebert", "2015-10-18T01:04:25+00:00",1000,0,"This movie is an atrocity!");
--violates the Review CHECK constraint of having a rating between 1 and 5.

INSERT INTO Review VALUES("Roger Ebert", "2015-10-18T01:04:25+00:00",0,2,"This movie is not so good...");
--violates the Review CHECK constraint of having an mid > 0.

--Let's pretend we start with a brand new database
INSERT INTO Movie VALUES(2, "Aladdin", 1992, "G","Walt Disney Studios");
INSERT INTO Review VALUES("Roger Ebert", "2015-10-18T01:04:25+00:00",1,5,"This movie is outstanding!");
--violates the Review FOREIGN KEY constraint since there is no movie in Movie where the ID is 1.

