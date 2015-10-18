SELECT CONCAT(Actor.first, ' ', Actor.last) as 'Actors in Die Another Day'
FROM Movie, MovieActor, Actor 
WHERE Movie.id = MovieActor.mid AND MovieActor.aid = Actor.id AND Movie.title = 'Die Another Day';

--Get the concatenated first names and last names of the actors whose ID matches the actor ID listed in 
--MovieActor and the movie ID listed in MovieActor matches the movie ID listed in Movie that is 
--paired with "Die Another Day"

SELECT COUNT(*) as 'Total Actors who acted in Multiple Movies'
FROM 
(SELECT T.totalMovies FROM
(SELECT COUNT(*) as totalMovies FROM MovieActor, Actor 
WHERE MovieActor.aid = Actor.id
GROUP BY Actor.id) T
WHERE T.totalMovies > 1)P;

--Get the total number of movies each actor has acted, then eliminate the actors whose 
--total number of movies are less than 1, then get the total number of actors remaining
--(the total number of actors who have acted in multiple movies).