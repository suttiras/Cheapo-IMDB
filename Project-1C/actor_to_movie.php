<html>
<html>
<head>
	<title>IMDB: Cheapo Version</title>
</head>
<body>
<p style="text-align:center"><a href="./Homepage.php"><img src="ImdbLogo.png" alt="Website Logo"></a></p>

<h2 style="text-align:center">
Add Actor to Movie
</h2>

<form method="GET" action="" onsubmit="return checkForm(this);"> 
   Actor:
   <?php require_once('global_functions.php');

	$sql="SELECT last,first,id FROM Actor order by last"; 

	echo "<select name=actor_name value=''>Actor Name</option>"; // list box select command
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

   Role: <input type="text" name="role" required>
   <span class="error">* <?php echo $movieNameErr;?></span>
   <br>
   <br>
 
   <input type="submit" name="submit" value="Submit"> 
   <!--Now we parse the content-->
   <?php
   //MovieActor(mid INT, aid INT, role VARCHAR(50) NOT NULL
		$title_id = $_GET["title"];
		$actor_name_2 = $_GET["actor_name"];
		$role_2 = $_GET["role"];
		
		if ($role_2 != "")
		{
			$addActorQuery = 'INSERT INTO MovieActor(mid, aid, role) VALUES(:mid, :aid, :role)';
			$pdo_obj = get_pdo();
			//echo $currentTime;
			$actorQueryStmt = $pdo_obj->prepare($addActorQuery);
			$actorQueryStmt->bindParam(':mid', $title_id, PDO::PARAM_INT);
			$actorQueryStmt->bindParam(':role', $role_2, PDO::PARAM_STR);
			$actorQueryStmt->bindParam(':aid', $actor_name_2, PDO::PARAM_INT);
			
			if (!$actorQueryStmt->execute())
			{
				echo "Could not add actor to Movie Actor database.";
			}
			else
			{
				echo "Successfully added actor to Movie Actor database!";
			}
		}
		
   
   ?>

   
</form>


</p>


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
Add an Actor to a Movie
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
<a href="./browse_movie.php">Movie</a>
</p>

</body>
</html>