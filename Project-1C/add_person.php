
<html>
<head>
	<title>IMDB: Cheapo Version</title>
</head>
<style>
.error {color: #FF0000;}
</style>
<body>
<p style="text-align:center"><a href="./Homepage.php"><img src="ImdbLogo.png" alt="Website Logo"></a></p>

<h2 style="text-align:center">
Add Actor/Director Page
</h2>
<p>
<script type ="text/javascript">
	function checkForm(form)
	{
		var alphabet = /^[a-zA-Z ]*$/;
		if(!alphabet.test(form.first_name.value) || !alphabet.test(form.last_name.value)  )
		{
			alert("Error: Name(s) contain(s) invalid characters.");
			return false;
		}
		var date_valid = /^(?:(?:(?:(?:(?:[13579][26]|[2468][048])00)|(?:[0-9]{2}(?:(?:[13579][26])|(?:[2468][048]|0[48]))))(?:(?:(?:09|04|06|11)(?:0[1-9]|1[0-9]|2[0-9]|30))|(?:(?:01|03|05|07|08|10|12)(?:0[1-9]|1[0-9]|2[0-9]|3[01]))|(?:02(?:0[1-9]|1[0-9]|2[0-9]))))|(?:[0-9]{4}(?:(?:(?:09|04|06|11)(?:0[1-9]|1[0-9]|2[0-9]|30))|(?:(?:01|03|05|07|08|10|12)(?:0[1-9]|1[0-9]|2[0-9]|3[01]))|(?:02(?:[01][0-9]|2[0-8])))))$/;
		if(!date_valid.test(form.dob.value))
		{
			alert("Error: Date of birth is invalid.");
			return false;
		}
		if (form.dod.value != "" && !date_valid.test(form.dod.value))
		{
			alert("Error: Date of death is invalid.");
			return false;
		}
		if (form.dod.value != "" && form.dod.value < form.dob.value)
		{
			alert("Error: Date of death and birth are invalid.");
			return false;
		}
	}
</script>

<form method="GET" action="" onsubmit="return checkForm(this);"> 
	Actor or Director:
   <input checked= "checked" type="radio" name="actor_or_director" value='actor'>Actor
   <input type="radio" name="actor_or_director" value='director'>Director
   <span class="error">* </span>
   <br><br>
   First Name: <input type="text" name="first_name" required>
   <span class="error">* </span>
   <br><br>
   
   Last Name: <input type="text" name="last_name" required>
	<span class="error">* </span>
   <br><br>
   Gender:
   <input checked= "checked" type="radio" name="gender" value='Female'>Female
   <input type="radio" name="gender" value='Male'>Male
   <span class="error">* </span>
   <br><br>
   Date of Birth (yyyymmdd): <input type="number" name="dob" required>
	<span class="error">* </span>
   <br><br>
   Date of Death (yyyymmdd)(if applicable): <input type="number" name="dod">
   <br><br>

   <br><br>
   <input type="submit" name="submit" value="Submit">
   
   <p>
	<?php
		require_once('global_functions.php');
		//echo "Hello World!";
		$first_name_2 = $_GET["first_name"];
		$last_name_2 = $_GET["last_name"];
		$gender_2 = $_GET["gender"];
		$actor_or_director_2 = $_GET["actor_or_director"];
		$dob_2 = $_GET["dob"];
		$dod_2 = $_GET["dod"];
		
		$success = true;
		if ($first_name_2 != "" && $last_name_2 != "")
		{	
			//attempt to add to database
			if ($actor_or_director_2 == "actor")
			{
				if ($dod_2 == "")
				{
					$dod_2 = "\N";
				}
				//$maxId = mysql_query("SELECT MAX(id) FROM MaxPersonID;", $db_connection);
				$maxIdQuery = 'SELECT MAX(id)+1 FROM MaxPersonID';
				$pdo_obj = get_pdo();
				$maxIDQueryStmt = $pdo_obj->prepare($maxIdQuery);
				if (! $maxIDQueryStmt->execute())
				{
					//failed to get maxID from MaxID table. We have to get it another way.
					$MaxId = 'SELECT MAX(id)+1 FROM Actor';
					$MaxIdUnion = 'SELECT MAX(id) 
									FROM (SELECT id FROM Actor
									UNION
									SELECT id FROM Director) AS M';
					$maxIDQueryStmt2 = $pdo_obj->prepare($maxIdUnion);
					if (! $maxIDQueryStmt2->execute())
					{
						echo "Doesn't work...";
					}
					else
					{
						$MaxId2 = $maxIDQueryStmt2->fetch(PDO::FETCH_COLUMN, 0);
						echo $MaxId2;
					}
					
					//TO DO
				}
				else
				{
					$MaxId = $maxIDQueryStmt->fetch(PDO::FETCH_COLUMN, 0);
					//TO DO:insert into MaxPersonID Table
				}
				echo "Max ID is $MaxId";
				
				$add_query = "INSERT INTO Actor VALUES(100, $last_name_2, $first_name_2, $gender_2, $dob_2, $dod_2);"; 
			}
			
			
			//after attempt to add to database
			if($success)
			{
				echo "<h3><b>You added the following to the database: </b></h3>";
				echo "Actor or Director: $actor_or_director_2 <br>";
				echo "First name: $first_name_2 <br>";
				echo "Last name: $last_name_2 <br>";
				echo "Gender: $gender_2 <br>";
				echo "DOB: $dob_2 <br>";
				echo "DOD: $dod_2 <br>";
			}
			else
			{
				echo "Failed to add actor to database.";
			}
		}
		
	?>
</p>
   
</form>

</p>

<p style="text-align:center">
<a href="./search.php">Search</a>
</p>

<h2 style="text-align:center"><font size = "3"><b>Add to Database</b></font></h2>
<p style="text-align:center">
Actor/Director
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