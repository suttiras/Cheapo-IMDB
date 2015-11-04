CREATE TABLE Movie(id INT PRIMARY KEY, title VARCHAR(100) NOT NULL, year INT, rating VARCHAR(10), company VARCHAR(50),
CHECK (year > 0),
CHECK (id > 0)
)ENGINE = INNODB;
--Ensure that the Movie's id is unique, not null and at least 1. Check that the year of the movie is not negative 
--(or at the 0th year, since it is impossible). Check that the title of the movie isn't NULL.

CREATE TABLE Actor(id INT PRIMARY KEY, last VARCHAR(20) NOT NULL, first VARCHAR(20) NOT NULL, sex VARCHAR(6), 
dob DATE NOT NULL, dod DATE,
CHECK (id > 0),
CHECK (sex = 'Female' OR sex = 'Male')
)ENGINE = INNODB;
--Ensure that the Actor's id is unique, not null and at least 1. Check that the date of birth is not 
--null, since the Actor must exist.
--Ensure that the Actor's first and last name are not null, since they must have a full name.
--Ensure that the sex of the Actor is either Male or Female.

CREATE TABLE Director(id INT PRIMARY KEY, last VARCHAR(20) NOT NULL, first VARCHAR(20) NOT NULL, dob DATE NOT NULL, 
dod DATE,
CHECK (id > 0))ENGINE = INNODB;
--Ensure that the Director's id is unique, not null and at least 1. Check that the date of birth is not 
--null, since the Director must exist.
--Ensure that the Director's first and last name are not null, since they must have a full name.

CREATE TABLE MovieGenre(mid INT, genre VARCHAR(20) NOT NULL, CHECK (mid > 0),
FOREIGN KEY (mid) references Movie(id)
)ENGINE = INNODB;
--Ensure that the movie ID exists in the Movie table, or else it refers to a movie that does not exist.
--Ensure that the movie ID is greater than 0.

CREATE TABLE MovieDirector(mid INT, did INT, CHECK (mid > 0), CHECK (did > 0),
FOREIGN KEY (did) references Director(id),
FOREIGN KEY (mid) references Movie(id)
)ENGINE = INNODB;
--Ensure that the movie ID exists in the Movie table, or else it refers to a movie that does not exist.
--Ensure that the director ID exists in the Director table, or else it refers to a director that does not exist.
--Ensure that the movie and director ID are greater than 0.

CREATE TABLE MovieActor(mid INT, aid INT, role VARCHAR(50) NOT NULL,
CHECK (mid > 0),
CHECK (aid > 0),
FOREIGN KEY (aid) references Actor(id),
FOREIGN KEY (mid) references Movie(id)
)ENGINE = INNODB;
--Ensure that the movie ID exists in the Movie table, or else it refers to a movie that does not exist.
--Ensure that the actor ID exists in the Actor table, or else it refers to an actor that does not exist.
--Ensure that the movie and actor ID are greater than 0.
--Ensure that the role is not null, because the actor must have played some role; the role must exist.

CREATE TABLE Review(name VARCHAR(20) NOT NULL, time TIMESTAMP, mid INT NOT NULL, rating INT NOT NULL, comment VARCHAR(500),
CHECK (rating BETWEEN 1 and 5),
CHECK (mid > 0),
FOREIGN KEY (mid) references Movie(id)
)ENGINE = INNODB;
--Ensure that the name of the reviewer is not null. The reviewer must have a name.
--Ensure that the movie ID is not null, greater than 0, and refers to a movie that exists in the Movie table.
--Otherwise, the movie that the review is referring to does not exist.
--Ensure that the rating is between 1 and 5, or else the rating is invalid. The rating must be 1 to 5 stars.

CREATE TABLE MaxPersonID(id INT NOT NULL, CHECK (id > 0))ENGINE = INNODB;
CREATE TABLE MaxMovieID(id INT NOT NULL, CHECK (id > 0))ENGINE = INNODB;
--Ensure that the id is greater than 0 and not null, or else it is invalid. Otherwise, it cannot exist in the
--Movie or Actor or Director table.

INSERT INTO MaxPersonID VALUES (69000);
INSERT INTO MaxMovieID VALUES (4750);