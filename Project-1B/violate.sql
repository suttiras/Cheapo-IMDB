--Let's pretend we start with a brand new database
INSERT INTO Movie VALUES(2, "Aladdin", 1992, "G","Walt Disney Studios");
INSERT INTO Movie VALUES(2, "The Simpsons Movie", 2007, "PG","20th Century Fox");
--violates the PRIMARY KEY constraint since ID's for both movies are the same.

INSERT INTO Movie VALUES(3, "The Simpsons Movie", -69, "PG","20th Century Fox");
--violates the CHECK constraint since the year is less than 1.

INSERT INTO Movie VALUES(-3, "The Simpsons Movie", -69, "PG","20th Century Fox");
--violates the CHECK constraint since the ID is less than 0.

INSERT INTO Actor VALUES(-3, "Black", "Jack", "Male",19690828,\N);
--violates the CHECK constraint since the ID is less than 0.

INSERT INTO Actor VALUES(3, "Black", "Jack", "Alien",19690828,\N);
--violates the CHECK constraint since the sex is not female or male.

--Let's pretend we start with a brand new database
INSERT INTO Actor VALUES(3, "Black", "Jack", "Male",19690828,\N);
INSERT INTO Actor VALUES(3, "Black", "Jack", "Male",19690828,\N);

(id INT PRIMARY KEY, last VARCHAR(20) NOT NULL, first VARCHAR(20) NOT NULL, dob DATE NOT NULL, 
dod DATE)

INSERT INTO Director VALUES(

INSERT INTO Review VALUES("Roger Ebert", "2015-10-18T01:04:25+00:00",1000,0,"This movie is an atrocity!");
--violates the Review CHECK constraint of having a rating between 1 and 5.

INSERT INTO Review VALUES("Roger Ebert", "2015-10-18T01:04:25+00:00",0,2,"This movie is not so good...");
--violates the Review CHECK constraint of having an mid > 0.

--Let's pretend we start with a brand new database
INSERT INTO Movie VALUES(2, "Aladdin", 1992, "G","Walt Disney Studios");
INSERT INTO Review VALUES("Roger Ebert", "2015-10-18T01:04:25+00:00",1,5,"This movie is outstanding!");
--violates the Review FOREIGN KEY constraint since there is no movie in Movie where the ID is 1.

