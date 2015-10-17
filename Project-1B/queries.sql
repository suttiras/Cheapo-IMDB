SELECT CONCAT(Actor.first, ' ', Actor.last) as 'Actors in Die Another Day'
FROM Movie, MovieActor, Actor 
WHERE Movie.id = MovieActor.mid AND MovieActor.aid = Actor.id AND Movie.title = 'Die Another Day';