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
   
   Production Company: <input type="text" name="year" required>
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