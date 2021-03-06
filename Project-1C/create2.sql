CREATE TABLE Movie(id INT PRIMARY KEY, title VARCHAR(100) NOT NULL, year INT, rating VARCHAR(10), company VARCHAR(50),
CHECK (year > 0),
CHECK (id > 0)
)ENGINE = INNODB;

CREATE TABLE Actor(id INT PRIMARY KEY, last VARCHAR(20) NOT NULL, first VARCHAR(20) NOT NULL, sex VARCHAR(6), 
dob DATE NOT NULL, dod DATE,
CHECK (id > 0),
CHECK (sex = 'Female' OR sex = 'Male')
)ENGINE = INNODB;

CREATE TABLE Director(id INT PRIMARY KEY, last VARCHAR(20) NOT NULL, first VARCHAR(20) NOT NULL, dob DATE NOT NULL, 
dod DATE,
CHECK (id > 0))ENGINE = INNODB;

CREATE TABLE MovieGenre(mid INT, genre VARCHAR(20) NOT NULL, CHECK (mid > 0),
FOREIGN KEY (mid) references Movie(id)
)ENGINE = INNODB;

CREATE TABLE MovieDirector(mid INT, did INT, CHECK (mid > 0), CHECK (did > 0),
FOREIGN KEY (did) references Director(id),
FOREIGN KEY (mid) references Movie(id)
)ENGINE = INNODB;

CREATE TABLE MovieActor(mid INT, aid INT, role VARCHAR(50) NOT NULL,
CHECK (mid > 0),
CHECK (aid > 0),
FOREIGN KEY (aid) references Actor(id),
FOREIGN KEY (mid) references Movie(id)
)ENGINE = INNODB;

CREATE TABLE Review(name VARCHAR(20) NOT NULL, time TIMESTAMP, mid INT NOT NULL, rating INT NOT NULL, comment VARCHAR(500),
CHECK (rating BETWEEN 1 and 5),
CHECK (mid > 0),
FOREIGN KEY (mid) references Movie(id)
)ENGINE = INNODB;

CREATE TABLE MaxPersonID(id INT, CHECK (id > 0))ENGINE = INNODB;
CREATE TABLE MaxMovieID(id INT, CHECK (id > 0))ENGINE = INNODB;

INSERT INTO MaxPersonID VALUES (69000);
INSERT INTO MaxMovieID VALUES (4750);