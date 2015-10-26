<html style = "font-family:Trebuchet MS">
<head>
	<title>IMDB: Cheapo Version</title>
</head>
<body>
<p style="text-align:center"><a href="./Homepage.php"><img src="ImdbLogo.png" alt="Website Logo"></a></p>

<h2 style="text-align:center">
Movie Information for:
</h2>
<!--Search-->
<p>
<form action="" method="GET">
<?php
if ($_GET["id"] == "")
{
	//echo "<h1 style='text-align:center'><b>ERROR! No information to display. Go to the Search page to find an actor/movie.</b></h1>";
	echo "<h1 style='text-align:center'>";
	echo 'No information to display. Click <b><a href="./search.php">here</a></b> to search!';
	echo "</h1>";
}

else if ($_GET["id"] != "")
{
	require_once('global_functions.php');
	$id = $_GET["id"];

	//list actors/roles
	
	$sql="SELECT title, year, rating, company FROM Movie WHERE id =" . $id; 
	$pdo_obj = get_pdo();
	
	foreach ($pdo_obj->query($sql) as $row){//Array or records stored in $row
	$formatted_name = $row[title];
	$formatted_year = $row[year];
	$formatted_rating = $row[rating];
	$formatted_company = $row[company];
	}
	echo "<h1 style='text-align:center'>";
	echo $formatted_name;
	echo " (" . $formatted_year . ")";
	echo "</h1>";
	echo "<h3> Actors </h3>";
	$actor_query = "SELECT CONCAT(Actor.first, ' ', Actor.last) as 'Actor1', MovieActor.role as role, Actor.id as id
	FROM Movie, MovieActor, Actor 
	WHERE Movie.id = MovieActor.mid AND MovieActor.aid = Actor.id AND Movie.id =" . $id;
	foreach ($pdo_obj->query($actor_query) as $row){//Array or records stored in $row
	$formatted_name = $row[Actor1];
	$role = $row[role];
	echo "<b>";
	//echo $formatted_name;
	$formattedURL = '<a href="./browse_actor.php?actor_name=' . $row[id] . '"';
	$formattedURL = $formattedURL . ">$formatted_name</a>";
	echo $formattedURL;
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
	//end of list director
	
	//get genre, rating, company
	echo "<br>";
	echo "<b>Rated: </b>" . $formatted_rating . "<br>";
	echo "<b>Production Company: </b>" . $formatted_company . "<br>";
	
	$genre_query = "SELECT genre
	FROM MovieGenre
	WHERE MovieGenre.mid = " . $id;
	echo "<br><b> Genre: </b><br>";
	foreach ($pdo_obj->query($genre_query) as $row){//Array or records stored in $row
	$formatted_genre = $row[genre];
	echo $formatted_genre;
	echo "<br>";
	}
	
	//end of genre, rating, company
	
	echo "<br>";
	
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
	$review_average = "<b> <p style = 'color:red'>AVERAGE RATING: " . $average_rating . "</b></p>";
	echo $review_average;
	//echo "<br>";
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