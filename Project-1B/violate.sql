INSERT INTO Movie VALUES(10000, "Aladdin", 1992, "G","Walt Disney Studios");
INSERT INTO Movie VALUES(20000, "Kung Fu Panda", 2008, "G","DreamWorks Studios");
INSERT INTO Movie VALUES(10000, "The Simpsons Movie", 2007, "PG","20th Century Fox");
--violates the PRIMARY KEY constraint since ID's for both movies Aladdin and The Simpsons Movie are the same.
--ERROR 1062 (23000): Duplicate entry '10000' for key 'PRIMARY'

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
--violates the PRIMARY KEY constraint since ID's for both actors have an ID identical
--to another actor (Vincent Candela).
--ERROR 1062 (23000): Duplicate entry '10000' for key 'PRIMARY'

INSERT INTO Director VALUES(10000, "Lucas", "George", 19440514, \N);
INSERT INTO Director VALUES(20000, "Osborne", "Mark", 19700917, \N);

INSERT INTO Director VALUES(NULL, "Spielberg", "Steven", 19461218, \N);
--violates the PRIMARY KEY constraint since ID is NULL for the Steven Spielberg entry.
--ERROR 1048 (23000): Column 'id' cannot be null

INSERT INTO Director VALUES(-10000, "Spielberg", "Steven", 19461218, \N);
--violates the CHECK constraint since the ID is less than 0.

INSERT INTO MovieGenre VALUES(50000, "Romance");
--violates FOREIGN KEY constraint since there is no movie with ID 50000.
--ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails ('TEST'.
--'MovieGenre', CONSTRAINT 'MovieGenre_ibfk_1' FOREIGN KEY ('mid') REFERENCES 'Movie' ('id'))

INSERT INTO MovieGenre VALUES(0, "Romance");
--violates CHECK constraint since the movie ID < 0.

INSERT INTO MovieGenre VALUES(20000, "Children's");

INSERT INTO MovieDirector VALUES(60000, 10000);
--violates FOREIGN KEY constraint since there is no movie with ID 60000.
--ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails ('TEST'.
--'MovieDirector', CONSTRAINT 'MovieDirector_ibfk_1' FOREIGN KEY ('mid') REFERENCES 'Movie' ('id'))

INSERT INTO MovieDirector VALUES(10000, 30000);
--violates FOREIGN KEY constraint since there is no director with ID 30000.
--ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails ('TEST'.
--'MovieDirector', CONSTRAINT 'MovieDirector_ibfk_1' FOREIGN KEY ('mid') REFERENCES 'Director' ('id'))

INSERT INTO MovieDirector VALUES(0, 10000);
--violates CHECK constraint since the movie ID is not > 0.

INSERT INTO MovieDirector VALUES(10000, -10000);
--violates CHECK constraint since the director ID is not > 0.

INSERT INTO MovieDirector VALUES(20000, 20000);

INSERT INTO MovieActor VALUES(50000, 10000, "Po");
--violates FOREIGN KEY constraint since there is no movie with ID 50000.
--ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails ('TEST'.
--'MovieActor', CONSTRAINT 'MovieActor_ibfk_1' FOREIGN KEY ('mid') REFERENCES 'Movie' ('id'))

INSERT INTO MovieActor VALUES(20000, 70000000, "Po");
--violates FOREIGN KEY constraint since there is no Actor with ID 70000000.
--ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails ('TEST'.
--'MovieActor', CONSTRAINT 'MovieActor_ibfk_1' FOREIGN KEY ('aid') REFERENCES 'Actor' ('id'))

INSERT INTO MovieActor VALUES(-20000, 20000, "Po");
--violates CHECK constraint since the movie ID is not > 0.

INSERT INTO MovieActor VALUES(20000, -20000, "Po");
--violates CHECK constraint since the actor ID is not > 0.

INSERT INTO MovieActor VALUES(20000, 10000, "Po");

INSERT INTO Review VALUES("Roger Ebert", "2015-10-18T01:04:25+00:00",10000,0,"This movie is an atrocity!");
--violates the Review CHECK constraint of having a rating between 1 and 5.

INSERT INTO Review VALUES("Roger Ebert", "2015-10-18T01:04:25+00:00",0,2,"This movie is not so good...");
--violates the Review CHECK constraint of having an mid > 0.

INSERT INTO Review VALUES("Roger Ebert", "2015-10-18T01:04:25+00:00",50000,5,"This movie is outstanding!");
--violates the Review FOREIGN KEY constraint since there is no movie in Movie where the ID is 50000.
--ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails ('TEST'.
--'Review', CONSTRAINT 'Review_ibfk_1' FOREIGN KEY ('mid') REFERENCES 'Movie' ('id'))

INSERT INTO Review VALUES("Roger Ebert", "2015-10-18T01:04:25+00:00",10000,5,"This movie is outstanding!");

DELETE FROM Movie WHERE id = 10000;
--violates the Review FOREIGN KEY constraint since there will be no movie in Movie where the ID is 10000.
--ERROR 1451 (23000): Cannot delete or update a parent row: a foreign key constraint fails ('TEST'.
--'Review', CONSTRAINT 'Review_idfk_1' FOREIGN KEY ('mid') REFERENCES 'Movie' ('id')) 

UPDATE Movie 
SET id=300000
WHERE id=10000;
--violates the Review FOREIGN KEY constraint since there will be no movie in Movie where the ID is 10000.
--ERROR 1451 (23000): Cannot delete or update a parent row: a foreign key constraint fails ('TEST'.
--'Review', CONSTRAINT 'Review_idfk_1' FOREIGN KEY ('mid') REFERENCES 'Movie' ('id')) 

INSERT INTO MaxPersonID VALUES (-69000);
--violates the Review CHECK constraint of having an id > 0.

INSERT INTO MaxMovieID VALUES (-4750);
--violates the Review CHECK constraint of having an id > 0.