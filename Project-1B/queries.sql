SELECT CONCAT(Actor.first, ' ', Actor.last) as 'Actors in Die Another Day'
FROM Movie, MovieActor, Actor 
WHERE Movie.id = MovieActor.mid AND MovieActor.aid = Actor.id AND Movie.title = 'Die Another Day';

--Get the first names and last names of the actors in Actor whose ID matches the actor ID listed in 
--MovieActor and the movie ID listed in MovieActor matches the movie ID listed in Movie that is 
--paired with "Die Another Day"

SELECT COUNT(*) as 'Number of Actors who Acted in Multiple Movies'
FROM (SELECT COUNT(*) 
FROM MovieActor
GROUP BY aid
HAVING COUNT(*) >= 2) T;

--Group the actors' ids in MovieActor and keep those who have acted in at least 2 movies, 
--and then count the number of movies that that actor acted in (with the number of movies 
--being represented in each row). Then count the total number of actors who have acted in 
--multiple movies by counting the number of rows.

SELECT Movie.title AS 'Movies That Brad Pitt Has Acted In'
FROM MovieActor, Actor, Movie
WHERE Actor.first = 'Brad' AND Actor.last = 'Pitt' AND MovieActor.aid = Actor.id AND MovieActor.mid = Movie.id;

--Match the actor's first name and last name to "Brad Pitt" and then made sure that the MovieActor's aid
--matched Actor's id. Also made sure that the MovieActor's movie id matched the Movie's id.