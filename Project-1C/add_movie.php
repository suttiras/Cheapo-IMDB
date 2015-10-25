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
   if (empty($_GET["movie_name"])) {
     $movieNameErr = "A movie name is required";
   } else {
     $movieName = test_input($_GET["movie_name"]);
     // check if name only contains letters and whitespace
	 /*
     if (!preg_match("/^[a-zA-Z0-9 ]*$/",$movie_name) {
       $firstnameErr = "Only letters, numbers, and white space allowed"; 
     }
	 */
   }
   if (empty($_GET["year"])) {
     $yearErr = "Year is required";
   } else {
     $year = test_input($_GET["year"]);
     // check if year has only numbers
     if (!preg_match("/^[0-9]*$/",$year)) { //doesn't completely work...
       $lastnameErr = "Only numbers allowed"; 
     }
   }

   if (empty($_GET["rating"])) {
     $ratingErr = "Rating is required.";
   } else {
     $rating = test_input($_GET["rating"]);
   }

   if (empty($_GET["productionCompany"])) {
     $productionCompanyErr = "Production company is required";
   } 
   else {
     $productionCompany = test_input($_GET["productionCompany"]);
   }
   
   if (empty($_GET["check_list"])) {
     $genreErr = "Genre is required";
   } 
   else {
     $check_list[] = test_input($_GET["check_list"]);
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
   <!--</form>-->
   Genre(s):
   <!--<form action="#" method="post" required>-->
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


 
   <input type="submit" name="submit" value="Submit"> 
   </form>
   
   
   <?php
		require_once('global_functions.php');
		//echo "Hello World!";
		$movie_name_2 = $_GET["movie_name"];
		$year_2 = $_GET["year"];
		$rating_2 = $_GET["rating"];
		$productionCompany_2 = $_GET["productionCompany"];
		$genre_2 = $_GET['check_list'];
		
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
					$MaxId2 = 'SELECT MAX(id) 
									FROM Movie';
					$maxIDQueryStmt2 = $pdo_obj->prepare($MaxId2);
					if (! $maxIDQueryStmt2->execute())
					{
						echo "Doesn't work...";
					}
					else
					{
						$MaxId = $maxIDQueryStmt2->fetch(PDO::FETCH_COLUMN, 0);
						$MaxId = $MaxId + 1;
						//echo "Got ID from Movie Table: $MaxId";	//debugging
						$success = true;
					}
				}
				else
				{
					$MaxId = $maxIDQueryStmt->fetch(PDO::FETCH_COLUMN, 0);
					//echo "Got Max ID from MaxMovieID Table is $MaxId";	//debugging
					$success = true;
					//TO DO:insert into MaxPersonID Table
					//$sql = 'INSERT id INTO MaxPersonID VALUES (:MaxId)';
					$MaxIdInsert = $pdo_obj->prepare('INSERT INTO MaxMovieID(id) VALUES (:MaxId)');
					
					$MaxIdInsert->bindParam(':MaxId', $MaxId, PDO::PARAM_INT);
					if(!$MaxIdInsert->execute())
					{
						echo "Failed to insert Max ID into MaxMovieID table.";
					}
					//echo $MaxIdInsert;
				}
				
				$add_query_1 = $pdo_obj->prepare('INSERT INTO Movie(id, title, year, rating, company) VALUES(:id, :movie_name_2, :year_2, :rating_2, :productionCompany_2)');
				$add_query_2 = $pdo_obj->prepare('INSERT INTO MovieGenre(mid, genre) VALUES(:id, :genre)');
				
				$add_query_1->bindParam(':id', $MaxId, PDO::PARAM_INT);
				$add_query_1->bindParam(':movie_name_2', $movie_name_2, PDO::PARAM_STR);
				$add_query_1->bindParam(':year_2', $year_2, PDO::PARAM_STR);
				$add_query_1->bindParam(':rating_2', $rating_2, PDO::PARAM_STR);
				$add_query_1->bindParam(':productionCompany_2', $productionCompany_2, PDO::PARAM_STR);
				
				$add_query_2->bindParam(':id', $MaxId, PDO::PARAM_INT);
				//testing purposes
				if (!$genre_2)
					echo "There is nothing in genre";
				foreach($genre_2 as $genre)
				{
					$add_query_2->bindParam(':genre', $genre, PDO::PARAM_STR);
					if(!$add_query_2->execute())
					{
						echo "Could not add genre to database.";
						$success = false;
					}
				}
				//TO DO: need to also insert into MovieGenre table
				
				
				
				if(!$add_query_1->execute())
				{
					echo "Could not add actor to database.";
					$success = false;
				}
			
			//after attempt to add to database
			if($success)
			{
				echo "<h3><b>You added the following to the database: </b></h3>";
				echo "<b>Movie: </b>$movie_name_2 <br>";
				echo "<b>Year: </b>$year_2 <br>";
				echo "<b>Rating: </b>$rating_2 <br>";
				echo "<b>Production Company: </b>$productionCompany_2 <br>";
				echo "<b>Genre(s): </b>";
				foreach($genre_2 as $genre)
				{
					echo $genre;
					echo '<br>';
				}
			}
			else
			{
				echo "Failed to add movie to database.";
			}
		}
		
	?>
   
   
   
   



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