<html style = "font-family:Trebuchet MS">
<head>
	<title>IMDB: Cheapo Version</title>
</head>
<body>
<p style="text-align:center"><a href="./Homepage.php"><img src="ImdbLogo.png" alt="Website Logo"></a></p>

<h2 style="text-align:center">
Add Director to Movie
</h2>

<form method="GET" action=""> 
   Director:
   <?php require_once('global_functions.php');

	$sql="SELECT last,first,id FROM Director order by last"; 

	echo "<select name=director_name value=''>Director Name</option>"; // list box select command
	$pdo_obj = get_pdo();
	foreach ($pdo_obj->query($sql) as $row){//Array or records stored in $row
	$formatted_name = $row[last] . ", " . $row[first];
	echo "<option value=$row[id]>$formatted_name</option>"; 

}

 echo "</select>";// Closing of list box
   ?>
   <br><br>
   
   
   Movie Title:
   <?php require_once('global_functions.php');

	$sql="SELECT title,id FROM Movie order by title"; 

	echo "<select name=title value=''>Movie Title</option>"; // list box select command
	$pdo_obj = get_pdo();
	foreach ($pdo_obj->query($sql) as $row){//Array or records stored in $row
	echo "<option value=$row[id]>$row[title]</option>"; 

}

 echo "</select>";// Closing of list box
   ?>
   <br><br>
 
   <input type="submit" name="submit" value="Submit"> 
   <!--Now we parse the content-->
   <?php
   //MovieDirector(mid INT, did INT,
		$title_id = $_GET["title"];
		$director_name_2 = $_GET["director_name"];
		$submit = $_GET["submit"];
		
		if ($submit != "")
		{
			$addDirectorQuery = 'INSERT INTO MovieDirector(mid, did) VALUES(:mid, :did)';
			$pdo_obj = get_pdo();
			//echo $currentTime;
			$directorQueryStmt = $pdo_obj->prepare($addDirectorQuery);
			$directorQueryStmt->bindParam(':mid', $title_id, PDO::PARAM_INT);
			//$directorQueryStmt->bindParam(':role', $role_2, PDO::PARAM_STR);
			$directorQueryStmt->bindParam(':did', $director_name_2, PDO::PARAM_INT);
			
			if (!$directorQueryStmt->execute())
			{
				echo "Could not add director to Movie Director database.";
			}
			else
			{
				echo "Successfully added director to Movie Director database!";
			}
		}
   ?>

   
</form>

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
Add a Director to a Movie
</p>
<p></p>
<h2 style="text-align:center"><font size = "3"><b>Browse Database</b></font></h2>
<p style="text-align:center">
<a href="./browse_actor.php">Actor</a>
</p>

<p style="text-align:center">
<a href="./browse_movie.php">Movie</a>
</p>

</body>
</html>