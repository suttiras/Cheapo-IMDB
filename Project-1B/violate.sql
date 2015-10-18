--Let's start with a brand new database for all of these violation cases

INSERT INTO Movie VALUES(10000, "Aladdin", 1992, "G","Walt Disney Studios");
INSERT INTO Movie VALUES(20000, "Kung Fu Panda", 2008, "G","DreamWorks Studios");
INSERT INTO Movie VALUES(10000, "The Simpsons Movie", 2007, "PG","20th Century Fox");
--violates the PRIMARY KEY constraint since ID's for both movies Aladdin and The Simpsons Movie are the same.

INSERT INTO Movie VALUES(30000, "The Simpsons Movie", -69, "PG","20th Century Fox");
--violates the CHECK constraint since the year is less than 1.

INSERT INTO Movie VALUES(-30000, "The Simpsons Movie", 2007, "PG","20th Century Fox");
--violates the CHECK constraint since the ID is less than 0.

INSERT INTO Actor VALUES(-10000, "Black", "Jack", "Male",19690828,\N);
--violates the CHECK constraint since the ID is less than 0.

INSERT INTO Actor VALUES(10000, "Black", "Jack", "Alien",19690828,\N);
--violates the CHECK constraint since the sex is not female or male.

INSERT INTO Actor VALUES(10000, "Black", "Jack", "Male",19690828,\N);
INSERT INTO Actor VALUES(10000, "Evans", "Chris", "Male",19810613,\N);
--violates the PRIMARY KEY constraint since ID's for both actors are the same.

INSERT INTO Director VALUES(10000, "Lucas", "George", 19440514, \N);
INSERT INTO Director VALUES(20000, "Osborne", "Mark", 19700917, \N);

INSERT INTO Director VALUES(NULL, "Spielberg", "Steven", 19461218, \N);
--violates the PRIMARY KEY constraint since ID is NULL for the Steven Spielberg entry.

INSERT INTO Director VALUES(-10000, "Spielberg", "Steven", 19461218, \N);
--violates the CHECK constraint since the ID is less than 0.

INSERT INTO MovieGenre VALUES(50000, "Romance");
--violates FOREIGN KEY constraint since there is no movie with ID 50000.

INSERT INTO MovieGenre VALUES(0, "Romance");
--violates CHECK constraint since the movie ID < 0.

INSERT INTO MovieGenre VALUES(20000, "Children's");

INSERT INTO MovieDirector VALUES(60000, 10000);
--violates FOREIGN KEY constraint since there is no movie with ID 60000.

INSERT INTO MovieDirector VALUES(10000, 30000);
--violates FOREIGN KEY constraint since there is no director with ID 30000.

INSERT INTO MovieDirector VALUES(0, 10000);
--violates CHECK constraint since the movie ID is not > 0.

INSERT INTO MovieDirector VALUES(10000, -10000);
--violates CHECK constraint since the director ID is not > 0.

INSERT INTO MovieDirector VALUES(20000, 20000);

INSERT INTO MovieActor(50000, 10000, "Po");
--violates FOREIGN KEY constraint since there is no movie with ID 50000.

INSERT INTO MovieActor(20000, 70000000, "Po");
--violates FOREIGN KEY constraint since there is no Actor with ID 70000000.

INSERT INTO MovieActor(-20000, 20000, "Po");
--violates CHECK constraint since the movie ID is not > 0.

INSERT INTO MovieActor(20000, -20000, "Po");
--violates CHECK constraint since the actor ID is not > 0.

INSERT INTO MovieActor(20000, 10000, "Po");

INSERT INTO Review VALUES("Roger Ebert", "2015-10-18T01:04:25+00:00",10000,0,"This movie is an atrocity!");
--violates the Review CHECK constraint of having a rating between 1 and 5.

INSERT INTO Review VALUES("Roger Ebert", "2015-10-18T01:04:25+00:00",0,2,"This movie is not so good...");
--violates the Review CHECK constraint of having an mid > 0.

INSERT INTO Review VALUES("Roger Ebert", "2015-10-18T01:04:25+00:00",50000,5,"This movie is outstanding!");
--violates the Review FOREIGN KEY constraint since there is no movie in Movie where the ID is 50000.

INSERT INTO MaxPersonID VALUES (-69000);
--violates the Review CHECK constraint of having an id > 0.

INSERT INTO MaxMovieID VALUES (-4750);
--violates the Review CHECK constraint of having an id > 0.