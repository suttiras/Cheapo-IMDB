<html>
<head>
	<title>IMDB: Cheapo Version</title>
</head>
<body>
<p style="text-align:center"><a href="./Homepage.php"><img src="ImdbLogo.png" alt="Website Logo"></a></p>

<h2>
Search
</h2>

<!--Search-->
<p>
<form action="" method="GET">
<textarea name="query" cols="60" rows="1"></textarea><br />
<input type="submit" value="Submit" />
<?php

if ($_GET["query"] == "")
{
	
	echo "<br><br>";
	$db_connection = mysql_connect("localhost", "cs143", "");
	
	mysql_select_db("TEST", $db_connection);
	
	//$sanitized_name = mysql_real_escape_string($query, $db_connection);
	//$actorName = $_GET["query"];
	//$query2 = "SELECT 
	$rs = mysql_query("SELECT first, last, sex, dob, dod FROM Actor WHERE id = 49514", $db_connection);
	
	if (!$rs) {
		echo "<br><br>Could not successfully run query from DB: " . mysql_error();
		exit;
	}

	if (mysql_num_rows($rs) == 0) {
		echo "<br><br>No rows found.";
		exit;
	}

	$result = mysql_fetch_assoc($rs);
	
	echo "<table border = 1 cellspacing = 1><tr>";
	
	
	
	//echo $result;
	/*
	foreach (array_keys($result) as $col)
	{
		echo "<th>";
		echo $col;
		echo "</th>";
	}*/
	
	
	echo "</tr>";

	echo "<tr>";
	
	do
	{
	
		foreach($result as $col)
		{
			echo " ";
			echo $col; 
			echo " ";
		}
		echo "<br>";
	}while ($result = mysql_fetch_assoc($rs));
	
	
	echo "</table>";
	//End of Brad Pitt's info
//Start of movies
	
	
	$rs = mysql_query("SELECT Movie.title AS 'Movies That Brad Pitt Has Acted In', Movie.year
FROM MovieActor, Actor, Movie
WHERE Actor.id = 49514 AND MovieActor.aid = Actor.id AND MovieActor.mid = Movie.id", $db_connection);
	
	if (!$rs) {
		echo "<br><br>Could not successfully run query from DB: " . mysql_error();
		exit;
	}

	if (mysql_num_rows($rs) == 0) {
		echo "<br><br>No rows found.";
		exit;
	}

	$result = mysql_fetch_assoc($rs);
	
	echo "<table border = 1 cellspacing = 1><tr>";
	
	//echo $result;
	
	foreach (array_keys($result) as $col)
	{
		echo "<th>";
		echo $col;
		echo "</th>";
	}
	
	echo "</tr>";

	echo "<tr>";
	
	do
	{
		foreach($result as $col)
		{
			echo "<td>";
			echo $col; 
			echo "</td>";
		}
		echo "</tr>";
	}while ($result = mysql_fetch_assoc($rs));
	
	echo "</table>";
	
	mysql_free_result($result);
	mysql_close($db_connection);
	
}


else if ($_GET["query"] != "")
{
	require_once('global_functions.php');
	$pdo_obj = get_pdo();
	echo "<br><br><b>The actor/movie you're searching for is: </b></br> </br>";
	echo $_GET["query"];
	echo "<br><br>";
	
	$search = $_GET["query"];
	$trimmedSearch = trim($search, " \t.");
	
	$queryMovie = 'SELECT id,title FROM Movie WHERE title LIKE %';
	
	if (!preg_match("/^[0-9a-zA-Z ]*$/",$trimmedSearch)) {
       echo "Only numbers, letters and white space allowed"; 
     }
	
	echo "Revised search is: ";
	echo $trimmedSearch;
	echo "<br><br>";
	
	//$db_connection = mysql_connect("localhost", "cs143", "");
	
	mysql_select_db("TEST", $db_connection);
	
	//$sanitized_name = mysql_real_escape_string($query, $db_connection);
	//$query2 = "SELECT 
	$searchTerms = explode(" ", $trimmedSearch);
	
	$first = false;
	
	//$clauses=array();
	foreach ($searchTerms as $part){
		//function_description in my case ,  replace it with whatever u want in ur table
		//$clauses[]="title LIKE '%" . mysql_real_escape_string($part) . "%'";
		if (!$first)
		{
			$queryMovie = $queryMovie . $part . "%";
			$first = true;
		}
		else
		{
			$queryMovie = $queryMovie . " AND title LIKE %" . $part . "%";
		}
	}
	echo $queryMovie;
	$queryMoviePrep = $pdo_obj->prepare($queryMovie);
	/*$rs = $queryMoviePrep->execute();
	
	if ($rs == "")
	{
		echo "Couldn't find any movies...";
	}
	else
	{
		$result = mysql_fetch_assoc($rs);
		do
		{
			foreach($result as $col)
			{
				echo $col; 
				echo "<br>";
			}
		}while ($result = mysql_fetch_assoc($rs));
	}
	*/
	
//print results
	//echo "<select name=title value=''>Movie Name</option>"; // list box select command
	$pdo_obj = get_pdo();
	foreach ($pdo_obj->query($queryMovie) as $row){//Array or records stored in $row
	$formatted_name = $row[title];
	echo "<br>Got a movie! <br>";
	//echo "<option value=$row[id]>$formatted_name</option>"; 

}

 //echo "</select>";// Closing of list box
   
   echo "<br><br>";
	
	
//end of print results
	mysql_free_result($result);
	mysql_close($db_connection);
	
	
}
?>
</form>
<!--End Search-->

<p style="text-align:center">
Search
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