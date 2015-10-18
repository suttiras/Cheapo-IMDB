SELECT CONCAT(Actor.first, ' ', Actor.last) as 'Actors in Die Another Day'
FROM Movie, MovieActor, Actor 
WHERE Movie.id = MovieActor.mid AND MovieActor.aid = Actor.id AND Movie.title = 'Die Another Day';

--Get the first names and last names of the actors in Actor whose ID matches the actor ID listed in 
--MovieActor and the movie ID listed in MovieActor matches the movie ID listed in Movie that is 
--paired with "Die Another Day"