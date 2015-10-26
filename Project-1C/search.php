<html>
<head>
	<title>IMDB: Cheapo Version</title>
</head>
<body>
<p style="text-align:center"><a href="./Homepage.php"><img src="ImdbLogo.png" alt="Website Logo"></a></p>

<h2 style="text-align:center">
Search
</h2>

<!--Search-->
<p>
<form action="" method="GET">
<textarea name="query" cols="60" rows="1"></textarea><br />
<input type="submit" value="Submit"/>
<?php

if ($_GET["query"] == "")
{}


else if ($_GET["query"] != "")
{
	require_once('global_functions.php');
	$pdo_obj = get_pdo();
	echo "<br><br><b>The actor/movie you're searching for is: </b></br> </br>";
	echo $_GET["query"];
	echo "<br><br>";
	
	$search = $_GET["query"];
	$trimmedSearch = trim($search, " \t.");
	
	$queryMovie = 'SELECT id,title FROM Movie WHERE title LIKE "%';
	$queryActor = 'SELECT id,first,last FROM Actor WHERE first LIKE "%';
	$queryActor2 = 'SELECT id,first,last FROM Actor WHERE last LIKE "%';
	/*
	if (!preg_match("/^[0-9a-zA-Z ]*$/",$trimmedSearch)) {
       echo "Only numbers, letters and white space allowed"; 
     }*/
	
	//echo "Revised search is: ";
	//echo $trimmedSearch;
	//echo "<br><br>";
	
	//$db_connection = mysql_connect("localhost", "cs143", "");
	
	mysql_select_db("TEST", $db_connection);
	
	//$sanitized_name = mysql_real_escape_string($query, $db_connection);
	//$query2 = "SELECT 
	$searchTerms = explode(" ", $trimmedSearch);
	
	$first = false;
	
	//$clauses=array();
	foreach ($searchTerms as $part){
		//$clauses[]="title LIKE '%" . mysql_real_escape_string($part) . "%'";
		if (!$first)
		{
			$queryMovie = $queryMovie . $part . '%' . '"';
			//$queryMovie = $queryMovie . ":part" . '%';
			//new
			//$queryMovie = $pdo_obj->prepare($queryMovie);
			//$queryMovie->bindValue(':part', $part, PDO::PARAM_STR);
			//
			$first = true;
		}
		else
		{
			$queryMovie = $queryMovie . ' AND title LIKE "%' . $part . "%" . '"';
			//$queryMovie = $queryMovie . ' AND title LIKE %' . ":part" . '%';
			//new
			//$queryMovie = $pdo_obj->prepare($queryMovie);
			//$queryMovie->bindValue(':part', $part, PDO::PARAM_STR);
			//
		}
	}
	$count = 0;
	$first = false;
	$findActor = true;
	foreach ($searchTerms as $part){
		//$clauses[]="title LIKE '%" . mysql_real_escape_string($part) . "%'";
		if (!$first)
		{
			$queryActor = $queryActor . $part . '%' . '"';
			$queryActor2 = $queryActor2 . $part . '%' . '"';
			$first = true;
			$count = $count + 1;
		}
		else if ($count < 2)
		{
			$queryActor = $queryActor . ' AND last LIKE "%' . $part . "%" . '"';
			$queryActor2 = $queryActor2 . ' AND first LIKE "%' . $part . "%" . '"';
			$count = $count + 1;
		}
		else
		{
		/*
			if ($count > 2)
			{
				$findActor = false;
			}
			*/
			$count = $count + 1;
		}
	}
	//echo $count;
	if ($count > 2)
			{
				$findActor = false;
			}
	//echo $queryActor;
	//echo $queryActor2;
	$queryMoviePrep = $pdo_obj->prepare($queryMovie);
	if($findActor){
	$queryActorPrep = $pdo_obj->prepare($queryActor);
	$queryActor2Prep = $pdo_obj->prepare($queryActor2);
	}
	//$queryMovie = 'SELECT id, title FROM Movie WHERE title LIKE "%Joe%" AND "%Black%"';
	//$queryMoviePrep = $pdo_obj->prepare($queryMovie);
	$rs = $queryMoviePrep->execute();
	$rs1 = "";
	$rs2 = "";
	if($findActor){
	$rs1 = $queryActorPrep->execute();
	$rs2 = $queryActor2Prep->execute();
	}
	
	if ($rs == "")
	{
		echo "Couldn't find any movies. <br>";
	}
	else if ($rs1 == "" && $rs2 == "")
	{
		echo "Couldn't find any actor. <br>";
	}
	
	
//print results
if($findActor){
	echo "<b>Actor Database:</b><br>";
	//echo "<select name=actor_name value=''>Actor Name</option>"; // list box select command
	
	foreach ($pdo_obj->query($queryActor) as $row){//Array or records stored in $row
	$formatted_name = $row[first] . " " . $row[last] . "<br>" ;
	$formattedURL = '<a href="./browse_actor.php?actor_name=' . $row[id] . '"';
	$formattedURL = $formattedURL . ">$formatted_name</a>";
	echo $formattedURL;
	//echo "<br>";

	//echo "<option value=$row[id]>$formatted_name</option>"; 
	
}
	foreach ($pdo_obj->query($queryActor2) as $row){//Array or records stored in $row
	$formatted_name = $row[first] . " " . $row[last] . "<br>";
	$formattedURL = '<a href="./browse_actor.php?actor_name=' . $row[id] . '"';
	$formattedURL = $formattedURL . ">$formatted_name</a>";
	echo $formattedURL;
}

	echo "<br>";
	}
	
	echo "<b>Movie Database:</b><br>";
	//echo "<select name=title value=''>Movie Name</option>"; // list box select command
	
	foreach ($pdo_obj->query($queryMovie) as $row){//Array or records stored in $row
	$formatted_name = $row[title] . "<br>";
	//echo $formatted_name;
	$formattedURL = '<a href="./browse_movie.php?id=' . $row[id] . '"';
	$formattedURL = $formattedURL . ">$formatted_name</a>";
	echo $formattedURL;

}
   
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
<a href="./browse_actor.php">Actor</a>
</p>

<p style="text-align:center">
<a href="./browse_movie.php">Movie</a>
</p>

</body>
</html>