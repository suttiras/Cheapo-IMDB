<html style = "font-family:Trebuchet MS">
<head>
	<title>IMDB: Cheapo Version</title>
</head>
<body>
<p style="text-align:center"><a href="./Homepage.php"><img src="ImdbLogo.png" alt="Website Logo"></a></p>

<h2 style='text-align:center'>
Actor Information for:
</h2>

<!--Search-->
<p>
<form action="" method="GET">
<?php
require_once('global_functions.php');

if ($_GET["actor_name"] != "")
{
	$id = $_GET["actor_name"];
	$pdo_obj = get_pdo();

	$actorQuery = "SELECT * FROM Actor WHERE id=" . $id;	

	$actorQueryStmt = $pdo_obj->prepare($actorQuery);
	
	$actor = $actorQueryStmt->execute();
	$row = $pdo_obj->query($actorQuery);
	foreach ($pdo_obj->query($actorQuery) as $row)
	{
			echo "<b>Name: </b>" . $row[first] . " " . $row[last] . "<br>";
			echo "<b>Gender: </b>" . $row[sex] . "<br>";
			echo "<b>Date of Birth: </b>" . $row[dob] . "<br>";
			echo "<b>Date of Death: </b>" . $row[dod] . "<br>";
	}
		
	$actorQuery1 = "SELECT title,role, mid FROM Movie, MovieActor WHERE MovieActor.mid = Movie.id AND MovieActor.aid=" . $id;

	$actorRoleStmt = $pdo_obj->prepare($actorQuery1);
	
	$actorRole = $actorRoleStmt->execute();
	
	$row = $pdo_obj->query($actorQuery1);
	echo "<b>Role(s): </b>";
	foreach ($pdo_obj->query($actorQuery1) as $row)
	{
		if ($row == "")
		{
			echo "There are no roles... <br>";
		}

			$formatted_name = $row[title] . "<br>";
			$formattedURL = '<a href="./browse_movie.php?id=' . $row[mid] . '"';
			$formattedURL = $formattedURL . ">$formatted_name</a>";
			
			echo $row[role] . "<b> in </b>" . $formattedURL;	
	}
}

else
{
	echo "<h1 style='text-align:center'>";
	echo 'No information to display. Click <b><a href="./search.php">here</a></b> to search!';
	echo "</h1>";
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