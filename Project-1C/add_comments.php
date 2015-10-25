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
Add Comments Page
</p>


<?php
// define variables and set to empty values
$movieNameErr = $ratingErr = $yearErr = $productionCompanyErr = $genreErr = "";
$movieName = $rating = $year = $productionCompany = $check_list[] = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {

   if (empty($_POST["movie_name"])) {
     $movieNameErr = "A movie name is required";
   } else {
     $movieName = test_input($_POST["movie_name"]);
	 }
	 
     // check if name only contains letters and whitespace
	 /*
     if (!preg_match("/^[a-zA-Z0-9 ]*$/",$movie_name) {
       $firstnameErr = "Only letters, numbers, and white space allowed"; 
     }
   }
   */

   
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
   <!--Name of Movie: <input type="text" name="movie_name" required>-->
   <!--<span class="error">* <?php echo $movieNameErr;?></span>-->
   <br>
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

   Rating:
	<select name="rating" required>
	<option value="1">*</option>
	<option value="2">**</option>
	<option value="3">***</option>
	<option value="4">****</option>
	<option value="5">*****</option>
	</select>
   <span class="error">* <?php echo $ratingErr;?></span>
   <br><br>
   Review:<span class="error">* <?php echo $ratingErr;?></span>
   <br>
   <TEXTAREA NAME="review" ROWS=5 COLS=50>

</TEXTAREA>
   <br><br>
   
   
 
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
<a href="./add_movie.php">Movie</a>
</p>
<p style="text-align:center">
Reviews
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