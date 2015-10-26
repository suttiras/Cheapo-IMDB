<html>
<head>
	<title>IMDB: Cheapo Version</title>
</head>
<body>
<p style="text-align:center"><a href="./Homepage.php"><img src="ImdbLogo.png" alt="Website Logo"></a></p>

<h2>
Movie Information for:
</h2>
<!--Search-->
<p>
<form action="" method="GET">
<?php

if ($_GET["id"] != "")
{
	require_once('global_functions.php');
	$id = $_GET["id"];

	//list actors/roles
	
	$sql="SELECT title FROM Movie WHERE id =" . $id; 
	$pdo_obj = get_pdo();
	
	foreach ($pdo_obj->query($sql) as $row){//Array or records stored in $row
	$formatted_name = $row[title];}
	echo "<h2>";
	echo $formatted_name;
	echo "<h2>";
	echo "<h3> Actors </h3>";
	$actor_query = "SELECT CONCAT(Actor.first, ' ', Actor.last) as 'Actor1', MovieActor.role as role
	FROM Movie, MovieActor, Actor 
	WHERE Movie.id = MovieActor.mid AND MovieActor.aid = Actor.id AND Movie.id =" . $id;
	foreach ($pdo_obj->query($actor_query) as $row){//Array or records stored in $row
	$formatted_name = $row[Actor1];
	$role = $row[role];
	echo "<b>";
	echo $formatted_name;
	echo "</b>";
	echo " as ";
	echo $role;
	echo "<br>";
	}
	//end of list actors/roles
	//list director
	$director_query = "SELECT CONCAT(Director.first, ' ', Director.last) as 'Director1'
	FROM Movie, MovieDirector, Director
	WHERE Movie.id = MovieDirector.mid AND MovieDirector.did = Director.id AND Movie.id =" . $id;
	echo "<br><b> Directed by: </b><br>";
	foreach ($pdo_obj->query($director_query) as $row){//Array or records stored in $row
	$formatted_name = $row[Director1];
	echo $formatted_name;
	echo "<br>";
	}
	echo "<br>";
	//end of list director
	/*
	Review(name VARCHAR(20) NOT NULL, time TIMESTAMP, mid INT NOT NULL, rating INT NOT NULL, comment
	*/
	
	//list reviews
	$review_query_2 = "SELECT AVG(Review.rating) as averageRtg FROM Review, Movie
	WHERE Movie.id = Review.mid AND Movie.id =" . $id;
	
	$review_query = "SELECT Review.name as name, Review.time as time, Review.rating as rating, Review.comment as comment
	FROM Review, Movie
	WHERE Movie.id = Review.mid AND Movie.id =" . $id;
	
	echo "<br><h2> Reviews </h2>";
	
	foreach ($pdo_obj->query($review_query_2) as $row){//Array or records stored in $row
	$average_rating = $row[averageRtg];
	//echo $formatted_name;
	$review_average = "<b> AVERAGE RATING: " . $average_rating . "</b><br>";
	echo $review_average;
	echo "<br>";
	}
	
	foreach ($pdo_obj->query($review_query) as $row){//Array or records stored in $row
	$formatted_name = $row[name];
	//echo $formatted_name;
	$review_title = "<b>" . $formatted_name . "</b>" . " rates this movie: <b>" . $row[rating] . " star(s) </b><br>";
	echo $review_title;
	echo $row[time];
	echo "<br>";
	echo $row[comment];
	echo "<br>";
	echo "<br>";
	}
	echo "<br>";
	//end of list reviews
}



?>
</form>
<!--End Search-->


<p style="text-align:center">
<a href="./search.php">Search</a>
</p>

<h2 style="text-align:center"><font size = "3"><b>Add to Database</b></font></h2>
<p style="text-align:center">
<a href="./add_person.php">Actor/Director</a>
</p>
<p style="text-align:center">
<a href="./add_movie.php">Movie</a>
</p>
<p style="text-align:center">
<a href="./add_comments.php">Reviews</a>
</p>
<p style="text-align:center">
<a href="./actor_to_movie.php">Add an Actor to a Movie</a>
</p>
<p style="text-align:center">
<a href="./director_to_movie.php">Add a Director to a Movie</a>
</p>
<p></p>
<h2 style="text-align:center"><font size = "3"><b>Browse Database</b></font></h2>
<p style="text-align:center">
<a href="./browse_actor.php">Actor</a>
</p>

<p style="text-align:center">
Movie
</p>

</body>
</html>