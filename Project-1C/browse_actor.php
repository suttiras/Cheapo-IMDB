<html>
<head>
	<title>IMDB: Cheapo Version</title>
</head>
<body>
<p style="text-align:center"><a href="./Homepage.php"><img src="ImdbLogo.png" alt="Website Logo"></a></p>

<h2>
Browse Actor Page
</h2>

<!--Search-->
<p>
<form action="" method="GET">
<textarea name="query" cols="60" rows="1"></textarea><br />
<input type="submit" value="Submit" />
<?php

if ($_GET["query"] == "")
{
	
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
Actor
</p>

<p style="text-align:center">
<a href="./browse_movie.php">Movie</a>
</p>

</body>
</html>