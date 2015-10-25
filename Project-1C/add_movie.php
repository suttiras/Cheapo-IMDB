<html>
<head>
	<title>IMDB: Cheapo Version</title>
</head>
<style>
.error {color: #FF0000;}
</style>
<body>
<p style="text-align:center"><a href="./Homepage.php"><img src="ImdbLogo.png" alt="Website Logo"></a></p>

<p>
Movie Page
</p>

<p>

<?php
// define variables and set to empty values
$movieNameErr = $ratingErr = $yearErr = $productionCompanyErr = $genreErr = "";
$movieName = $rating = $year = $productionCompany = $check_list[] = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
   if (empty($_POST["movie_name"])) {
     $movieNameErr = "A movie name is required";
   } else {
     $movieName = test_input($_POST["movie_name"]);
     // check if name only contains letters and whitespace
	 /*
     if (!preg_match("/^[a-zA-Z0-9 ]*$/",$movie_name) {
       $firstnameErr = "Only letters, numbers, and white space allowed"; 
     }
	 */
   }
   if (empty($_POST["year"])) {
     $yearErr = "Year is required";
   } else {
     $year = test_input($_POST["year"]);
     // check if year has only numbers
     if (!preg_match("/^[0-9]*$/",$year)) { //doesn't completely work...
       $lastnameErr = "Only numbers allowed"; 
     }
   }

   if (empty($_POST["rating"])) {
     $ratingErr = "Rating is required.";
   } else {
     $rating = test_input($_POST["rating"]);
   }

   if (empty($_POST["productionCompany"])) {
     $productionCompanyErr = "Production company is required";
   } 
   else {
     $productionCompany = test_input($_POST["productionCompany"]);
   }
   
   if (empty($_POST["check_list[]"])) {
     $genreErr = "Genre is required";
   } 
   else {
     $check_list[] = test_input($_POST["check_list[]"]);
   }
}
?>

<form method="GET" action=""> 
   Name of Movie: <input type="text" name="movie_name" required>
   <span class="error">* <?php echo $movieNameErr;?></span>
   <br><br>
   
   Year: <input type="text" name="year" required>
	<span class="error">* <?php echo $yearErr;?></span>
   <br><br>
   Rating:
	<select name="rating" required>
	<option value="G">G</option>
	<option value="PG">PG</option>
	<option value="PG-13">PG-13</option>
	<option value="R">R</option>
	<option value="NC-17">NC-17</option>
	<option value="surrendere">Unrated</option>
	</select>
   <span class="error">* <?php echo $ratingErr;?></span>
   <br><br>
   
   Production Company: <input type="text" name="productionCompany" required>
	<span class="error">* <?php echo $yearErr;?></span>
   <br><br>
   
   Genre(s):
   <form action="#" method="post" required>
   <span class="error">* <?php echo $ratingErr;?></span>
<input type="checkbox" name="check_list[]" value="Action"><label>Action</label>
<input type="checkbox" name="check_list[]" value="Adult"><label>Adult</label>
<input type="checkbox" name="check_list[]" value="Adventure"><label>Adventure</label><br/>
<input type="checkbox" name="check_list[]" value="Animation"><label>Animation</label>
<input type="checkbox" name="check_list[]" value="Comedy"><label>Comedy</label>
<input type="checkbox" name="check_list[]" value="Crime"><label>Crime</label><br/>
<input type="checkbox" name="check_list[]" value="Documentary"><label>Documentary</label>
<input type="checkbox" name="check_list[]" value="Drama"><label>Drama</label>
<input type="checkbox" name="check_list[]" value="Family"><label>Family</label><br/>
<input type="checkbox" name="check_list[]" value="Fantasy"><label>Fantasy</label>
<input type="checkbox" name="check_list[]" value="Horror"><label>Horror</label>
<input type="checkbox" name="check_list[]" value="Musical"><label>Musical</label><br/>
<input type="checkbox" name="check_list[]" value="Mystery"><label>Mystery</label>
<input type="checkbox" name="check_list[]" value="Romance"><label>Romance</label>
<input type="checkbox" name="check_list[]" value="Sci-Fi"><label>Sci-Fi</label><br/>
<input type="checkbox" name="check_list[]" value="Short"><label>Short</label>
<input type="checkbox" name="check_list[]" value="Thriller"><label>Thriller</label>
<input type="checkbox" name="check_list[]" value="War"><label>War</label><br/>
<input type="checkbox" name="check_list[]" value="Western"><label>Western</label><br/>

</form>
 
   <input type="submit" name="submit" value="Submit"> 
   
   
   
   <?php
		require_once('global_functions.php');
		//echo "Hello World!";
		$movie_name_2 = $_GET["movie_name"];
		$year_2 = $_GET["year"];
		$rating_2 = $_GET["rating"];
		$productionCompany_2 = $_GET["productionCompany"];
		$genre_2 = $_GET["check_list[]"];
		
		$success = false;
		if ($movie_name_2 != "" && $year_2 != "")
		{	
			//attempt to add to database
			
				//$maxId = mysql_query("SELECT MAX(id) FROM MaxPersonID;", $db_connection);
				$maxIdQuery = 'SELECT MAX(id)+1 FROM MaxMovieID';
				$pdo_obj = get_pdo();
				
				$maxIDQueryStmt = $pdo_obj->prepare($maxIdQuery);
				
				if (! $maxIDQueryStmt->execute())
				{
				
					//failed to get maxID from MaxID table. We have to get it another way.
					//$MaxId = 'SELECT MAX(id)+1 FROM Actor';
					$MaxId = 'SELECT MAX(id) 
									FROM Movie';
					$maxIDQueryStmt2 = $pdo_obj->prepare($MaxId);
					if (! $maxIDQueryStmt2->execute())
					{
						echo "Doesn't work...";
					}
					else
					{
						$MaxId2 = $maxIDQueryStmt2->fetch(PDO::FETCH_COLUMN, 0);
						$MaxId2 = $MaxId2 + 1;
						echo "Got ID from Movie Table: $MaxId2";	//debugging
						$success = true;
					}
				}
				else
				{
					$MaxId = $maxIDQueryStmt->fetch(PDO::FETCH_COLUMN, 0);
					echo "Got Max ID from MaxMovieID Table is $MaxId";	//debugging
					$success = true;
					//TO DO:insert into MaxPersonID Table
					//$sql = 'INSERT id INTO MaxPersonID VALUES (:MaxId)';
					$MaxIdInsert = $pdo_obj->prepare('INSERT INTO MaxMovieID(id) VALUES (:MaxId)');
					
					$MaxIdInsert->bindParam(':MaxId', $MaxId, PDO::PARAM_INT);
					if(!$MaxIdInsert->execute())
					{
						echo "Failed to insert Max ID into MaxPersonID table.";
					}
					//echo $MaxIdInsert;
				}
				
				$add_query = $pdo_obj->prepare('INSERT INTO Actor(id, last, first, sex, dob, dod) VALUES(:id, :last_name_2, :first_name_2, :gender_2, :dob_2, :dod_2)');
				$add_query->bindParam(':id', $MaxId, PDO::PARAM_INT);
				$add_query->bindParam(':last_name_2', $last_name_2, PDO::PARAM_STR);
				$add_query->bindParam(':first_name_2', $first_name_2, PDO::PARAM_STR);
				$add_query->bindParam(':gender_2', $gender_2, PDO::PARAM_STR);
				$add_query->bindParam(':dob_2', $dob_2, PDO::PARAM_INT);
				$add_query->bindParam(':dod_2', $dod_2, PDO::PARAM_INT);
				
				if(!$add_query->execute())
				{
					echo "Could not add actor to database.";
					$success = false;
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
Movie
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