--Let's start with a brand new database for all of these violation cases

INSERT INTO Movie VALUES(1, "Aladdin", 1992, "G","Walt Disney Studios");
INSERT INTO Movie VALUES(2, "Kung Fu Panda", 2008, "G","DreamWorks Studios");
INSERT INTO Movie VALUES(1, "The Simpsons Movie", 2007, "PG","20th Century Fox");
--violates the PRIMARY KEY constraint since ID's for both movies Aladdin and The Simpsons Movie are the same.

INSERT INTO Movie VALUES(3, "The Simpsons Movie", -69, "PG","20th Century Fox");
--violates the CHECK constraint since the year is less than 1.

INSERT INTO Movie VALUES(-3, "The Simpsons Movie", -69, "PG","20th Century Fox");
--violates the CHECK constraint since the ID is less than 0.

INSERT INTO Actor VALUES(-1, "Black", "Jack", "Male",19690828,\N);
--violates the CHECK constraint since the ID is less than 0.

INSERT INTO Actor VALUES(1, "Black", "Jack", "Alien",19690828,\N);
--violates the CHECK constraint since the sex is not female or male.

INSERT INTO Actor VALUES(1, "Black", "Jack", "Male",19690828,\N);
INSERT INTO Actor VALUES(1, "Evans", "Chris", "Male",19810613,\N);
--violates the PRIMARY KEY constraint since ID's for both actors are the same.

INSERT INTO Director VALUES(1, "Lucas", "George", 19440514, \N);
INSERT INTO Director VALUES(2, "Osborne", "Mark", 19700917, \N);

INSERT INTO Director VALUES(NULL, "Spielberg", "Steven", 19461218, \N);
--violates the PRIMARY KEY constraint since ID is NULL for the Steven Spielberg entry.

INSERT INTO Director VALUES(-1, "Spielberg", "Steven", 19461218, \N);
--violates the CHECK constraint since the ID is less than 0.

INSERT INTO MovieGenre VALUES(5, "Romance");
--violates FOREIGN KEY constraint since there is no movie with ID 5.

INSERT INTO MovieGenre VALUES(0, "Romance");
--violates CHECK constraint since the movie ID < 0.

INSERT INTO MovieGenre VALUES(2, "Children's");

INSERT INTO MovieDirector VALUES(6, 1);
--violates FOREIGN KEY constraint since there is no movie with ID 6.

INSERT INTO MovieDirector VALUES(1, 3);
--violates FOREIGN KEY constraint since there is no director with ID 3.

INSERT INTO MovieDirector VALUES(0, 1);
--violates CHECK constraint since the movie ID is not > 0.

INSERT INTO MovieDirector VALUES(1, -1);
--violates CHECK constraint since the director ID is not > 0.

INSERT INTO MovieDirector VALUES(2, 2);

INSERT INTO MovieActor(5, 1, "Po");
--violates FOREIGN KEY constraint since there is no movie with ID 5.

INSERT INTO MovieActor(2, 7000, "Po");
--violates FOREIGN KEY constraint since there is no Actor with ID 7000.

INSERT INTO MovieActor(-2, 2, "Po");
--violates CHECK constraint since the movie ID is not > 0.

INSERT INTO MovieActor(2, -2, "Po");
--violates CHECK constraint since the actor ID is not > 0.

INSERT INTO MovieActor(2, 1, "Po");

INSERT INTO Review VALUES("Roger Ebert", "2015-10-18T01:04:25+00:00",1000,0,"This movie is an atrocity!");
--violates the Review CHECK constraint of having a rating between 1 and 5.

INSERT INTO Review VALUES("Roger Ebert", "2015-10-18T01:04:25+00:00",0,2,"This movie is not so good...");
--violates the Review CHECK constraint of having an mid > 0.

INSERT INTO Review VALUES("Roger Ebert", "2015-10-18T01:04:25+00:00",5,5,"This movie is outstanding!");
--violates the Review FOREIGN KEY constraint since there is no movie in Movie where the ID is 5.

INSERT INTO MaxPersonID VALUES (-69000);
--violates the Review CHECK constraint of having an id > 0.

INSERT INTO MaxMovieID VALUES (-4750);
--violates the Review CHECK constraint of having an id > 0.